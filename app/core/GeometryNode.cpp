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
//! \file "GeometryNode.cpp"
//! \brief Implementation file for GeometryNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "GeometryNode.h"
#include "SceneNodeParameter.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the GeometryNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \param outputGeometryName The name of the geometry output parameter.
//!
GeometryNode::GeometryNode ( const QString &name, ParameterGroup *parameterRoot, const QString &outputGeometryName /* = "Geometry" */ ) :
    ViewNode(name, parameterRoot),
    m_outputGeometryName(outputGeometryName)
{
    addOutputParameter(new GeometryParameter(m_outputGeometryName));
}


//!
//! Destructor of the GeometryNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
GeometryNode::~GeometryNode ()
{
}


///
/// Public Functions
///


//!
//! Returns the scene node that contains scene objects created or modified
//! by this node.
//!
//! \return The scene node containing objects created or modified by this node.
//!
Ogre::SceneNode * GeometryNode::getSceneNode ()
{
    Ogre::SceneNode *result = getSceneNodeValue(m_outputGeometryName, true);
    if (!result)
        Log::error("Could not obtain scene node.", "GeometryNode::getSceneNode");
    return result;
}


//!
//! Returns the radius of the bounding sphere surrounding this object.
//!
//! \return The radius of the bounding sphere surrounding this object.
//!
double GeometryNode::getBoundingSphereRadius () const
{
    return 0;
}


///
/// Private Functions
///


//!
//! Attaches a locator geometry to the geometry node's scene node.
//!
void GeometryNode::createLocatorGeometry ()
{
    //Ogre::Root *ogreRoot = Ogre::Root::getSingletonPtr();
    //Ogre::SceneManager *sceneManager = 0;
    //if (ogreRoot)
    //    sceneManager = ogreRoot->getSceneManager(SCENE_MANAGER_NAME);
    //if (!sceneManager) {
    //    Log::error("The OGRE scene manager could not be obtained.", "NullNode::createGeometry");
    //    return;
    //}

    //if (!m_nullGeometrySceneNode) {
    //    Log::error("The null geometry's OGRE scene node could not be obtained.", "NullNode::createGeometry");
    //    return;
    //}

    //Ogre::ManualObject *xAxis = sceneManager->createManualObject(QString(m_name + "_xAxis").toStdString());
    //xAxis->begin("nullMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    //xAxis->position(-NullGeometryScale, 0, 0);
    //xAxis->position(NullGeometryScale, 0, 0);
    //xAxis->end();

    //Ogre::ManualObject *yAxis = sceneManager->createManualObject(QString(m_name + "_yAxis").toStdString());
    //yAxis->begin("nullMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    //yAxis->position(0, -NullGeometryScale, 0);
    //yAxis->position(0, NullGeometryScale, 0);
    //yAxis->end();

    //Ogre::ManualObject *zAxis = sceneManager->createManualObject(QString(m_name + "_zAxis").toStdString());
    //zAxis->begin("nullMaterial", Ogre::RenderOperation::OT_LINE_LIST);
    //zAxis->position(0, 0, -NullGeometryScale);
    //zAxis->position(0, 0, NullGeometryScale);
    //zAxis->end();

    //m_nullGeometrySceneNode->attachObject(xAxis);
    //m_nullGeometrySceneNode->attachObject(yAxis);
    //m_nullGeometrySceneNode->attachObject(zAxis);
}

