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
//! \file "FilenameParameter.cpp"
//! \brief Implementation file for FilenameParameter class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       03.05.2009 (last updated)
//!

#include "FilenameParameter.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the FilenameParameter class.
//!
//! \param name The name of the parameter.
//! \param value The parameter's value.
//!
FilenameParameter::FilenameParameter ( const QString &name, const QVariant &value ) :
    Parameter(name, Parameter::T_Filename, value),
    m_filenameType(FT_Open)
{
}


//!
//! Copy constructor of the FilenameParameter class.
//!
//! \param parameter The parameter to copy.
//!
FilenameParameter::FilenameParameter ( const FilenameParameter &parameter ) :
    Parameter(parameter),
    m_filters(parameter.m_filters),
    m_filenameType(parameter.m_filenameType)
{
}


//!
//! Destructor of the FilenameParameter class.
//!
FilenameParameter::~FilenameParameter ()
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
AbstractParameter * FilenameParameter::clone ()
{
    return new FilenameParameter(*this);
}


//!
//! Returns the filename filters to use when selecting a file in a file
//! browser dialog.
//!
//! \return The filters to use when selecting a file in a file dialog.
//!
QString FilenameParameter::getFilters () const
{
    return m_filters;
}


//!
//! Sets the filename filters to use when selecting a file in a file
//! browser dialog.
//!
//! \param filters The filters to use when selecting a file in a file dialog.
//!
void FilenameParameter::setFilters ( const QString &filters )
{
    m_filters = filters;
}


//!
//! Returns the type for the filename.
//!
//! \return The type for the filename.
//!
FilenameParameter::Type FilenameParameter::getType () const
{
    return m_filenameType;
}


//!
//! Sets the type for the filename.
//!
//! \param filenameType The type for the filename.
//!
void FilenameParameter::setType ( FilenameParameter::Type filenameType )
{
    m_filenameType = filenameType;
}
