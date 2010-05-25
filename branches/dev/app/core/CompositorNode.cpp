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
//! \file "CompositorNode.cpp"
//! \brief Implementation file for CompositorNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       26.05.2009 (last updated)
//!

#include "CompositorNode.h"
#include "Parameter.h"
#include "SceneNodeParameter.h"
#include "OgreTools.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the CompositorNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CompositorNode::CompositorNode ( const QString &name, ParameterGroup *parameterRoot ) :
    RenderNode(name, parameterRoot),
	m_compositor(0),
	m_resourceGroupName("")
{
	
	// set the reload functionality
    Parameter *reloadParameter = getParameter("Reload");
    if (reloadParameter) {
        // set the command function for the reset parameter
        reloadParameter->setCommandFunction(SLOT(reload()));
    } else
		Log::error("Could not obtain reload parameter.", "CompositorNode::CompositorNode");
}


//!
//! Destructor of the CompositorNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CompositorNode::~CompositorNode ()
{
	if (m_compositor) {
		m_compositor->setEnabled(false);
		m_compositor->removeListener(this);
	}
    DEC_INSTANCE_COUNTER
}

///
/// Protected Slots
///

///
/// Reload resources.
///
void CompositorNode::reload ()
{
    // save the compositor name and destroy compositor
    //if (m_compositor)
    //    return;
    m_compositor->setEnabled(false);
    Ogre::Compositor *compositor = m_compositor->getCompositor();
    Ogre::String compositorName;
    if (compositor) {
        compositorName = compositor->getName();
        m_compositor->removeListener(this);
        Ogre::CompositorManager::getSingleton().removeCompositorChain(m_viewport);		
        m_compositor = 0;
    }

	// get the resource location and reload the resources
	QString resourceGroupName = getStringValue("Resource Group Name");
	std::cout << resourceGroupName.toStdString() << std::endl;
	if (resourceGroupName != "") {

		//TODO: NILZ: Do resource group checking.
		Ogre::FileInfoListPtr fileInfoVector = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo(resourceGroupName.toStdString(), false);
		
		if (fileInfoVector->size() > 0) {
			Ogre::FileInfo fileInfo = fileInfoVector->at(0);
			Ogre::String archiveName = fileInfo.archive->getName();
			OgreTools::destroyResourceGroup(resourceGroupName);
			OgreTools::createResourceGroup(resourceGroupName, QString::fromStdString(archiveName));
		}
	}
	
	// create the new compositor with old name and location
	m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, compositorName, 0);
	if (m_compositor) {
		m_compositor->addListener(this);
		m_compositor->setEnabled(true);
		triggerRedraw();
	}
}


//!
//! Protected Functions
//!

//!
//!	Resizes the main render texture (m_renderTexture).
//!
//!	\param width The width of render texture.
//!	\param height The height of render texture.
//!
void CompositorNode::resizeRenderTexture(int width, int height, Ogre::PixelFormat pixelFormat /* = Ogre::PF_FLOAT16_RGBA */)
{
	if (m_renderTexture.isNull() || (m_renderTexture->getWidth() == width && m_renderTexture->getHeight() == height 
                                                                          && m_renderTexture->getFormat() == pixelFormat))
        return;

    // save the compositor name and destroy compositor
	m_compositor->setEnabled(false);
	Ogre::Compositor *compositor = m_compositor->getCompositor();
	Ogre::String compositorName;
	if (compositor) {
		compositorName = compositor->getName();
		m_compositor->removeListener(this);
		Ogre::CompositorManager::getSingleton().removeCompositorChain(m_viewport);		
		m_compositor = 0;
	}
	
	RenderNode::resizeRenderTexture(width, height, pixelFormat);
	
	// create the new compositor with old name and location
	m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, compositorName, 0);
	if (m_compositor) {
		m_compositor->addListener(this);
		m_compositor->setEnabled(true);
	}
}


    //!
    //! Get shader parameters.
    //!
    //! \param material The material.
    //!
    Ogre::GpuProgramParametersSharedPtr CompositorNode::getShaderParameters( Ogre::MaterialPtr &material )
    {
        if (!material.isNull())
            return material->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
        else
            return Ogre::GpuProgramParametersSharedPtr();
    }

    //!
    //! Assign texture to texture slot.
    //!
    //! \param material The material.
    //! \param texture Pointer to the texture.
    //! \param slot The texture slot id.
    //!
    void CompositorNode::setTexture( Ogre::MaterialPtr &material, Ogre::TexturePtr &texture, unsigned int slot )
    {
        if (!material.isNull() && !texture.isNull()) {
            Ogre::Technique *technique = material->getTechnique(0);
            if (!technique)
                return;
            Ogre::Pass *pass = technique->getPass(0);
            if (!pass)
                return;
            unsigned short numPasses = pass->getNumTextureUnitStates();
            if (slot >= numPasses)
                return;
            Ogre::TextureUnitState *textureUnitState = pass->getTextureUnitState(slot);
            textureUnitState->setTextureName(texture->getName());
            //material->getTechnique(0)->getPass(0)->getTextureUnitState(slot)->setTextureName(texture->getName());
        }
    }

    //!
    //! Set integer shader parameter.
    //!
    //! \param mat The shader parameters.
    //! \param name Name of the parameter.
    //! \param value Integer value of the parameter.
    //!
    void CompositorNode::setShaderParameter(Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, int value)
    {
        if (fpParams->_findNamedConstantDefinition(name)) {
            fpParams->setNamedConstant(name, value);
        }
    }

    //!
    //! Set float shader parameter.
    //!
    //! \param mat The shader parameters.
    //! \param name Name of the parameter.
    //! \param value Float value of the parameter.
    //!
    void CompositorNode::setShaderParameter(Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Real value)
    {
        if (fpParams->_findNamedConstantDefinition(name))
            fpParams->setNamedConstant(name, value);
    }

    //!
    //! Set vector (3 components) shader parameter.
    //!
    //! \param mat The shader parameters.
    //! \param name Name of the parameter.
    //! \param vector Vector (3 components) value of the parameter.
    //!
    void CompositorNode::setShaderParameter(Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Vector3 vector)
    {
        if (fpParams->_findNamedConstantDefinition(name))
            fpParams->setNamedConstant(name, vector);
    }

    //!
    //! Set vector (4 components) shader parameter.
    //!
    //! \param mat The shader parameters.
    //! \param name Name of the parameter.
    //! \param vector Vector (4 components) value of the parameter.
    //!
    void CompositorNode::setShaderParameter(Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Vector4 vector)
    {
        if (fpParams->_findNamedConstantDefinition(name))
            fpParams->setNamedConstant(name, vector);
    }

    //!
    //! Set matrix (4x4 components) shader parameter.
    //!
    //! \param mat The shader parameters.
    //! \param name Name of the parameter.
    //! \param vector Matrix (4x4 components) value of the parameter.
    //!
    void CompositorNode::setShaderParameter(Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Matrix4 matrix)
    {
        if (fpParams->_findNamedConstantDefinition(name))
            fpParams->setNamedConstant(name, matrix);
    }

//!
//! Callback when instance of this class is registered as Ogre::CompositorListener.
//! 
//! \param pass_id Id to identifiy current compositor pass.
//! \param mat Material this pass is currently using.
//!
void CompositorNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
}