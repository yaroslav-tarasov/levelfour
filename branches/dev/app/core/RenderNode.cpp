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
//! \file "RenderNode.cpp"
//! \brief Implementation file for RenderNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       26.05.2009 (last updated)
//!

#include "RenderNode.h"
#include "Parameter.h"
#include "SceneNodeParameter.h"
#include "OgreManager.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the RenderNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
RenderNode::RenderNode ( const QString &name, ParameterGroup *parameterRoot ) :
    ImageNode(name, parameterRoot),
    m_renderWidth(DefaultRenderWidth),
    m_renderHeight(DefaultRenderHeight),
    m_sceneManager(0),
    m_viewport(0),
    m_camera(0),
    m_renderTexture(0),
    m_renderTextureName(Ogre::String(""))
{
    Ogre::Root *ogreRoot = OgreManager::getRoot();
    if (ogreRoot) {
        // create new Ogre::SceneManager to handle the temporary render scene
        m_sceneManager = ogreRoot->createSceneManager(Ogre::ST_GENERIC, createUniqueName("SceneManager_RenderNode"));
        if (m_sceneManager) {
            m_renderTextureName = createUniqueName("RenderTexture_RenderNode");
			m_renderTexture = Ogre::TextureManager::getSingleton().createManual(m_renderTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, m_renderWidth, m_renderHeight, 0, Ogre::PF_FLOAT16_RGBA, Ogre::TU_RENDERTARGET);
            // create temp camera for render target
			//Ogre::Camera *camera = m_sceneManager->createCamera(createUniqueName("TmpCamera"));
            m_camera = m_sceneManager->createCamera(createUniqueName("TmpCamera"));
            m_camera->setAspectRatio((double)m_renderWidth / (double)m_renderHeight);
            // create viewport for render target
            m_viewport = m_renderTexture->getBuffer()->getRenderTarget()->addViewport(m_camera);
            m_viewport->setBackgroundColour(Ogre::ColourValue(0,1,0));
            m_viewport->setOverlaysEnabled(false);
        }
    }

	// create default texture
	Ogre::TextureManager &textureManager = Ogre::TextureManager::getSingleton();
    m_defaultTexture = (Ogre::TexturePtr) textureManager.getByName("MissingCameraTexture");

    if (m_defaultTexture.isNull()) {
        m_defaultTexture = textureManager.createManual("MissingCameraTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, DefaultRenderWidth, DefaultRenderHeight, 0, Ogre::PF_R8G8B8,  Ogre::TU_DEFAULT);
        Ogre::Image defaultImage;
        defaultImage.load("DefaultRenderImage.png", "General");
        m_defaultTexture->loadImage(defaultImage);
    }

	Parameter *outputParameter = getParameter(m_outputImageName);
    outputParameter->setAuxProcessingFunction(SLOT(redrawTriggered()));
}


//!
//! Destructor of the RenderNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
RenderNode::~RenderNode ()
{
    if (!m_renderTexture.isNull()) {
        m_renderTexture->getBuffer()->getRenderTarget()->removeAllListeners();
		if(Ogre::TextureManager::getSingleton().resourceExists(m_renderTexture->getName())) {
			Ogre::TextureManager::getSingleton().remove(m_renderTexture->getName());
		}
    }

	finalizeRenderTargets();

	Ogre::CompositorManager::getSingleton().removeCompositorChain(m_viewport);
	m_renderTexture->getBuffer()->getRenderTarget()->removeAllViewports();
    Ogre::Root *ogreRoot = OgreManager::getRoot();
    if (ogreRoot) {
		m_sceneManager->destroyAllCameras();
        ogreRoot->destroySceneManager(m_sceneManager);
    }
}

///
/// Private Methods
///



//!
//! Generates a Ogre RenderTexture by using the createManual(...) function.
//!
//! \return The unique name of the new render texture.
//!
Ogre::String RenderNode::generateTexture(const QString &name, Ogre::TextureType texType, Ogre::uint width, Ogre::uint height, int num_mips, Ogre::PixelFormat format, int usage)
{
	// create unique name for the reder texture
	Ogre::String uniqueRenderTextureName = createUniqueName(name);
	// convert Ogre::String to QString
	QString qtUniqueRenderTextureName = QString::fromStdString(uniqueRenderTextureName);
	// create render Texture
	Ogre::TexturePtr test = Ogre::TextureManager::getSingleton().createManual(uniqueRenderTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, texType, width, height, num_mips, format, usage);
    m_textureMap[qtUniqueRenderTextureName] = test;
	return uniqueRenderTextureName;
}

//!
//! Create a viewport for all RenderTextures (RenderTargets) in m_textureMap.
//!
//!	\param cam Camera to create a Ogre::Viewport.
//!	
void RenderNode::initializeRenderTargets(Ogre::Camera* cam)
{
	QMap<QString, Ogre::TexturePtr>::iterator iterTexMap;
	QMap<QString, Ogre::Viewport*>::iterator iterViewMap;

	for (iterTexMap=m_textureMap.begin(); iterTexMap!=m_textureMap.end(); ++iterTexMap) {
		// create viewports for all render targets in the m_textureMap
		iterViewMap = m_viewportMap.find(iterTexMap.key());
		Ogre::TexturePtr texptrTest = iterTexMap.value();
		Ogre::Viewport *test = iterViewMap.value();
		if (iterViewMap == m_viewportMap.end())
			iterViewMap = m_viewportMap.insert(iterTexMap.key(), iterTexMap.value()->getBuffer()->getRenderTarget()->addViewport(cam));

		iterViewMap.value()->setClearEveryFrame(true);
		iterViewMap.value()->setBackgroundColour(Ogre::ColourValue(0,0,0));
        //iterViewMap.value()->setBackgroundColour(OgreManager::convertColor(getColorValue(BackgroundColorParameterName)));
        iterViewMap.value()->setOverlaysEnabled(false);

		//Ogre::TexturePtr texptrTest = iterTexMap.value();
		//Ogre::Viewport *test = iterViewMap.value();
	}
}

//!
//!	Resizes all textures in m_textureMap.
//!
//!	\param width The width of new render texture.
//!	\param height The height of new render texture.
//!
void RenderNode::resizeAllTexturesInMap(int width, int height)
{

	Ogre::TextureManager &textureManager = Ogre::TextureManager::getSingleton();
	Ogre::String textureName, resourceGroupManager;
	Ogre::TextureType texType;

	for (int i = 0; i < m_textureMap.values().size(); ++i) {
		Ogre::TexturePtr texture = m_textureMap.values()[i];
		// save parameters of the texture
		textureName = texture->getName();
		resourceGroupManager = texture->getGroup();
		texType = texture->getTextureType();
		size_t num_mips = texture->getNumMipmaps();
		Ogre::PixelFormat format = texture->getFormat();
		int usage = texture->getUsage();

		// delete the render texture
		if (textureManager.resourceExists(textureName)) {
			texture->getBuffer()->getRenderTarget()->removeAllViewports();
			textureManager.remove(textureName);
		}

		// create new render Texture
		Ogre::TexturePtr newTexture = textureManager.createManual(textureName, resourceGroupManager, texType, width, height, num_mips, format, usage);
		m_textureMap[QString::fromStdString(textureName)] = newTexture;
	}
	m_viewportMap.clear();
}

//!
//!	Resizes the main render texture (m_renderTexture).
//!
//!	\param width The width of new render texture.
//!	\param height The height of new render texture.
//!
void RenderNode::resizeRenderTexture(int width, int height)
{
	Ogre::TextureManager &textureManager = Ogre::TextureManager::getSingleton();
	// delete the render texture
    if (textureManager.resourceExists(m_renderTextureName)) {
        m_renderTexture->getBuffer()->getRenderTarget()->removeAllViewports();
        textureManager.remove(m_renderTextureName);
    }
	// (re-)create the render texture
	m_renderTexture = Ogre::TextureManager::getSingleton().createManual(m_renderTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, width, height, 0, Ogre::PF_FLOAT16_RGBA, Ogre::TU_RENDERTARGET);

	//Ogre::Camera* tempCamera = new Ogre::Camera("tempCamX", m_sceneManager); 

	m_viewport = m_renderTexture->getBuffer()->getRenderTarget()->addViewport(m_camera);

	m_viewport->setClearEveryFrame(true);
	m_viewport->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
    m_viewport->setOverlaysEnabled(false);
}

//!
//! Returns one image of the m_textureMap selected by its name
//!
//! \return The image selected by its name.
//!
//Ogre::TexturePtr RenderNode::getImage ()
//{
//	return getTextureValue(m_viewImageName, true);
//}

//!
//! Clean up all textures
//!
void RenderNode::finalizeRenderTargets()
{

	QMap<QString, Ogre::TexturePtr>::iterator iterTexMap;
	QMap<QString, Ogre::Viewport*>::iterator iterViewMap;

	for (iterTexMap=m_textureMap.begin(); iterTexMap!=m_textureMap.end(); ++iterTexMap) {
		if (!iterTexMap.value().isNull()) {
			iterTexMap.value()->getBuffer()->getRenderTarget()->removeAllListeners();

			if(Ogre::TextureManager::getSingleton().resourceExists(iterTexMap.value()->getName()))
				Ogre::TextureManager::getSingleton().remove(iterTexMap.value()->getName());
		}
		Ogre::CompositorManager::getSingleton().removeCompositorChain(m_viewportMap[iterTexMap.key()]);
		iterTexMap.value()->getBuffer()->getRenderTarget()->removeAllViewports();
	}
}

//!
//! Remove all viewports from the rendertargets of all textures in the m_textureMap
//!
void RenderNode::removeViewportsFromRenderTargets()
{
	// get OGRE texture manager
    Ogre::TextureManager &textureManager = Ogre::TextureManager::getSingleton();
	QMap<QString, Ogre::TexturePtr>::iterator iterTexMap;

	for (iterTexMap=m_textureMap.begin(); iterTexMap!=m_textureMap.end(); ++iterTexMap) {
		if (textureManager.resourceExists(Ogre::String(iterTexMap.key().toStdString())))
			iterTexMap.value()->getBuffer()->getRenderTarget()->removeAllViewports();
			iterTexMap.value()->getBuffer()->getRenderTarget()->removeAllListeners();
	}
	m_viewportMap.clear();
}

///
/// Public Methods
///


//!
//! Returns the width of the render target.
//!
//! \return The width of the render target.
//!
unsigned int RenderNode::getWidth() const
{
    return this->m_renderWidth;
}

//!
//! Returns the height of the render target.
//!
//! \return The height of the render target.
//!
unsigned int RenderNode::getHeight() const
{
    return this->m_renderWidth;
}


///
/// Public Slots
///


//!
//! Redraw of ogre scene has been triggered.
//!
void RenderNode::redrawTriggered ()
{
	/*if (m_renderTexture.isNull())
		return;*/

	Ogre::RenderTexture *renderTarget = m_renderTexture->getBuffer()->getRenderTarget();
    if (renderTarget)
        renderTarget->update();

	Ogre::RenderTexture *texture;

	foreach (Ogre::TexturePtr ptr, m_textureMap) {
		if (!ptr.isNull() && (texture = ptr->getBuffer()->getRenderTarget()))
			texture->update();
	}

}
