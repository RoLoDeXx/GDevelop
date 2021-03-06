/*
 * GDevelop Core
 * Copyright 2008-present Florian Rival (Florian.Rival@gmail.com). All rights
 * reserved. This project is released under the MIT License.
 */
#include "GDCore/Events/Parsers/ExpressionParser2.h"
#include <algorithm>
#include <memory>
#include <vector>
#include "GDCore/CommonTools.h"
#include "GDCore/Events/Expression.h"
#include "GDCore/Extensions/Metadata/ExpressionMetadata.h"
#include "GDCore/Extensions/Metadata/InstructionMetadata.h"
#include "GDCore/Extensions/Metadata/MetadataProvider.h"
#include "GDCore/Extensions/Platform.h"
#include "GDCore/Project/Layout.h"
#include "GDCore/Project/Project.h"
#include "GDCore/Tools/Localization.h"
#include "GDCore/Tools/MakeUnique.h"

using namespace std;

namespace gd {

gd::String ExpressionParser2::NUMBER_FIRST_CHAR = ".0123456789";
gd::String ExpressionParser2::DOT = ".";
gd::String ExpressionParser2::PARAMETERS_SEPARATOR = ",";
gd::String ExpressionParser2::QUOTE = "\"";
gd::String ExpressionParser2::BRACKETS = "()[]{}";
gd::String ExpressionParser2::EXPRESSION_OPERATORS = "+-<>?^=\\:!";
gd::String ExpressionParser2::TERM_OPERATORS = "/*";
gd::String ExpressionParser2::UNARY_OPERATORS = "+-";
gd::String ExpressionParser2::WHITESPACES = " \n\r";
gd::String ExpressionParser2::NAMESPACE_SEPARATOR = "::";

ExpressionParser2::ExpressionParser2(
    const gd::Platform& platform_,
    const gd::ObjectsContainer& globalObjectsContainer_,
    const gd::ObjectsContainer& objectsContainer_)
    : expression(""),
      currentPosition(0),
      platform(platform_),
      globalObjectsContainer(globalObjectsContainer_),
      objectsContainer(objectsContainer_) {}

namespace {
/**
 * Return the minimum number of parameters, starting from a given parameter
 * (by convention, 1 for object functions and 2 for behavior functions).
 */
size_t GetMinimumParametersNumber(
    const std::vector<gd::ParameterMetadata>& parameters,
    size_t initialParameterIndex) {
  size_t nb = 0;
  for (std::size_t i = initialParameterIndex; i < parameters.size(); ++i) {
    if (!parameters[i].optional && !parameters[i].codeOnly) nb++;
  }

  return nb;
}

/**
 * Return the maximum number of parameters, starting from a given parameter
 * (by convention, 1 for object functions and 2 for behavior functions).
 */
size_t GetMaximumParametersNumber(
    const std::vector<gd::ParameterMetadata>& parameters,
    size_t initialParameterIndex) {
  size_t nb = 0;
  for (std::size_t i = initialParameterIndex; i < parameters.size(); ++i) {
    if (!parameters[i].codeOnly) nb++;
  }

  return nb;
}
}  // namespace

std::unique_ptr<ExpressionParserDiagnostic> ExpressionParser2::ValidateFunction(
    const gd::FunctionNode& function, size_t functionStartPosition) {
  if (gd::MetadataProvider::IsBadExpressionMetadata(
          function.expressionMetadata)) {
    return gd::make_unique<ExpressionParserError>(
        "invalid_function_name",
        _("Cannot find an expression with this name: ") +
            function.functionName + "\n" +
            _("Double check that you've not made any typo in the name."),
        functionStartPosition,
        GetCurrentPosition());
  }

  size_t minParametersCount = GetMinimumParametersNumber(
      function.expressionMetadata.parameters,
      WrittenParametersFirstIndex(function.objectName, function.behaviorName));
  size_t maxParametersCount = GetMaximumParametersNumber(
      function.expressionMetadata.parameters,
      WrittenParametersFirstIndex(function.objectName, function.behaviorName));
  if (function.parameters.size() < minParametersCount ||
      function.parameters.size() > maxParametersCount) {
    gd::String expectedCountMessage =
        minParametersCount == maxParametersCount
            ? _("The number of parameters must be exactly ") +
                  gd::String::From(minParametersCount)
            : _("The number of parameters must be: ") +
                  gd::String::From(minParametersCount) + "-" +
                  gd::String::From(maxParametersCount);

    if (function.parameters.size() < minParametersCount) {
      return gd::make_unique<ExpressionParserError>(
          "too_few_parameters",
          "You have not entered enough parameters for the expression. " +
              expectedCountMessage,
          functionStartPosition,
          GetCurrentPosition());
    }
  }

  return gd::make_unique<ExpressionParserDiagnostic>();
}

std::unique_ptr<TextNode> ExpressionParser2::ReadText() {
  size_t textStartPosition = GetCurrentPosition();
  SkipWhitespace();
  if (!IsAnyChar("\"")) {
    auto text = gd::make_unique<TextNode>("");
    text->diagnostic =
        RaiseSyntaxError(_("A text must start with a double quote (\")."));
    text->location = ExpressionParserLocation(textStartPosition, GetCurrentPosition());
    return text;
  }
  SkipChar();

  gd::String parsedText = "";
  bool textParsingHasEnded = false;
  bool expectEscapedCharacter = false;
  while (!IsEndReached() && !textParsingHasEnded) {
    if (GetCurrentChar() == '"') {
      if (expectEscapedCharacter) {
        parsedText += '"';
        expectEscapedCharacter = false;
      } else {
        textParsingHasEnded = true;
      }
    } else if (GetCurrentChar() == '\\') {
      if (expectEscapedCharacter) {
        parsedText += '\\';
        expectEscapedCharacter = false;
      } else {
        expectEscapedCharacter = true;
      }
    } else {
      if (expectEscapedCharacter) {
        parsedText += '\\';
      }

      parsedText += GetCurrentChar();
    }

    currentPosition++;
  }

  auto text = gd::make_unique<TextNode>(parsedText);
  text->location = ExpressionParserLocation(textStartPosition, GetCurrentPosition());
  if (!textParsingHasEnded) {
    text->diagnostic =
        RaiseSyntaxError(_("A text must end with a double quote (\"). Add a "
                           "double quote to terminate the text."));
  }

  return text;
}

std::unique_ptr<NumberNode> ExpressionParser2::ReadNumber() {
  size_t numberStartPosition = GetCurrentPosition();
  SkipWhitespace();
  gd::String parsedNumber;

  bool numberHasStarted = false;
  bool digitFound = false;
  bool dotFound = false;
  while (!IsEndReached()) {
    if (IsAnyChar("0")) {
      numberHasStarted = true;
      digitFound = true;
      if (!parsedNumber.empty()) { // Ignore leading 0s.
        parsedNumber += GetCurrentChar();
      }
    } else if (IsAnyChar("123456789")) {
      numberHasStarted = true;
      digitFound = true;
      parsedNumber += GetCurrentChar();
    } else if (IsAnyChar(".") && !dotFound) {
      numberHasStarted = true;
      dotFound = true;
      if (parsedNumber == "") {
        parsedNumber += "0."; //Normalize by adding a leading 0, only in this case.
      } else {
        parsedNumber += ".";
      }
    } else {
      break;
    }

    currentPosition++;
  }

  // parsedNumber can be empty in the only case where we have only seen
  // 0s (one or more), so normalize it to a single 0.
  if (parsedNumber.empty()) {
    parsedNumber = "0";
  }

  // Note that parsedNumber can finish by a dot (1., 2., 0.). This is
  // valid in most languages so we allow this.

  auto number = gd::make_unique<NumberNode>(parsedNumber);
  number->location = ExpressionParserLocation(numberStartPosition, GetCurrentPosition());
  if (!numberHasStarted || !digitFound) {
    number->diagnostic = RaiseSyntaxError(
        _("A number was expected. You must enter a number here."));
  }

  return number;
}

}  // namespace gd
