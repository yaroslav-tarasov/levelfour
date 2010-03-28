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
//! \file "TableViewPanel.cpp"
//! \brief Implementation file for TableViewPanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Sebastian Bach <sebastian.bach@filmakademie.de>
//! \version    1.0
//! \date       07.12.2009 (last updated)
//!

#include "TableViewPanel.h"
#include <QMenu>
#include <QLineEdit>
#include <QMenuBar>

///
/// Constructors and Destructors
///

//!
//! Constructor of the TableViewPanel class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
TableViewPanel::TableViewPanel ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    ViewPanel(ViewPanel::T_PluginPanel, parent, flags)
{

	setupUi(this);

	// Temporary for viewing purposes
	tableView->setRowCount(20);
	tableView->setColumnCount(8);

	m_description = false;
}


//!
//! Destructor of the NetworkPanel class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
TableViewPanel::~TableViewPanel ()
{

}

///
/// Public Funcitons
///
//!
//! Fills the given tool bars with actions for the TableViewPanel view.
//!
//! \param mainToolBar The main tool bar to fill with actions.
//! \param panelToolBar The panel tool bar to fill with actions.
//!
void TableViewPanel::fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar )
{
	// Temp
	QAction *ui_descriptionAction;
	ui_descriptionAction = new QAction(this);
    ui_descriptionAction->setObjectName(QString::fromUtf8("ui_descriptionAction"));
    ui_descriptionAction->setCheckable(true);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/infoIcon"), QSize(), QIcon::Normal, QIcon::Off);
    ui_descriptionAction->setIcon(icon1);
	ui_descriptionAction->setToolTip("Show Description");

	connect(ui_descriptionAction, SIGNAL(toggled(bool)), this, SLOT(showDiscription(bool)));

	// Column visibility select control
	// Place holder for drop down to select which columns of table are visible
	// This should be populated with column names
	QAction *visibilityAct = new QAction(tr("&Select Columns..."), this);
	visibilityAct->setStatusTip(tr("Select columns for visibility..."));
    connect(visibilityAct, SIGNAL(triggered()), this, SLOT());

	QMenu *visibilityMenu = new QMenu();
	visibilityMenu->addAction(visibilityAct);
    visibilityMenu->addSeparator();

	QString menuStyleSheet (
        "QMenuBar {"
        "    border: none;"
        "    margin: 0px;"
        "    padding: 0px;"
		"    background: none;"
        "}"
        "QMenuBar::item {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #a5a5a5, stop:0.2 #999999, stop:0.8 #b0b0b0, stop:1 #555555);"
        "    color: #000000;"
		"    border: 1px solid #323232"
        "    border-radius: 5px;"
        "}"
        "QMenuBar::item:selected {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #a6a6a6, stop:1 #8a8a8a);"
        "    color: #000000;"
		"    border: 1px solid #222222"
        "    border-radius: 5px;"
        "}"
        "QMenuBar::item:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #a6a6a6, stop:1 #8a8a8a);"
        "    color: #000000;"
		"    border: 1px solid #222222"
        "    border-radius: 5px;"
        "}"
    );

	QMenuBar *visibilityMenuBar = new QMenuBar();
	visibilityMenuBar->setMinimumWidth(50);
	visibilityMenuBar->setStyleSheet(menuStyleSheet);
	visibilityMenuBar->addMenu(visibilityMenu);
	visibilityMenuBar->addAction(visibilityAct);

	// Search control
	// Keyword search highlights rows

	QLineEdit *searchField = new QLineEdit();
	searchField->setMaximumWidth(200);

	mainToolBar->addAction(ui_descriptionAction);
	mainToolBar->addSeparator();
	mainToolBar->addWidget(visibilityMenuBar);
	mainToolBar->addSeparator();
	mainToolBar->addWidget(searchField);
}

///
/// Private Slots
///

//! Returns the tree view that is used to display the scene objects.
//!
//! \return The tree view that is used to display the scene objects.
//!
QTableWidget * TableViewPanel::getTableViewPanel ()
{
    return tableView;
}

//!
//! Sets the description mode
//!
//! \param description boolean value of the action´s state
//!
void TableViewPanel::showDescription(bool description)
{
	m_description = description;
}

