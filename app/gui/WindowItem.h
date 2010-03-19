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
//! \file "WindowItem.h"
//! \brief Header file for WindowItem class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       27.03.2009 (last updated)
//!

#ifndef WINDOWITEM_H
#define WINDOWITEM_H

#include "FrapperPrerequisites.h"
#include <QtGui/QStandardItem>
#include "Window.h"


//!
//! Class for standard items that represent one of the application's windows.
//!
//! <div align="center"><b>Inheritance Diagram</b></div>
//!
//! \dot
//!   digraph WindowItem {
//!     node [fontname="FreeSans",fontsize="10",shape="box",height=0.2,width=0.4,fontname="FreeSans",color="black",style="filled",fillcolor="white"];
//!     edge [fontname="FreeSans",fontsize="10",labelfontname="FreeSans",labelfontsize="10",dir=back,style="solid",color="midnightblue"];
//!
//!     WindowItem [label="WindowItem",fillcolor="grey75"];
//!     QStandardItem -> WindowItem;
//!     QStandardItem [label="QStandardItem",fontcolor="white",fillcolor="#66b036",URL="http://doc.qtsoftware.com/4.5/qstandarditem.html"];
//!   }
//! \enddot
//! <div><center>[<a href="graph_legend.html">legend</a>]</center></div>
//!
class FRAPPER_GUI_EXPORT WindowItem : public QStandardItem
{

public: // constructors and destructors

    //!
    //! Constructor of the WindowItem class.
    //!
    //! \param window The window that the item represents.
    //!
    WindowItem ( Window *window );

public: // functions

    //!
    //! Returns the window that the item represents.
    //!
    //! \return The window that the item represents.
    //!
    Window * getWindow ();

private: // data

    //!
    //! The window that the item represents.
    //!
    Window *m_window;

};


#endif
