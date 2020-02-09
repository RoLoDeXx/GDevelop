// @flow
import { Trans } from '@lingui/macro';

import * as React from 'react';
import Dialog from '../../UI/Dialog';
import FlatButton from '../../UI/FlatButton';
import InstructionEditor from './index.js';
import {
  type ResourceSource,
  type ChooseResourceFunction,
} from '../../ResourcesList/ResourceSource.flow';
import { type ResourceExternalEditor } from '../../ResourcesList/ResourceExternalEditor.flow';
import { type EventsScope } from '../EventsScope.flow';

type Props = {|
  project: gdProject,
  scope: EventsScope,
  globalObjectsContainer: gdObjectsContainer,
  objectsContainer: gdObjectsContainer,
  instruction: gdInstruction,
  isCondition: boolean,
  resourceSources: Array<ResourceSource>,
  onChooseResource: ChooseResourceFunction,
  resourceExternalEditors: Array<ResourceExternalEditor>,
  style?: Object,
  isNewInstruction: boolean,
  onCancel: () => void,
  onSubmit: () => void,
  open: boolean,
  openInstructionOrExpression: (
    extension: gdPlatformExtension,
    type: string
  ) => void,
  anchorEl?: any, // Unused
|};
type State = {||};

export default class InstructionEditorDialog extends React.Component<
  Props,
  State
> {
  render() {
    const {
      isNewInstruction,
      onCancel,
      onSubmit,
      open,
      ...otherProps
    } = this.props;
    const actions = [
      <FlatButton
        key="cancel"
        label={<Trans>Cancel</Trans>}
        primary={false}
        onClick={onCancel}
      />,
      <FlatButton
        key="ok"
        label={<Trans>Ok</Trans>}
        primary={true}
        keyboardFocused={false}
        onClick={onSubmit}
      />,
    ];

    return (
      <Dialog
        actions={actions}
        open={open}
        onRequestClose={onCancel}
        maxWidth={false}
        flexBody
        noMargin
      >
        <InstructionEditor {...otherProps} />
      </Dialog>
    );
  }
}
