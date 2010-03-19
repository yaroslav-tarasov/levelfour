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
//! \brief Header file for Key class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       11.04.2009 (last updated)
//!

#include "FrapperPrerequisites.h"
#include <QtCore/QVariant>

#ifndef KEY_H
#define KEY_H

class NumberParameter;

//!
//! Class representing keyframes in a NumberParameter value animation.
//!
class FRAPPER_CORE_EXPORT Key
{

public: // constructors and destructors

    //!
    //! Constructor of the Key class.
    //!
    //! \param index The index for the new key.
    //! \param value The value for the new key.
	//! \param numberParameter The parent parameter for the new key.
    //!
    Key (const float &index, const QVariant &value, NumberParameter *parameter = 0 );

	//!
    //! Standard constructor of the Key class with inex and value = 0.0.
    //!
    Key ();

	//!
	//! Copy constructor of the Key class.
	//!
	//! \param parameter The parameter to copy.
	//!
	Key (const Key &key);

    //!
    //! Destructor of the Key class.
    //!
    ~Key ();

public: // functions

    //!
    //! Returns the key's index.
    //!
    //! \return The key's index.
    //!
    float getIndex () const;

    //!
    //! Sets the index of the key to the given index.
    //!
    //! \param index The index for the key.
    //!
    void setIndex (const float &index );

    //!
    //! Returns the key's value.
    //!
    //! \return The key's value.
    //!
    QVariant getValue () const;

    //!
    //! Sets the value of the key to the given value.
    //!
    //! \param value The value for the key.
    //!
    void setValue ( const QVariant &value );

	//!
    //! Sets the parent parameter of the key.
    //!
    //! \param parameter The parent parameter for the key.
    //!
    void setParameter ( NumberParameter* parameter );

	/// Operators

	//!
	//! Multiplies the Value of the key with the given scalar
	//!
	//! \param Thie geven scalar
	//!
	inline Key& operator /= ( const float &fScalar )
    {
		register float value;
		if (value = m_value.toFloat())
			m_value = value / fScalar;
        return *this;
    }

private: // data

    //!
    //! The key's index.
    //!
    float m_index;

    //!
    //! The key's value.
    //!
    QVariant m_value;

	//!
    //! The key's parent numberparameter.
    //!
	NumberParameter *m_numberParameter;
};

#endif