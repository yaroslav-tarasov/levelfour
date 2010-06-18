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
file "MaterialMapperNode.cpp"
brief Implementation file for MaterialMapperNode class.

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


#include "MaterialMapperNode.h"
#include "MaterialMapParameter.h"
#include "OgreManager.h"
#include "OgreTools.h"
#include "OgreScriptCompiler.h"
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtCore/QTime>

///
/// Constructors and Destructors
///

//!
//! Constructor of the MaterialMapperNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
MaterialMapperNode::MaterialMapperNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
	m_resourceGroup(name.toStdString()),
	m_outputMaterialMapParameterName("MaterialMapOutput"),
	m_material("")
{
    // create the MaterialMap output parameter
	m_outputMaterialMapParameter = new MaterialMapParameter(m_outputMaterialMapParameterName);
    m_outputMaterialMapParameter->setPinType(Parameter::PT_Output);
    m_outputMaterialMapParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(m_outputMaterialMapParameter);
	
	// set affections and functions
    addAffection("Material File", m_outputMaterialMapParameterName);
    setChangeFunction("Material File", SLOT(loadMaterial()));
    setCommandFunction("Material File", SLOT(loadMaterial()));
}


//!
//! Destructor of the MaterialMapperNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
MaterialMapperNode::~MaterialMapperNode ()
{
}

//!
//!
//! \return True, if successful loading of material
//!            False, otherwise.
//!
bool MaterialMapperNode::loadMaterial()
{
	QString filename = getStringValue("Material File");
    if (filename == "") {
        Log::debug(QString("Wrong material file, it has not been set yet. (\"%1\")").arg(m_name), "MaterialMapperNode::loadMaterial");
        return false;
    }

	// check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Mesh file \"%1\" not found.").arg(filename), "MaterialMapperNode::loadMaterial");
        return false;
    }

	// load material file
	QFile file(filename);
	if (!file.open(QIODevice::Text | QIODevice::ReadOnly))
		return false;

	// split the absolute filename to path and base filename
    int lastSlashIndex = filename.lastIndexOf('/');
    QString path = "";
    if (lastSlashIndex > -1) {
        path = filename.mid(0, lastSlashIndex);
        filename = filename.mid(lastSlashIndex + 1);
    }
    if (!filename.endsWith(".material")) {
        Log::error("The Material file has to be an OGRE material script.", "MaterialMapperNode::loadMaterial");
        return false;
	} else {
		// remove .material 
		int lastDot = filename.lastIndexOf('.');
        filename = filename.mid(0, lastDot);
	}

	m_material = filename.toStdString();

	if (resourceGroupExists(m_resourceGroup))
	{
		Ogre::ResourceGroupManager * rgm = Ogre::ResourceGroupManager::getSingletonPtr();
		rgm->unloadResourceGroup(m_resourceGroup);
		rgm->destroyResourceGroup(m_resourceGroup);
	}
	Ogre::ResourceGroupManager * rgm = Ogre::ResourceGroupManager::getSingletonPtr();
	rgm->createResourceGroup(m_resourceGroup);
	rgm->addResourceLocation(path.toStdString(), "FileSystem", m_resourceGroup);
	rgm->declareResource(m_material, "Material", m_resourceGroup);
	rgm->initialiseResourceGroup(m_resourceGroup);
	Ogre::ResourceBackgroundQueue::getSingleton().loadResourceGroup(m_resourceGroup);

//	Ogre::MaterialPtr compilematerial = Ogre::MaterialManager::getSingleton().getByName(materialName);

	updateMaterialOuput();
	Log::info(QString("Material file \"%1\" loaded.").arg(QString(m_material.c_str())), "MaterialMapperNode::loadMaterial");
    return true;
}

// update the material map output
void MaterialMapperNode::updateMaterialOuput()
{
	if (m_outputMaterialMapParameter->getMaterialForAll() == m_material)
		return;

	m_outputMaterialMapParameter->setMaterialForAll(m_material);
	m_outputMaterialMapParameter->propagateDirty(0);
}

bool MaterialMapperNode::resourceGroupExists( Ogre::String &group ) const
{
   Ogre::StringVector groupNameList = Ogre::ResourceGroupManager::getSingleton().getResourceGroups(); 
   Ogre::StringVector::iterator resGroupIter = groupNameList.begin(); 

   for(;resGroupIter < groupNameList.end();resGroupIter++)
   { 
      Ogre::String resGroupName = (*resGroupIter); 
      if( resGroupName == group )
      {
             return true;
      } 
   }
   return false;
}