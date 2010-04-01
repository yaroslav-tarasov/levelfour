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
//! \file "AdvancedRenderNode.cpp"
//! \brief Implementation file for AdvancedRenderNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author		Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       16.10.2009 (last updated)
//!

#include "AdvancedRenderNode.h"
#include "GeometryRenderNode.h"
#include "Parameter.h"
#include "SceneNodeParameter.h"
#include "OgreTools.h"
#include "OgreContainer.h"
#include "OgreManager.h"


Depth3DRenderListener::Depth3DRenderListener(Ogre::MaterialManager::Listener* materialListener, AdvancedRenderNode* node) :
	m_materialListener(materialListener),
	m_node(node)
{
}

void Depth3DRenderListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
	Ogre::MaterialManager::getSingleton().addListener( m_materialListener );
	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("AdvancedRender/BasicWorldDepth3D");
	Ogre::Camera *camera = 0;
	Ogre::SceneNode *sceneNode = 0;
	Ogre::Vector3 campos = Ogre::Vector3(0.0);

	if ((sceneNode = m_node->getSceneNodeValue("Camera")) &&
		(camera = OgreTools::getFirstCamera(sceneNode)))
		campos = camera->getRealPosition();

	if (!mat.isNull()) {
		Ogre::GpuProgramParametersSharedPtr fpParams = mat->getTechnique(0)->getPass(0)->getVertexProgramParameters();
		if (fpParams->_findNamedConstantDefinition("camp")) {
			fpParams->setNamedConstant("camp", campos);
		}
	}
}

void Depth3DRenderListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
	Ogre::MaterialManager::getSingleton().removeListener( m_materialListener );
}


RenderListener::RenderListener(Ogre::MaterialManager::Listener* materialListener) :
	m_materialListener(materialListener)
{
}

void RenderListener::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
	Ogre::MaterialManager::getSingleton().addListener( m_materialListener );		
}

void RenderListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
	Ogre::MaterialManager::getSingleton().removeListener( m_materialListener );
}

///
/// Constructors and Destructors
///

//!
//! Constructor of the AdvancedRenderNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
AdvancedRenderNode::AdvancedRenderNode ( const QString &name, ParameterGroup *parameterRoot ) :
    GeometryRenderNode(name, parameterRoot),
	m_renderTarget(0)
{
    if (m_sceneManager)
        m_sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	m_materialSwitchListener = new MaterialSwitcher();

	m_outputSecondImageName = QString("Normal Map");
	m_outputThirdImageName  = QString("Depth Map");
	m_outputFourthImageName  = QString("Depth Map 3D");

    // load default texture
    m_defaultTextureImage.load("DefaultRenderImage.png", "General");

	Parameter *outputImageParameter = getParameter(m_outputImageName);
    if (outputImageParameter) {
        outputImageParameter->setProcessingFunction(SLOT(processOutputImage()));
        outputImageParameter->setDirty(true);
		// set up parameter affections
        outputImageParameter->addAffectingParameter(getParameter(GeometryRenderNode::InputGeometryParameterName));
        outputImageParameter->addAffectingParameter(getParameter(GeometryRenderNode::InputLightsParameterName));
        outputImageParameter->addAffectingParameter(getParameter(GeometryRenderNode::InputCameraParameterName));
        outputImageParameter->addAffectingParameter(getParameter(GeometryRenderNode::CameraLightToggleParameterName));
		outputImageParameter->addAffectingParameter(getParameter(GeometryRenderNode::BackgroundColorParameterName));
    }

	// add second output image parameter for the normal map
    addOutputParameter(Parameter::createImageParameter(m_outputSecondImageName));

	Parameter *outputImageParameterNormal = getParameter(m_outputSecondImageName);
    if (outputImageParameterNormal) {
        outputImageParameterNormal->setProcessingFunction(SLOT(processOutputImage()));
		outputImageParameterNormal->setAuxProcessingFunction(SLOT(redrawTriggered()));
        outputImageParameterNormal->setDirty(true);
		// set up parameter affections
        outputImageParameterNormal->addAffectingParameter(getParameter(GeometryRenderNode::InputGeometryParameterName));
        outputImageParameterNormal->addAffectingParameter(getParameter(GeometryRenderNode::InputLightsParameterName));
        outputImageParameterNormal->addAffectingParameter(getParameter(GeometryRenderNode::InputCameraParameterName));
        outputImageParameterNormal->addAffectingParameter(getParameter(GeometryRenderNode::CameraLightToggleParameterName));
		outputImageParameterNormal->addAffectingParameter(getParameter(GeometryRenderNode::BackgroundColorParameterName));
    }

	// add third output image parameter for the depth map
    addOutputParameter(Parameter::createImageParameter(m_outputThirdImageName));

	Parameter *outputImageParameterDepth = getParameter(m_outputThirdImageName);
    if (outputImageParameterDepth) {
        outputImageParameterDepth->setProcessingFunction(SLOT(processOutputImage()));
		outputImageParameterDepth->setAuxProcessingFunction(SLOT(redrawTriggered()));
        outputImageParameterDepth->setDirty(true);
		// set up parameter affections
        outputImageParameterDepth->addAffectingParameter(getParameter(GeometryRenderNode::InputGeometryParameterName));
        outputImageParameterDepth->addAffectingParameter(getParameter(GeometryRenderNode::InputLightsParameterName));
        outputImageParameterDepth->addAffectingParameter(getParameter(GeometryRenderNode::InputCameraParameterName));
        outputImageParameterDepth->addAffectingParameter(getParameter(GeometryRenderNode::CameraLightToggleParameterName));
		outputImageParameterDepth->addAffectingParameter(getParameter(GeometryRenderNode::BackgroundColorParameterName));
    }

	// add third output image parameter for the 3D depth map
    addOutputParameter(Parameter::createImageParameter(m_outputFourthImageName));

	Parameter *outputImageParameterDepth3D = getParameter(m_outputFourthImageName);
    if (outputImageParameterDepth3D) {
        outputImageParameterDepth3D->setProcessingFunction(SLOT(processOutputImage()));
		outputImageParameterDepth3D->setAuxProcessingFunction(SLOT(redrawTriggered()));
        outputImageParameterDepth3D->setDirty(true);
		// set up parameter affections
		outputImageParameterDepth3D->addAffectingParameter(getParameter(GeometryRenderNode::InputGeometryParameterName));
		outputImageParameterDepth3D->addAffectingParameter(getParameter(GeometryRenderNode::InputLightsParameterName));
		outputImageParameterDepth3D->addAffectingParameter(getParameter(GeometryRenderNode::InputCameraParameterName));
		outputImageParameterDepth3D->addAffectingParameter(getParameter(GeometryRenderNode::CameraLightToggleParameterName));
		outputImageParameterDepth3D->addAffectingParameter(getParameter(GeometryRenderNode::BackgroundColorParameterName));
	}
	
	m_renderTextureNormalName = generateTexture("NormalTexture", Ogre::TEX_TYPE_2D, m_renderWidth, m_renderHeight, 0, Ogre::PF_FLOAT16_RGB, Ogre::TU_RENDERTARGET);
	m_renderTextureDepthName = generateTexture("DepthTexture", Ogre::TEX_TYPE_2D, m_renderWidth, m_renderHeight, 0, Ogre::PF_FLOAT16_R, Ogre::TU_RENDERTARGET);
	m_renderTextureDepth3DName = generateTexture("DepthTexture3D", Ogre::TEX_TYPE_2D, m_renderWidth, m_renderHeight, 0, Ogre::PF_FLOAT16_RGB, Ogre::TU_RENDERTARGET);
}


//!
//! Destructor of the AdvancedRenderNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
AdvancedRenderNode::~AdvancedRenderNode ()
{
    if (m_sceneManager)
        // destroy all scene nodes and objects in the render scene
        OgreTools::deepDeleteSceneNode(m_sceneManager->getRootSceneNode(), m_sceneManager);
}


///
/// Public Slots
///


//!
//! Processes the node's input data to generate the node's output image.
//!
void AdvancedRenderNode::processOutputImage ()
{
    Parameter *tempImageParameter = getParameter(m_outputImageName);
    if (!tempImageParameter) {
        Log::error("Could not obtain output image parameter.", "AdvancedRenderNode::processOutputImage");
        return;
    }

	tempImageParameter = getParameter(m_outputSecondImageName);
    if (!tempImageParameter) {
        Log::error("Could not obtain output image parameter.", "AdvancedRenderNode::processOutputImage");
        return;
    }

	tempImageParameter = getParameter(m_outputThirdImageName);
    if (!tempImageParameter) {
        Log::error("Could not obtain output image parameter.", "AdvancedRenderNode::processOutputImage");
        return;
    }

	tempImageParameter = getParameter(m_outputFourthImageName);
    if (!tempImageParameter) {
        Log::error("Could not obtain output image parameter.", "AdvancedRenderNode::processOutputImage");
        return;
    }

    // check if a camera light should be used instead of the input lights
    bool useCameraLight = getBoolValue(CameraLightToggleParameterName);

    // get OGRE texture manager
    Ogre::TextureManager &textureManager = Ogre::TextureManager::getSingleton();

    // destroy all scene nodes and objects in the render scene
    OgreTools::deepDeleteSceneNode(m_sceneManager->getRootSceneNode(), m_sceneManager);

    if (m_cameraSceneNode) {
        OgreContainer *sceneNodeContainer = Ogre::any_cast<OgreContainer *>(m_cameraSceneNode->getUserAny());
        if (sceneNodeContainer)
            delete sceneNodeContainer;
        OgreTools::deepDeleteSceneNode(m_cameraSceneNode, m_sceneManager);
        m_cameraSceneNode->removeAndDestroyAllChildren();
        m_sceneManager->destroySceneNode(m_cameraSceneNode);
        m_cameraSceneNode = 0;
		m_cameraCopy = 0;
    }

    // destroy cameras, lights and entities
    m_sceneManager->destroyAllCameras();
    m_sceneManager->destroyAllEntities();
    m_sceneManager->destroyAllLights();

    // process input geometry
    Ogre::SceneNode *geometrySceneNode = getSceneNodeValue(InputGeometryParameterName);
    if (geometrySceneNode) {
        // duplicate the input geometry
        Ogre::SceneNode *geometrySceneNodeCopy = 0;
        OgreTools::deepCopySceneNode(geometrySceneNode, geometrySceneNodeCopy, m_name, m_sceneManager);
        // add the geometry to the render scene
        m_sceneManager->getRootSceneNode()->addChild(geometrySceneNodeCopy);
    } else
        Log::error("Could not obtain scene node from input geometry parameter.", "AdvancedRenderNode::processOutputImage");

    // process input lights
    if (!useCameraLight) {
#ifdef LIGHT_PARAMETER_AVAILABLE
        // process input lights
        Ogre::SceneNode *lightsSceneNode = getSceneNodeValue(InputLightsParameterName);
        if (lightsSceneNode) {
             duplicate the input lights
            Ogre::SceneNode *lightsSceneNodeCopy = 0;
            OgreTools::deepCopySceneNode(lightsSceneNode, lightsSceneNodeCopy, m_name, m_sceneManager);
             add the lights to the render scene
            m_sceneManager->getRootSceneNode()->addChild(lightsSceneNodeCopy);
        } else
            Log::error("Could not obtain scene node from input lights parameter.", "AdvancedRenderNode::processOutputImage");
#endif
    }

    // process input camera
    Ogre::SceneNode *cameraSceneNode = getSceneNodeValue(InputCameraParameterName);
	Ogre::RenderTexture *renderTargetNormal, *renderTargetDepth, *renderTargetDepth3D;
	renderTargetNormal = renderTargetDepth = renderTargetDepth3D = 0;

	setValue(m_outputImageName, m_defaultTexture);
	setValue(m_outputSecondImageName, m_defaultTexture);
	setValue(m_outputThirdImageName, m_defaultTexture);
	setValue(m_outputFourthImageName, m_defaultTexture);
	
	if (cameraSceneNode) {
        // get the first camera attached to the input camera scene node
        Ogre::Camera *camera = OgreTools::getFirstCamera(cameraSceneNode);
        
		if (camera) {
			// duplicate the input camera
            m_cameraSceneNode = 0;
            OgreTools::deepCopySceneNode(cameraSceneNode, m_cameraSceneNode, m_name, m_sceneManager);
            if (!m_cameraSceneNode) {
                Log::error("The camera's scene node could not be copied.", "AdvancedRenderNode::processOutputImage");
                return;
            }

            if (useCameraLight) {
                // set up names for camera light objects
                QString cameraLightName = QString("%1CameraLight").arg(m_name);
                //QString cameraLightSceneNodeName = QString("%1SceneNode").arg(cameraLightName);
                // create camera light objects
                Ogre::Light *cameraLight = m_sceneManager->createLight(cameraLightName.toStdString());
                //Ogre::SceneNode *cameraLightSceneNode = m_cameraSceneNode->createChildSceneNode(cameraLightSceneNodeName.toStdString());
                m_cameraSceneNode->attachObject(cameraLight);
            }

			// process input light
			Ogre::SceneNode *lightSceneNode = getSceneNodeValue(InputLightsParameterName);
			if (lightSceneNode) {
				// get the lights attached to the input light scene node
				Ogre::Light *light = OgreTools::getFirstLight(lightSceneNode);
				if (light) {
					// duplicate the input camera
					m_lightSceneNode = 0;
					OgreTools::deepCopySceneNode(lightSceneNode, m_lightSceneNode, m_name, m_sceneManager);
					if (!m_lightSceneNode) {
						Log::error("The light's scene node could not be copied.", "AdvancedRenderNode::processOutputImage");
						return;
					}
					// set visibility of light gometry representation to false
					Ogre::Entity *entity = OgreTools::getFirstEntity(m_lightSceneNode);
					if (entity)
						entity->setVisible(false);
					// add the lights to the render scene
					m_sceneManager->getRootSceneNode()->addChild(m_lightSceneNode);
				}
			}

            // retrieve render resolution values from custom parameter
            const Ogre::Any &customData = dynamic_cast<Ogre::MovableObject *>(camera)->getUserAny();
            if (!customData.isEmpty()) {
                CameraInfo *cameraInfo = Ogre::any_cast<CameraInfo *>(customData);
                if (cameraInfo) {
                    m_renderWidth = cameraInfo->width;
                    m_renderHeight = cameraInfo->height;
                }
            }	        

            // (re-)create the render texture
            if (textureManager.resourceExists(m_renderTextureName)) {
					m_renderTarget = m_renderTexture->getBuffer()->getRenderTarget();
					m_renderTarget->removeAllViewports();
					textureManager.remove(m_renderTextureName);
            }

			m_renderTexture = textureManager.createManual(m_renderTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, m_renderWidth, m_renderHeight, 0, Ogre::PF_FLOAT16_RGBA, Ogre::TU_RENDERTARGET);
			
			//m_renderTexture->setFSAA(4);

            // get render targets
			m_renderTarget = m_renderTexture->getBuffer()->getRenderTarget();

			// copy first camera
			m_cameraCopy = OgreTools::getFirstCamera(m_cameraSceneNode);

            // (re-)create the viewport for the render texture's render target
            m_renderTarget = m_renderTexture->getBuffer()->getRenderTarget();
            m_renderTarget->removeAllViewports();
			Ogre::Viewport *viewport = m_renderTarget->addViewport(m_cameraCopy);

			viewport->setClearEveryFrame(true);
            viewport->setBackgroundColour(OgreManager::convertColor(getColorValue(BackgroundColorParameterName)));
            viewport->setOverlaysEnabled(false);
			removeViewportsFromRenderTargets();
						
			int testWidth = m_textureMap[QString::fromStdString(m_renderTextureNormalName)]->getWidth();

			// resize all textures in m_textureMap if necessary. Attention: recreates all textures in the map
			if (m_renderWidth != m_textureMap[QString::fromStdString(m_renderTextureNormalName)]->getWidth() || m_renderHeight != m_textureMap[QString::fromStdString(m_renderTextureNormalName)]->getHeight())
				resizeAllTexturesInMap(m_renderWidth, m_renderHeight);

			initializeRenderTargets(m_cameraCopy);

			m_viewportMap[QString::fromStdString(m_renderTextureNormalName)]->setMaterialScheme("NormalPass");
			m_viewportMap[QString::fromStdString(m_renderTextureDepthName)]->setMaterialScheme("DepthPass");
			m_viewportMap[QString::fromStdString(m_renderTextureDepth3DName)]->setMaterialScheme("DepthPass3D");
			
			RenderListener *renderListener = new RenderListener(this->m_materialSwitchListener);
			Depth3DRenderListener *renderListenerDepth3D = new Depth3DRenderListener(m_materialSwitchListener, this);

			renderTargetNormal  = m_textureMap[QString::fromStdString(m_renderTextureNormalName)]->getBuffer()->getRenderTarget();
			renderTargetDepth   = m_textureMap[QString::fromStdString(m_renderTextureDepthName)]->getBuffer()->getRenderTarget();
			renderTargetDepth3D = m_textureMap[QString::fromStdString(m_renderTextureDepth3DName)]->getBuffer()->getRenderTarget();

			renderTargetNormal->addListener(renderListener);
			renderTargetDepth->addListener(renderListener);
			renderTargetDepth3D->addListener(renderListenerDepth3D);

			// update render texture and render target
			setValue(m_outputImageName, m_renderTexture);
			setValue(m_outputSecondImageName, m_textureMap[QString::fromStdString(m_renderTextureNormalName)]);
			setValue(m_outputThirdImageName,  m_textureMap[QString::fromStdString(m_renderTextureDepthName)]);
			setValue(m_outputFourthImageName,  m_textureMap[QString::fromStdString(m_renderTextureDepth3DName)]);
		}
    }
	else
		Log::error("First object attached to scene node contained in input camera parameter is not a camera.", "AdvancedRenderNode::processOutputImage");

	RenderNode::redrawTriggered();
}

///
/// Constructors and Destructors
///

//!
//! Constructor of the MaterialSwitcher class.
//!
MaterialSwitcher::MaterialSwitcher ()
{
	Ogre::MaterialPtr normMat = Ogre::MaterialPtr(Ogre::MaterialManager::getSingleton().getByName("AdvancedRender/BasicWorldNormal"));
	normMat->load();
	m_normalsTechnique = normMat->getBestTechnique();

	Ogre::MaterialPtr depthMat = Ogre::MaterialPtr(Ogre::MaterialManager::getSingleton().getByName("AdvancedRender/BasicWorldDepth"));
	depthMat->load();
	m_depthTechnique = depthMat->getBestTechnique();

	Ogre::MaterialPtr depth3DMat = Ogre::MaterialPtr(Ogre::MaterialManager::getSingleton().getByName("AdvancedRender/BasicWorldDepth3D"));
	depth3DMat->load();
	m_depth3DTechnique = depth3DMat->getBestTechnique();
}

//!
//! Destructor of the MaterialSwitcher class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
MaterialSwitcher::~MaterialSwitcher ()
{
}


///
/// Public Functions
///
	
Ogre::Technique* MaterialSwitcher::handleSchemeNotFound(unsigned short schemeIndex, 
														const Ogre::String& schemeName,
														Ogre::Material* originalMaterial, 
									                    unsigned short lodIndex,
														const Ogre::Renderable* rend)
{
		if (schemeName == "DepthPass")
            return m_depthTechnique;
		if (schemeName == "NormalPass")
			return m_normalsTechnique;
		if (schemeName == "DepthPass3D")
			return m_depth3DTechnique;

		else return NULL;
}