/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

//!
//! \file "TableViewPanel.h"
//! \brief Header file for TableViewPanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Sebastian Bach <sebastian.bach@filmakademie.de>
//! \version    1.0
//! \date       24.06.2009 (last updated)
//!

#ifndef TableViewPanel_H
#define TableViewPanel_H

#include "FrapperPrerequisites.h"
#include "ViewPanel.h"
#include "ui_TableViewPanel.h"


//!
//! Class representing a view for displaying and editing parameters of nodes.
//!
class TableViewPanel : public ViewPanel, protected Ui::TableViewPanel
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the TableViewPanel class.
    //!
    //! \param parent The parent widget the created instance will be a child of.
    //! \param flags Extra widget options.
    //!
    TableViewPanel ( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

    //!
    //! Destructor of the TableViewPanel class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~TableViewPanel ();

public slots: //

   

public: // functions

	//!
	//! Fills the given tool bars with actions for the TableViewPanel view.
	//!
	//! \param mainToolBar The main tool bar to fill with actions.
	//! \param panelToolBar The panel tool bar to fill with actions.
	//!
	void fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar );

	//!
    //! Returns the tree view that is used to display the scene objects.
    //!
    //! \return The tree view that is used to display the scene objects.
    //!
    QTableWidget * getTableViewPanel ();

private slots: //

	//!
	//! Sets the description mode
	//!
	void showDescription(bool description);

private: // data

	//!
	//! Bool if detailed description should be used
	//!
	//! \param description boolean value of the action´s state
	//!
	bool m_description;
};


#endif
