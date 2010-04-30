/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2010 Filmakademie Baden-Wuerttemberg, Institute of Animation 

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
//! \file "GenericParameter.cpp"
//! \brief Implementation file for GenericParameter class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       16.04.2010 (last updated)
//!

#include "GenericParameter.h"
#include "Node.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the FilenameParameter class.
//!
//! \param name The name of the parameter.
//! \param type The type of the parameter's value(s).
//! \param value The parameter's value.
//! \param animated The parameter's animation status.
//!
GenericParameter::GenericParameter ( const QString &name, const QVariant &value) :
Parameter(name, Parameter::Type::T_Generic, value),
	m_typeInfo("NO TYPE")
{
}


//!
//! Copy constructor of the GenericParameter class.
//!
//! \param parameter The parameter to copy.
//! \param node The parent node.
//!
GenericParameter::GenericParameter ( const GenericParameter &parameter, Node* node /*= 0*/ ) :
    Parameter(parameter)
{
	m_node = node;
	if (node)
		m_node->getTimeParameter()->addAffectedParameter(this);
}


//!
//! Destructor of the FilenameParameter class.
//!
GenericParameter::~GenericParameter ()
{
}


///
/// Public Functions
///


//!
//! Creates an exact copy of the parameter.
//!
//! \return An exact copy of the parameter.
//!
AbstractParameter * GenericParameter::clone ()
{
    return new GenericParameter(*this);
}


//!
//! Sets whether the parameter's value has changed.
//!
//! \param dirty The new value for the parameter's dirty flag.
//!
void GenericParameter::setDirty (const bool dirty )
{
    Parameter::setDirty(dirty);
}

//!
//! Sets whether the controls for editing the parameter or parameter group
//! in the UI should be enabled.
//!
//! \param enabled The value for the parameter's enabled flag.
//!
void GenericParameter::setEnabled (const bool enabled )
{
    m_enabled = enabled;
	if (!enabled) setValue(0.0);
	emit valueChanged();
}

//!
//! Returns the parameter's value while optionally triggering the
//! evaluation chain.
//!
//! \param Trigger evaluation.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The parameter's value.
//!
QVariant GenericParameter::getValue ( bool triggerEvaluation /* = false */ )
{
    // optionally trigger the evaluation chain
    if (triggerEvaluation) {
        Parameter::PinType pinType = getPinType();
        if (pinType == Parameter::PT_Output || pinType == Parameter::PT_Input)
            propagateEvaluation(0);
    }

    return m_value;
}

//!
//! Returns the name of the given parameter type.
//!
//! \param type The type for which to return the name.
//! \return The name of the given parameter type.
//!
QString GenericParameter::getTypeName ( Parameter::Type type ) const
{
	return QString("Generic: " + m_typeInfo);
}

//!
//! Convenience function for setting the type info
//!
//! \param typeInfo The type info to set for the parameter.
//!
void GenericParameter::setTypeInfo (const QString &typeInfo) 
{
	m_typeInfo = typeInfo;
}

//!
//! Returns the parameter's type info string.
//!
//!
//! \return The parameter's type info string
//!
QString GenericParameter::getTypeInfo ( ) const
{
	return m_typeInfo;
}

//!
//! Convenience function for setting the value of an unsigned integer
//! parameter while optionally triggering the dirtying chain.
//!
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void GenericParameter::setValue ( QVariant value, bool triggerDirtying /*= false*/ )
{
	Parameter::setValue(value);
	// optionally trigger the dirtying chain
    if (triggerDirtying)
		propagateDirty(0);
}