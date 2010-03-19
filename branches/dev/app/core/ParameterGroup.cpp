/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "ParameterGroup.cpp"
//! \brief Implementation file for ParameterGroup class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       05.08.2009 (last updated)
//!

#include "ParameterGroup.h"
#include "Node.h"
#include "Log.h"

INIT_INSTANCE_COUNTER(ParameterGroup)


///
/// Constructors and Destructors
///


//!
//! Constructor of the ParameterGroup class.
//!
//! \param name The name of the parameter group.
//!
ParameterGroup::ParameterGroup ( const QString &name /* = "" */ ) :
    AbstractParameter(name)
{
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ParameterGroup class.
//!
ParameterGroup::~ParameterGroup ()
{
	foreach (AbstractParameter *parameter, m_parameterList) {
		delete parameter;
		parameter = 0;
	}
  
    clear();

    DEC_INSTANCE_COUNTER
}


///
/// Public Functions
///


//!
//! Sets whether the controls for editing the parameter group in the UI
//! should be enabled.
//!
//! Recursively sets the enabled state for all parameters and parameter
//! groups contained in this group.
//!
//! \param enabled The value for the parameters' enabled flags.
//!
void ParameterGroup::setEnabled ( bool enabled )
{
    AbstractParameter::setEnabled(enabled);

    for (int i = 0; i < m_parameterList.size(); ++i)
        m_parameterList[i]->setEnabled(enabled);
}


//!
//! Sets the node to which this parameter group belongs.
//!
//! Recursively sets the node for all parameters and parameter groups
//! contained in this group.
//!
//! \param node The node to which the parameter group belongs.
//!
void ParameterGroup::setNode ( Node *node )
{
    AbstractParameter::setNode(node);

    for (int i = 0; i < m_parameterList.size(); ++i)
        m_parameterList[i]->setNode(node);
}


//!
//! Creates an exact copy of the parameter.
//!
//! \return An exact copy of the parameter.
//!
AbstractParameter * ParameterGroup::clone ()
{
    ParameterGroup *parameterGroup = new ParameterGroup(m_name);
    // iterate over the list of parameters and parameter groups
    for (int i = 0; i < m_parameterList.size(); ++i)
        // add a copy of the current parameter object to the new group
        parameterGroup->addParameter(m_parameterList[i]->clone());
    return parameterGroup;
}


//!
//! Returns whether the parameter object derived from this class is a
//! parameter group.
//!
//! \return True if the parameter object is a parameter group, otherwise False.
//!
bool ParameterGroup::isGroup () const
{
    return true;
}


//!
//! Resets the parameters contained in the group to their default values.
//!
void ParameterGroup::reset ()
{
    for (int i = 0; i < m_parameterList.size(); ++i)
        m_parameterList[i]->reset();
}


//!
//! Disconnects all signals from parameters contained in the group from
//! slots of the given receiver object.
//!
//! \param receiver The object to disconnect the contained parameters from.
//!
void ParameterGroup::disconnectParameters ( QObject *receiver )
{
    // iterate over all parameter groups and parameters contained in this group
    for (int i = 0; i < m_parameterList.size(); ++i)
        if (m_parameterList[i]->isGroup()) {
            // recursively disconnect parameters contained in the nested parameter group
            ParameterGroup *parameterGroup = dynamic_cast<ParameterGroup *>(m_parameterList[i]);
            if (parameterGroup)
                parameterGroup->disconnectParameters(receiver);
        } else {
            Parameter *parameter = dynamic_cast<Parameter *>(m_parameterList[i]);
            if (parameter)
                disconnect(parameter, 0, receiver, 0);
        }
}


//!
//! Clears the parameter group by emptying the parameter data structures.
//!
void ParameterGroup::clear ()
{
    m_parameterList.clear();
    m_parameterMap.clear();
}

//!
//! Destroys all parameters in the group.
//!
void ParameterGroup::destroyAllParameters ()
{
	foreach (AbstractParameter *parameter, m_parameterList) {
        delete parameter;
		parameter = 0;
	}
    clear();
}


//!
//! Returns a list of parameters whose names contain the given search
//! text.
//!
//! \param searchText The text to match the parameter names against.
//! \param diveInChildGroups Flag to control whether to also process child groups.
//! \return A list of parameters whose names contain the given search text.
//!
QList<Parameter *> ParameterGroup::filterParameters ( QString searchText, bool diveInChildGroups, bool getAll /* = false */ ) const
{
    QList<Parameter *> parameterList;
    if (getAll || searchText != "")
        for (int i = 0; i < m_parameterList.size(); ++i) {
            if (m_parameterList[i]->isGroup() && diveInChildGroups) {
                // recursively search for the parameter in the current group
                ParameterGroup *parameterGroup = dynamic_cast<ParameterGroup *>(m_parameterList[i]);
                if (parameterGroup)
                    parameterList += parameterGroup->filterParameters(searchText, true, getAll);
            } else {
                Parameter *parameter = dynamic_cast<Parameter *>(m_parameterList[i]);
				if (parameter && (getAll || parameter->getName().toLower().contains(searchText)))
					if (!parameterList.contains(parameter))
						parameterList.append(parameter);
            }
        }
    return parameterList;
}


//!
//! Returns whether the group contains a parameter with the given name.
//!
//! \param name The parameter name to find in the group.
//! \return True if the group contains a parameter with the given name, otherwise False.
//!
bool ParameterGroup::contains ( const QString &name ) const
{
    bool result = false;
    int i = 0;
    while (!result && i < m_parameterList.size()) {
        if (m_parameterList[i]->isGroup())
            // recursively search for the parameter in the current group
            result = dynamic_cast<ParameterGroup *>(m_parameterList[i])->contains(name);
        else
            result = dynamic_cast<Parameter *>(m_parameterList[i])->getName() == name;
        ++i;
    }
    return result;
}

//!
//! Returns whether the group contains a group with the given name.
//!
//! \param name The group name to find in the group.
//! \return True if the group contains a group with the given name, otherwise False.
//!
bool ParameterGroup::containsGroup ( const QString &name ) const
{
    bool result = false;
    int  i= 0;
    AbstractParameter::Map::iterator iter = m_parameterMap.find(name);

    if (iter != m_parameterMap.end()) {
        if (iter.value()->isGroup()) {
            if (name == dynamic_cast<ParameterGroup *>(iter.value())->getName())
                result = true;
            else
                // recursively search for the parameter in the current group
                result = dynamic_cast<ParameterGroup *>(iter.value())->containsGroup(name);
        }
        else
            result = dynamic_cast<Parameter *>(iter.value())->getName() == name;
    }
    return result;
}


//!
//! Returns whether the group contains the given parameter.
//!
//! \param parameter The parameter to find in the group.
//! \return True if the group contains the given parameter, otherwise False.
//!
bool ParameterGroup::contains ( Parameter *parameter ) const
{
    bool result = false;
    int i = 0;
    while (!result && i < m_parameterList.size()) {
        if (m_parameterList[i]->isGroup())
            // recursively search for the parameter in the current group
            result = dynamic_cast<ParameterGroup *>(m_parameterList[i])->contains(parameter);
        else
            result = dynamic_cast<Parameter *>(m_parameterList[i]) == parameter;
        ++i;
    }
    return result;
}


//!
//! Returns whether the group contains parameters of the given type.
//!
//! \param parameterType The parameter type to find in the group.
//! \return True if the group contains parameters of the given type, otherwise False.
//!
bool ParameterGroup::contains ( Parameter::Type parameterType ) const
{
    bool result = false;
    int i = 0;
    while (!result && i < m_parameterList.size()) {
        if (m_parameterList[i]->isGroup())
            // recursively search for the parameter in the current group
            result = dynamic_cast<ParameterGroup *>(m_parameterList[i])->contains(parameterType);
        else
            result = dynamic_cast<Parameter *>(m_parameterList[i])->getType() == parameterType;
        ++i;
    }
    return result;
}


//!
//! Returns whether the group contains parameters of the given pin type.
//!
//! \param pinType The pin type of parameters to look for in the group.
//! \return True if the group contains parameters of the given pin type, otherwise False.
//!
bool ParameterGroup::contains ( Parameter::PinType pinType ) const
{
    bool result = false;
    int i = 0;
    while (!result && i < m_parameterList.size()) {
        if (m_parameterList[i]->isGroup())
            // recursively search for parameters in the current group
            result = dynamic_cast<ParameterGroup *>(m_parameterList[i])->contains(pinType);
        else
            result = dynamic_cast<Parameter *>(m_parameterList[i])->getPinType() == pinType;
        ++i;
    }
    return result;
}


//!
//! Returns whether the group contains parameters of the given pin type and
//! of the given connection status.
//!
//! \param pinType The pin type of parameters to look for in the group.
//! \param connected The connection status of parameters to look for in the group.
//! \return True if the group contains parameters of the given pin type and of the given connection status, otherwise False.
//!
bool ParameterGroup::contains ( Parameter::PinType pinType, bool connected ) const
{
    bool result = false;
    int i = 0;
    while (!result && i < m_parameterList.size()) {
        if (m_parameterList[i]->isGroup())
            // recursively search for parameters in the current group
            result = dynamic_cast<ParameterGroup *>(m_parameterList[i])->contains(pinType, connected);
        else {
            Parameter *parameter = dynamic_cast<Parameter *>(m_parameterList[i]);
            result = parameter->getPinType() == pinType && parameter->isConnected() == connected;
        }
        ++i;
    }
    return result;
}


//!
//! Returns whether the group contains parameters or not
//!
//!
bool ParameterGroup::isEmpty ( ) const
{
	return m_parameterList.isEmpty() && m_parameterMap.isEmpty();
}


//!
//! Adds the given parameter to the list of parameters.
//!
//! \param parameter The parameter to add.
//! \param prepend Flag to control whether to insert the parameter at the beginning of the parameter list.
//!
void ParameterGroup::addParameter ( AbstractParameter *parameter, bool prepend /* = false */ )
{
    if (m_parameterMap.contains(parameter->getName())) {
        Log::error(QString("A parameter \"%1.%2\" already exists.").arg(m_node->getName(), parameter->getName()), "ParameterGroup::addParameter");
        return;
    }

    if (prepend)
        m_parameterList.prepend(parameter);
    else
        m_parameterList.append(parameter);

    m_parameterMap.insert(parameter->getName() , parameter);
    parameter->setNode(m_node);

    if (!m_enabled)
        parameter->setEnabled(false);
}


//!
//! Returns the parameter with the given name.
//!
//! \param name The name of the parameter to return.
//! \return The parameter with the given name.
//!
Parameter * ParameterGroup::getParameter ( const QString &name ) const
{
    Parameter *result = 0;

    QString parameterName = name;
    QString parentParameterGroupName = Parameter::extractFirstGroupName(&parameterName);

    if (parentParameterGroupName.isEmpty())
        if (m_parameterMap.contains(parameterName)) {
            AbstractParameter *abstractParameter = m_parameterMap[parameterName];
            if (!abstractParameter->isGroup())
                result = dynamic_cast<Parameter *>(abstractParameter);
        } else {
            // iterate over the parameters and recursively search for the parameter name in nested parameter groups
            AbstractParameter::Map::iterator iter = m_parameterMap.begin();
            while (iter != m_parameterMap.end() && !result) {
                if (iter.value()->isGroup())
                    result = dynamic_cast<ParameterGroup *>(iter.value())->getParameter(parameterName);
                ++iter;
            }
        }
    else
        if (m_parameterMap.contains(parentParameterGroupName)) {
            AbstractParameter *parentAbstractParameter = m_parameterMap[parentParameterGroupName];
            if (parentAbstractParameter->isGroup()) {
                ParameterGroup *parentParameterGroup = dynamic_cast<ParameterGroup *>(parentAbstractParameter);
                result = parentParameterGroup->getParameter(parameterName);
            }
        }

 //   AbstractParameter::Map::iterator iter = m_parameterMap.find(firstGroupName);

    //if (iter != m_parameterMap.end()) {
    //    if (iter.value()->isGroup()) {
    //        if (firstGroupName == "" || iter.value()->getName() == firstGroupName)
    //            // recursively search for the parameter in the current group
    //            result = dynamic_cast<ParameterGroup *>(iter.value())->getParameter(parameterName);
    //    }
    //    else if (iter.value()->getName() == parameterName)
    //        result = dynamic_cast<Parameter *>(iter.value());
    //}

    return result;
}


//!
//! Removes and deletes the parameter with the given name from the
//! parameter group.
//!
//! \param name The name of the parameter to remove and delete.
//! \return 0.
//!
Parameter * ParameterGroup::removeParameter ( const QString &name, bool diveInGroups /*= true*/ )
{
    Parameter *parameter = getParameter(name);
    if (!parameter) {
        Log::warning(QString("Parameter \"%1.%2\" could not be found.").arg(m_node->getName(), name), "ParameterGroup::removeParameter");
        return 0;
    }

    return removeParameter(parameter, diveInGroups);
}


//!
//! Removes and deletes the given parameter from the parameter group.
//!
//! \param parameter The parameter to remove and delete.
//! \return 0.
//!
Parameter * ParameterGroup::removeParameter ( Parameter *parameter, bool diveInGroups /*= true*/ )
{
    unsigned int i = 0;
    while (parameter && i < m_parameterList.size()) {
        if (m_parameterList[i]->isGroup() && diveInGroups)
            // recursively remove the parameter in child groups
            parameter = dynamic_cast<ParameterGroup *>(m_parameterList[i])->removeParameter(parameter);
        ++i;
    }

    // check if the parameter has not been removed in a child group and is contained in this group
    if (parameter && m_parameterMap.contains(parameter->getName())) {
        m_parameterList.removeAll(parameter);
		m_parameterMap.remove(parameter->getName());

        delete parameter;
        parameter = 0;
    }

    return parameter;
}


//!
//! Removes and deletes the given parameter group from the parameter group.
//!
//! \param parameter The parameter group to remove and delete.
//!
void ParameterGroup::removeParameterGroup ( const QString &name )
{
    ParameterGroup *parameterGroup = getParameterGroup(name, false);

    m_parameterList.removeAll(parameterGroup);
    m_parameterMap.remove(name);

    delete parameterGroup;
}


//!
//! Returns the parameter group with the given name.
//!
//! \param name The name of the parameter group to return.
//! \return The parameter group with the given name.
//!
ParameterGroup * ParameterGroup::getParameterGroup ( const QString &name, bool recursive /*= true*/ ) const
{
    ParameterGroup *result = 0;
    AbstractParameter::Map::iterator iter = m_parameterMap.find(name);

    if (iter != m_parameterMap.end()){

        if (iter.value()->isGroup()) {
            ParameterGroup *parameterGroup = dynamic_cast<ParameterGroup *>(iter.value());
            if (iter.value()->getName() == name)
                result = parameterGroup;
            else if (recursive)
                result = parameterGroup->getParameterGroup(name);
        }
    }
    return result;
}


//!
//! Returns a pointer to the list of parameters and parameter groups that
//! are contained in this group.
//!
//! \return A pointer to the list of parameters of this group.
//!
AbstractParameter::List * ParameterGroup::getParameterList ()
{
    return &m_parameterList;
}


//!
//! Returns a pointer to the map of parameters and parameter groups that
//! are contained in this group.
//!
//! \return A pointer to the map of parameters of this group.
//!
AbstractParameter::Map * ParameterGroup::getParameterMap ()
{
    return &m_parameterMap;
}
