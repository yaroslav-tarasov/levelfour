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
//! \file "TreeViewPanel.cpp"
//! \brief Implementation file for TreeViewPanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Sebastian Bach <sebastian.bach@filmakademie.de>
//! \version    1.0
//! \date       07.12.2009 (last updated)
//!

#include "TreeViewPanel.h"
#include <QtGui/QMenu>
#include <QtGui/QStandardItemModel>
#include <QtGui/QItemSelectionModel>

///
/// Constructors and Destructors
///

//!
//! Constructor of the TreeViewPanel class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
TreeViewPanel::TreeViewPanel ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    ViewPanel(ViewPanel::T_PluginPanel, parent, flags)
{

	setupUi(this);

	m_description = false;
}


//!
//! Destructor of the NetworkPanel class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
TreeViewPanel::~TreeViewPanel ()
{

}

///
/// Public Funcitons
///


//!
//! Connects the panel with the scene.
//!
//! \param *nodeModel NodeModel of the scene
//! \param *sceneModel SceneModel of the scene
//!
//!
//! Fills the given tool bars with actions for the TreeViewPanel view.
//!
//! \param mainToolBar The main tool bar to fill with actions.
//! \param panelToolBar The panel tool bar to fill with actions.
//!
void TreeViewPanel::fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar )
{
	QAction *ui_descriptionAction;
	ui_descriptionAction = new QAction(this);
    ui_descriptionAction->setObjectName(QString::fromUtf8("ui_descriptionAction"));
    ui_descriptionAction->setCheckable(true);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/infoIcon"), QSize(), QIcon::Normal, QIcon::Off);
    ui_descriptionAction->setIcon(icon1);
	ui_descriptionAction->setToolTip("Show Description");

	connect(ui_descriptionAction, SIGNAL(toggled(bool)), this, SLOT(showDiscription(bool)));

	mainToolBar->addAction(ui_descriptionAction);
}

//!
//! Returns the tree view that is used to display the scene objects.
//!
//! \return The tree view that is used to display the scene objects.
//!
QTreeView * TreeViewPanel::getTreeViewPanel ()
{
    return treeView;
}

//!
//! Sets the description mode
//!
//! \param description boolean value of the action´s state
//!
void TreeViewPanel::showDiscription(bool description)
{
	m_description = description;
}

