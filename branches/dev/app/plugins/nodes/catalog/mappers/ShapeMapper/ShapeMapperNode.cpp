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

/*
-----------------------------------------------------------------------------
file "ShapeMapperNode.cpp"
brief Implementation file for ShapeMapperNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------


Parameters
----------
      
Returns
-------

    
Notes
-----

Reference
---------

-----------------------------------------------------------------------------
*/


#include "ShapeMapperNode.h"
#include "EntityParameter.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "SceneNodeParameter.h"
#include "FilenameParameter.h"
#include "OgreManager.h"
#include "OgreTools.h"
#include <QtCore/QFile>
#include <QtCore/QDir>

INIT_INSTANCE_COUNTER(ShapeMapperNode)

///
/// Constructors and Destructors
///

//!
//! Constructor of the ShapeMapperNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ShapeMapperNode::ShapeMapperNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_outputGeometryParameterName("GeometryOutput")
{
	// create the geometry output parameter (shape mapper)
	m_outputGeometryParameter = new EntityParameter(m_outputGeometryParameterName);
	m_outputGeometryParameter->setPinType(Parameter::PT_Output);
	parameterRoot->addParameter(m_outputGeometryParameter);

	// set affections and functions
    setChangeFunction("Geometry File", SLOT(geometryFileChanged()));
    setCommandFunction("Geometry File", SLOT(geometryFileChanged()));

	INC_INSTANCE_COUNTER
}

//!
//! Destructor of the ShapeMapperNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ShapeMapperNode::~ShapeMapperNode ()
{
    destroyEntity();
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);

	DEC_INSTANCE_COUNTER
}

bool ShapeMapperNode::geometryFileChanged ()
{
    // destroy an existing OGRE entity for the mesh
    destroyEntity();

	QString filename = getStringValue("Geometry File");
    if (filename == "") {
        Log::debug(QString("Geometry file has not been set yet. (\"%1\")").arg(m_name), "ShapeMapperNode::geometryFileChanged");
        return false;
    }

	// check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Mesh file \"%1\" not found.").arg(filename), "ShapeMapperNode::geometryFileChanged");
        return false;
    }

    // split the absolute filename to path and base filename
    int lastSlashIndex = filename.lastIndexOf('/');
    QString path = "";
    if (lastSlashIndex > -1) {
        path = filename.mid(0, lastSlashIndex);
        filename = filename.mid(lastSlashIndex + 1);
    }
    if (!filename.endsWith(".mesh")) {
        Log::error("The geometry file has to be an OGRE mesh file.", "ShapeMapperNode::geometryFileChanged");
        return false;
    }

	// destroy old resource group and generate new one
    QString resourceGroupName = QString::fromStdString(createUniqueName("ResourceGroup_" + filename + "_ShapeMapperNode"));
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);
    m_oldResourceGroupName = resourceGroupName;
    OgreTools::createResourceGroup(resourceGroupName, path);

    // recreating the entity
	createEntity(m_name, filename);

    Log::info(QString("Mesh file \"%1\" loaded.").arg(filename), "ShapeMapperNode::geometryFileChanged");
    return true;
}

void ShapeMapperNode::destroyEntity ()
{
    if (m_entity) {
        // remove the entity from the scene node it is attached to
        Ogre::SceneNode *parentSceneNode = m_entity->getParentSceneNode();
        if (parentSceneNode)
            parentSceneNode->detachObject(m_entity);

        // destroy the entity through its scene manager
        Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
        if (sceneManager) {
            sceneManager->destroyEntity(m_entity);
            m_entity = 0;
        }
    }
}

//!
//! Create new scene.
//! \return True if the scene was successfully created, otherwise False.
//!
bool ShapeMapperNode::createEntity(QString name, QString fileName)
{
    // destroy the entity through its scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    // create a new OGRE entity for each vertex
    m_entity = sceneManager->createEntity(name.toStdString(), fileName.toStdString());
    if (m_entity) {
        // set cumulative blend mode instead of Ogre::ANIMBLEND_AVERAGE which is default
        if (m_entity->hasSkeleton()) {
            Ogre::Skeleton *skeleton = m_entity->getSkeleton();
            skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
        }

		m_outputGeometryParameter->setEntity(m_entity);
    }

	return true;
}
