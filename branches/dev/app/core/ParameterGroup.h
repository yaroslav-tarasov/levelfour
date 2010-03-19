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
//! \file "ParameterGroup.h"
//! \brief Header file for ParameterGroup class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       05.08.2009 (last updated)
//!

#ifndef PARAMETERGROUP_H
#define PARAMETERGROUP_H

#include "AbstractParameter.h"
#include "Parameter.h"
#include "InstanceCounterMacros.h"


//!
//! Class representing a group of parameters of a Node.
//!
class FRAPPER_CORE_EXPORT ParameterGroup : public AbstractParameter
{

    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the ParameterGroup class.
    //!
    //! \param name The name of the parameter group.
    //!
    ParameterGroup ( const QString &name = "" );

    //!
    //! Destructor of the ParameterGroup class.
    //!
    virtual ~ParameterGroup ();

public: // functions

    //!
    //! Sets whether the controls for editing the parameter group in the UI
    //! should be enabled.
    //!
    //! Recursively sets the enabled state for all parameters and parameter
    //! groups contained in this group.
    //!
    //! \param enabled The value for the parameters' enabled flags.
    //!
    virtual void setEnabled ( bool enabled );

    //!
    //! Sets the node to which this parameter group belongs.
    //!
    //! Recursively sets the node for all parameters and parameter groups
    //! contained in this group.
    //!
    //! \param node The node to which the parameter group belongs.
    //!
    virtual void setNode ( Node *node );

    //!
    //! Creates an exact copy of the parameter.
    //!
    //! \return An exact copy of the parameter.
    //!
    virtual AbstractParameter * clone ();

    //!
    //! Returns whether the parameter object derived from this class is a
    //! parameter group.
    //!
    //! \return True if the parameter object is a parameter group, otherwise False.
    //!
    virtual bool isGroup () const;

    //!
    //! Resets the parameters contained in the group to their default values.
    //!
    virtual void reset ();

    //!
    //! Disconnects all signals from parameters contained in the group from
    //! slots of the given receiver object.
    //!
    //! \param receiver The object to disconnect the contained parameters from.
    //!
    void disconnectParameters ( QObject *receiver );

    //!
    //! Clears the parameter group by emptying the parameter data structures.
    //!
    void clear ();

    //!
    //! Destroys all parameters in the group.
    //!
    void ParameterGroup::destroyAllParameters ();

    //!
    //! Returns a list of parameters whose names contain the given search
    //! text.
    //!
    //! \param searchText The text to match the parameter names against.
    //! \param diveInChildGroups Flag to control whether to also process child groups.
    //! \param getAll Retrieves all parameters.
    //! \return A list of parameters whose names contain the given search text.
    //!
    QList<Parameter *> filterParameters ( QString searchText, bool diveInChildGroups, bool getAll = false ) const;

    //!
    //! Returns whether the group contains a group with the given name.
    //!
    //! \param name The group name to find in the group.
    //! \return True if the group contains a group with the given name, otherwise False.
    //!
    bool containsGroup ( const QString &name ) const;

    //!
    //! Returns whether the group contains a parameter with the given name.
    //!
    //! \param name The parameter name to find in the group.
    //! \return True if the group contains a parameter with the given name, otherwise False.
    //!
    bool contains ( const QString &name ) const;

    //!
    //! Returns whether the group contains the given parameter.
    //!
    //! \param parameter The parameter to find in the group.
    //! \return True if the group contains the given parameter, otherwise False.
    //!
    bool contains ( Parameter *parameter ) const;

    //!
    //! Returns whether the group contains parameters of the given type.
    //!
    //! \param parameterType The parameter type to find in the group.
    //! \return True if the group contains parameters of the given type, otherwise False.
    //!
    bool contains ( Parameter::Type parameterType ) const;

    //!
    //! Returns whether the group contains parameters of the given pin type.
    //!
    //! \param pinType The pin type of parameters to look for in the group.
    //! \return True if the group contains parameters of the given pin type, otherwise False.
    //!
    bool contains ( Parameter::PinType pinType ) const;

    //!
    //! Returns whether the group contains parameters of the given pin type and
    //! of the given connection status.
    //!
    //! \param pinType The pin type of parameters to look for in the group.
    //! \param connected The connection status of parameters to look for in the group.
    //! \return True if the group contains parameters of the given pin type and of the given connection status, otherwise False.
    //!
    bool contains ( Parameter::PinType pinType, bool connected ) const;

	//!
    //! Returns whether the group contains parameters or not
    //!
    //!
    bool isEmpty ( ) const;

    //!
    //! Adds the given parameter to the group of parameters.
    //!
    //! \param parameter The parameter to add.
    //! \param prepend Flag to control whether to insert the parameter at the beginning of the parameter list.
    //!
    void addParameter ( AbstractParameter *parameter, bool prepend = false );

    //!
    //! Returns the parameter with the given name.
    //!
    //! \param name The name of the parameter to return.
    //! \return The parameter with the given name.
    //!
    Parameter * getParameter ( const QString &name ) const;

    //!
    //! Removes and deletes the parameter with the given name from the
    //! parameter group.
    //!
    //! \param name The name of the parameter to remove and delete.
    //! \return 0.
    //!
    Parameter * removeParameter ( const QString &name, bool diveInGroups = true );

    //!
    //! Removes and deletes the given parameter from the parameter group.
    //!
    //! \param parameter The parameter to remove and delete.
    //! \return 0.
    //!
    Parameter * removeParameter ( Parameter *parameter, bool diveInGroups = true );

    //!
    //! Removes and deletes the given parameter group from the parameter group.
    //!
    //! \param parameter The parameter group to remove and delete.
    //!
    void removeParameterGroup (  const QString &name );

    //!
    //! Returns the parameter group with the given name.
    //!
    //! \param name The name of the parameter group to return.
    //! \return The parameter group with the given name.
    //!
    ParameterGroup * getParameterGroup ( const QString &name, bool recursive = true ) const;

    //!
    //! Returns a pointer to the list of parameters and parameter groups that
    //! are contained in this group.
    //!
    //! \return A pointer to the list of parameters of this group.
    //!
    AbstractParameter::List * getParameterList ();

    //!
    //! Returns a pointer to the map of parameters and parameter groups that
    //! are contained in this group.
    //!
    //! \return A pointer to the map of parameters of this group.
    //!
    AbstractParameter::Map * getParameterMap ();


private: // data

    //!
    //! The list of parameters or parameter groups contained in this group.
    //!
    AbstractParameter::List m_parameterList;

    //!
    //! The map of parameters or parameter groups contained in this group.
    //!
    AbstractParameter::Map m_parameterMap;
};


#endif
