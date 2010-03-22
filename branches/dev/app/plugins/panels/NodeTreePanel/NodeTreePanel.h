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
//! \file "NodeTreePanel.h"
//! \brief Header file for NodeTreePanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Sebastian Bach <sebastian.bach@filmakademie.de>
//! \version    1.0
//! \date       24.06.2009 (last updated)
//!

#ifndef NodeTreePanel_H
#define NodeTreePanel_H

#include "FrapperPrerequisites.h"
#include "ViewPanel.h"
#include "Node.h"
#include "Parameter.h"
#include "NumberParameter.h"
#include "FilenameParameter.h"
#include "EnumerationParameter.h"
#include "ParameterPlugin.h"
#include <QTreeView>


//!
//! Class representing a view for displaying available patterns (composites of nodes for visual encoding).
//!
class NodeTreePanel : public ViewPanel
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the NodeTreePanel class.
    //!
    //! \param parent The parent widget the created instance will be a child of.
    //! \param flags Extra widget options.
    //!
    NodeTreePanel ( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

    //!
    //! Destructor of the NodeTreePanel class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~NodeTreePanel ();

public slots: //

   

public: // functions


	//!
	//! Connects the panel with the scene.
	//!
	//! \param *nodeModel NodeModel of the scene
	//! \param *sceneModel SceneModel of the scene
	//!
	virtual void registerControl(NodeModel *nodeModel, SceneModel *sceneModel);

	//!
	//! Fills the given tool bars with actions for the NodeTreePanel view.
	//!
	//! \param mainToolBar The main tool bar to fill with actions.
	//! \param panelToolBar The panel tool bar to fill with actions.
	//!
	void fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar );

private slots: //

	//!
	//! Sets the description mode
	//!
	void showDescription(bool description);

private: // data


	//!
	//! Node model
	//!
	NodeModel *m_nodeModel;

	//!
	//! selected node
	//!
	Node *m_node;

	//!
	//! Scene model
	//!
	SceneModel *m_sceneModel;

	//!
	//! Bool if detailed description should be used
	//!
	//! \param description boolean value of the action´s state
	//!
	bool m_description;
};


#endif
