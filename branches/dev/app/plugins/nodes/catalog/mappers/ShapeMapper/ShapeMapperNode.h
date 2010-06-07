/*
-----------------------------------------------------------------------------
This source file is part of Acuity4D
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
//! \file "ShapeMapperNode.h"
//! \brief Header file for ShapeMapperNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef ShapeMapperNODE_H
#define ShapeMapperNODE_H

#include "Node.h"
#include "Ogre.h"
#include "OgreTools.h"
#include "EntityParameter.h"
#include "ShapeMapParameter.h";

// OGRE
#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif


//!
//! Class for a Degree Centrality Measure
//!
class ShapeMapperNode : public Node
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the ShapeMapperNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ShapeMapperNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ShapeMapperNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ShapeMapperNode ();

private slots: //
	void processShapeMap();

private: // functions

private: //data

	//!
    //! The name of the input geometry parameter.
    //!
    QString m_inputEntityParameterName;

	//!
    //! The input entity parameter.
    //!
	EntityParameter *inputEntityParameter;

	//!
    //! OGRE entity.
    //!
    Ogre::Entity *m_entity;

    //!
    //! The input shape map parameter name. 
    //!
	QString m_inputShapeMapParameterName;

    //!
    //! The input shape map parameter. If set, than it's directly used as shape mapper
    //!
	ShapeMapParameter * inputShapeMapParameter;

	//!
    //! The ôutput shape map parameter name. 
    //!
	QString m_outputShapeMapParameterName;

    //!
    //! The ôutput shape map parameter. If set, than it's directly used as shape mapper
    //!
	ShapeMapParameter * outputShapeMapParameter;

    //!
    //! The name of the vtk table input parameter.
    //!
	QString m_inputVTKTableName; 

    //!
    //! The vtk table input parameter.
    //!
	VTKTableParameter * inputVTKTableParameter;

	//!
    //! The enumeration used to select which shape table to use.
    //!
	EnumerationParameter * shapeFieldParameter;
};


#endif
