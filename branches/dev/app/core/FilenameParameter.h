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
//! \file "FilenameParameter.h"
//! \brief Header file for FilenameParameter class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       03.05.2009 (last updated)
//!

#ifndef FILENAMEPARAMETER_H
#define FILENAMEPARAMETER_H

#include "FrapperPrerequisites.h"
#include "Parameter.h"


//!
//! Class representing parameters for filenames.
//!
class FRAPPER_CORE_EXPORT FilenameParameter : public Parameter
{

public: // nested enumeration

    //!
    //! Nested enumeration of different filename types.
    //!
    enum Type {
        FT_Open,
        FT_Save
    };

public: // constructors and destructors

    //!
    //! Constructor of the FilenameParameter class.
    //!
    //! \param name The name of the parameter.
    //! \param value The parameter's value.
    //!
    FilenameParameter ( const QString &name, const QVariant &value );

    //!
    //! Copy constructor of the FilenameParameter class.
    //!
    //! \param parameter The parameter to copy.
    //!
    FilenameParameter ( const FilenameParameter &parameter );

    //!
    //! Destructor of the FilenameParameter class.
    //!
    virtual ~FilenameParameter ();

public: // functions

    //!
    //! Creates an exact copy of the parameter.
    //!
    //! \return An exact copy of the parameter.
    //!
    virtual AbstractParameter * clone ();

    //!
    //! Returns the filename filters to use when selecting a file in a file
    //! browser dialog.
    //!
    //! \return The filters to use when selecting a file in a file dialog.
    //!
    QString getFilters () const;

    //!
    //! Sets the filename filters to use when selecting a file in a file
    //! browser dialog.
    //!
    //! \param filters The filters to use when selecting a file in a file dialog.
    //!
    void setFilters ( const QString &filters );

    //!
    //! Returns the type for the filename.
    //!
    //! \return The type for the filename.
    //!
    Type getType () const;

    //!
    //! Sets the type for the filename.
    //!
    //! \param filenameType The type for the filename.
    //!
    void setType ( Type filenameType );

private: // data

    //!
    //! The filename filters to use when selecting a file in a file browser
    //! dialog.
    //!
    QString m_filters;

    //!
    //! The type of the filename.
    //!
    Type m_filenameType;

};


#endif
