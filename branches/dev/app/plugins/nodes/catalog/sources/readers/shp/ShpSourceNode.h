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
//! \file "ShpSourceNode.h"
//! \brief Header file for ShpSourceNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef ShpSourceNode_H
#define ShpSourceNode_H

#include "VTKTableNode.h"
#include "ShapeMapParameter.h"
#include "OgreManualObject.h"
#include "vtkPolyData.h"
#include "vtkTable.h"

// OGRE
#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif


//!
//! Class for a node that is used for debugging purposes.
//!
class ShpSourceNode : public VTKTableNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the ShpSourceNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ShpSourceNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ShpSourceNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ShpSourceNode ();

private slots: // slots
	void shapeFileChanged();

private: // function 

	// Create the table that will contain an Ogre mesh for each shape contained in the shapefile
	vtkTable * polydataToMesh(vtkPolyData * polydata, int type);

	// Empty the table and destroy all meshes contained on it
	void cleanTable();

private: // data
	QString m_outputShapeMapParameterName;

	ShapeMapParameter * outputShapeMapParameter;
};


#endif
