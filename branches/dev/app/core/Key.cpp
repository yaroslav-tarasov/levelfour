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
//! \file "Key.h"
//! \brief Implementation file for Key class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Simon Spielmann <simon.spielmann@filmakademie.de>
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       11.04.2009 (last updated)
//!

#include "Key.h"
#include "NumberParameter.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the Key class.
//!
//! \param index The index for the new key.
//! \param value The value for the new key.
//! \param numberParameter The parent parameter for the new key.
//!
Key::Key ( const float &index, const QVariant &value, NumberParameter *numberParameter ):
    m_index(index),
    m_value(value),
	m_numberParameter(numberParameter)
{
}

//!
//! Standard constructor of the Key class with inex = value = 0.0.
//!
Key::Key () :
    m_index(0.0),
    m_value(0.0),
	m_numberParameter(0)
{
}

//!
//! Copy constructor of the Key class.
//!
//! \param parameter The parameter to copy.
//!
Key::Key (const Key &key) :
	m_index(key.m_index),
	m_value(key.m_value)
{
}

//!
//! Destructor of the Key class.
//!
Key::~Key ()
{
    // noop
}


///
/// Public Functions
///


//!
//! Returns the key's index.
//!
//! \return The key's index.
//!
float Key::getIndex () const
{
    return m_index;
}


//!
//! Sets the index of the key to the given index.
//!
//! \param index The index for the key.
//!
void Key::setIndex (const float &index )
{
    m_index = index;
	if (!m_numberParameter->isDirty())
		m_numberParameter->propagateDirty(0);
}


//!
//! Returns the key's value.
//!
//! \return The key's value.
//!
QVariant Key::getValue () const
{
    return m_value;
}


//!
//! Sets the value of the key to the given value.
//!
//! \param value The value for the key.
//!
void Key::setValue (const QVariant &value )
{
    m_value = value;
	//if (!m_numberParameter->isDirty()) {
	//m_numberParameter->setDirty(true);
	m_numberParameter->propagateDirty(0);
	//}
}


//!
//! Sets the parent parameter of the key.
//!
//! \param parameter The parent parameter for the key.
//!
void Key::setParameter ( NumberParameter* parameter )
{
	m_numberParameter = parameter;
}
