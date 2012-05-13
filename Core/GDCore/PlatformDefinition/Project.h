/** \file
 *  Game Develop
 *  2008-2012 Florian Rival (Florian.Rival@gmail.com)
 */

#ifndef GDCORE_PROJECT_H
#define GDCORE_PROJECT_H
#include <string>
#include <vector>
#include "GDCore/PlatformDefinition/ObjectGroup.h"
namespace gd { class Platform; }
namespace gd { class Layout; }
namespace gd { class ExternalEvents; }
namespace gd { class Object; }
namespace gd { class VariablesContainer; }
#undef GetObject //Disable an annoying macro

namespace gd
{
/**
 * \brief Base class used to represent a project of a platform
 */
class GD_CORE_API Project
{
public:
    Project();
    virtual ~Project();

    /** \name Common properties
     * Some properties for the project
     */
    ///@{

    /**
     * Must change the name of the project with the name passed as parameter.
     */
    virtual void SetName(const std::string & name) =0;

    /**
     * Must return the name of the project.
     */
    virtual const std::string & GetName() const =0;

    /**
     * Must change the name of the project with the name passed as parameter.
     */
    virtual void SetAuthor(const std::string & name) =0;

    /**
     * Must return the name of the project.
     */
    virtual const std::string & GetAuthor() =0;

    /**
     * Must change the default width of the project main window
     */
    virtual void SetMainWindowDefaultWidth(unsigned int width) =0;

    /**
     * Must return the default width of the project main window
     */
    virtual unsigned int GetMainWindowDefaultWidth() const  =0;

    /**
     * Must change the default width of the project main window
     */
    virtual void SetMainWindowDefaultHeight(unsigned int width) =0;

    /**
     * Must return the default width of the project main window
     */
    virtual unsigned int GetMainWindowDefaultHeight() const  =0;

    /**
     * Must change the maximum number of frames allowed to be rendered per seconds
     */
    virtual void SetMaximumFPS(int maxFPS) =0;

    /**
     * Must return the maximum number of frames allowed to be rendered per seconds
     */
    virtual int GetMaximumFPS() const  =0;

    /**
     * Must change the minimum number of frames allowed to be rendered per seconds
     */
    virtual void SetMinimumFPS(unsigned int minFPS) =0;

    /**
     * Must return the minimum number of frames allowed to be rendered per seconds
     */
    virtual unsigned int GetMinimumFPS() const =0;

    /**
     * Must return true if vertical synchronization is activated by default when starting the game
     */
    virtual bool IsVerticalSynchronizationEnabledByDefault() const =0;

    /**
     * Must change the value of the vertical synchronization activation
     */
    virtual void SetVerticalSyncActivatedByDefault(bool enable) =0;


    /**
     * Must return a reference to the vector containing the names of extensions used by the project.
     *
     * \note Default implementation: Return a reference to an empty vector
     */
    virtual const std::vector < std::string > & GetUsedPlatformExtensions() const { return noPlatformExtensionsUsed; };

    /**
     * Must return a reference to the vector containing the names of extensions used by the project.
     *
     * \note Default implementation: Return a reference to an empty vector
     */
    virtual std::vector < std::string > & GetUsedPlatformExtensions() { return noPlatformExtensionsUsed; };

    /**
     * Must return a reference to the platform the project is based on.
     */
    virtual Platform & GetPlatform() const =0;

    ///@}

    /** \name Layouts management
     * Members functions related to layout management.
     */
    ///@{

    /**
     * Must return true if layout called "name" exists.
     */
    virtual bool HasLayoutNamed(const std::string & name) const =0;

    /**
     * Must return a reference to the layout called "name".
     */
    virtual Layout & GetLayout(const std::string & name) =0;

    /**
     * Must return a reference to the layout called "name".
     */
    virtual const Layout & GetLayout(const std::string & name) const =0;

    /**
     * Must return a reference to the layout at position "index" in the layout list
     */
    virtual Layout & GetLayout(unsigned int index) =0;

    /**
     * Must return a reference to the layout at position "index" in the layout list
     */
    virtual const Layout & GetLayout (unsigned int index) const =0;

    /**
     * Must return the position of the layout called "name" in the layout list
     */
    virtual unsigned int GetLayoutPosition(const std::string & name) const =0;

    /**
     * Must return the number of layouts.
     */
    virtual unsigned int GetLayoutCount() const =0;

    /**
     * Must add a new empty layout called "name" at the specified position in the layout list.
     */
    virtual void InsertNewLayout(std::string & name, unsigned int position) =0;

    /**
     * Must add a new layout constructed from the layout passed as parameter.
     * \note No pointer or reference must be kept on the layout passed as parameter.
     * \param layout The layout that must be copied and inserted into the project
     * \param position Insertion position. Even if the position is invalid, the layout must be inserted at the end of the layout list.
     */
    virtual void InsertLayout(const Layout & layout, unsigned int position) =0;

    /**
     * Must delete layout named "name".
     */
    virtual void RemoveLayout(const std::string & name) =0;

    ///@}

    /** \name External events management
     * Members functions related to external events management.
     */
    ///@{

    /**
     * Must return true if external events called "name" exists.
     */
    virtual bool HasExternalEventsNamed(const std::string & name) const =0;

    /**
     * Must return a reference to the external events called "name".
     */
    virtual ExternalEvents & GetExternalEvents(const std::string & name) =0;

    /**
     * Must return a reference to the external events called "name".
     */
    virtual const ExternalEvents & GetExternalEvents(const std::string & name) const =0;

    /**
     * Must return a reference to the external events at position "index" in the external events list
     */
    virtual ExternalEvents & GetExternalEvents(unsigned int index) =0;

    /**
     * Must return a reference to the external events at position "index" in the external events list
     */
    virtual const ExternalEvents & GetExternalEvents (unsigned int index) const =0;

    /**
     * Must return the position of the external events called "name" in the external events list
     */
    virtual unsigned int GetExternalEventsPosition(const std::string & name) const =0;

    /**
     * Must return the number of external events.
     */
    virtual unsigned int GetExternalEventsCount() const =0;

    /**
     * Must add a new empty external events sheet called "name" at the specified position in the layout list.
     */
    virtual void InsertNewExternalEvents(std::string & name, unsigned int position) =0;

    /**
     * Must add a new external events sheet constructed from the layout passed as parameter.
     * \note No pointer or reference must be kept on the external events passed as parameter.
     * \param externalEvents The external events that must be copied and inserted into the project
     * \param position Insertion position. Even if the position is invalid, the external events must be inserted at the end of the external events list.
     */
    virtual void InsertExternalEvents(const ExternalEvents & externalEvents, unsigned int position) =0;

    /**
     * Must delete external events named "name".
     */
    virtual void RemoveExternalEvents(const std::string & name) =0;

    ///@}

    /** \name Global objects management
     * Members functions related to global objects management.
     */
    ///@{

    /**
     * Must return true if global object called "name" exists.
     */
    virtual bool HasObjectNamed(const std::string & name) const =0;

    /**
     * Must return a reference to the global object called "name".
     */
    virtual Object & GetObject(const std::string & name) =0;

    /**
     * Must return a reference to the global object called "name".
     */
    virtual const Object & GetObject(const std::string & name) const =0;

    /**
     * Must return a reference to the global object at position "index" in the global objects list
     */
    virtual Object & GetObject(unsigned int index) =0;

    /**
     * Must return a reference to the global object at position "index" in the global objects list
     */
    virtual const Object & GetObject (unsigned int index) const =0;

    /**
     * Must return the position of the global object called "name" in the global objects list
     */
    virtual unsigned int GetObjectPosition(const std::string & name) const =0;

    /**
     * Must return the number of global object.
     */
    virtual unsigned int GetObjectsCount() const =0;

    /**
     * Must add a new empty global object sheet called "name" at the specified position in the layout list.
     */
    virtual void InsertNewObject(std::string & name, unsigned int position) =0;

    /**
     * Must add a new global object constructed from the layout passed as parameter.
     * \note No pointer or reference must be kept on the global object passed as parameter.
     * \param globalObject The global object that must be copied and inserted into the project
     * \param position Insertion position. Even if the position is invalid, the global object must be inserted at the end of the global objects list.
     */
    virtual void InsertObject(const Object & globalObject, unsigned int position) =0;

    /**
     * Must delete the global object named "name".
     */
    virtual void RemoveObject(const std::string & name) =0;

    /**
     * Return a reference to the vector containing the project's objects groups.
     */
    std::vector <ObjectGroup> & GetObjectGroups() { return objectGroups; }

    /**
     * Return a const reference to the vector containing the project's objects groups.
     */
    const std::vector <ObjectGroup> & GetObjectGroups() const { return objectGroups; }

    ///@}


    /** \name Variable management
     * Members functions related to global variables management.
     */
    ///@{

    /**
     * Must return a reference to the container storing the global variables
     * \see gd::VariablesContainer
     */
    virtual const gd::VariablesContainer & GetVariables() const =0;

    /**
     * Must return a reference to the container storing the global variables
     * \see gd::VariablesContainer
     */
    virtual gd::VariablesContainer & GetVariables() =0;
    ///@}

private:

    std::vector<ObjectGroup> objectGroups; ///< Global objects groups
    static std::vector < std::string > noPlatformExtensionsUsed;
};

}


#endif // GDCORE_PROJECT_H
