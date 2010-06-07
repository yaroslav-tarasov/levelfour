//!
//! \file "CompositorBaseNode.cpp"
//! \brief Implementation file for CompositorBaseNode class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       16.10.2009 (last updated)
//!


#include "CompositorBaseNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"


///
/// Constructors and Destructors
///

//!
//! Constructor of the CompositorBaseNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CompositorBaseNode::CompositorBaseNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    m_resourceGroupName = QString::fromStdString(createUniqueName("ResourceGroup_RenderNode"));

    // create compositor
    if (m_viewport) {
        const QString &compositorName = getStringValue("Compositor Name");
        if (compositorName != "")
            m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, compositorName.toStdString(), 0);
    }

    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));

    // set affections for all input map parameters
    ParameterGroup *inputMapsGroup = getParameterGroup("Input Maps");
    if (inputMapsGroup) {
        QList<Parameter*> inputMapParameters = inputMapsGroup->filterParameters("", true, true);
        for (int i = 0; i < inputMapParameters.size(); ++i) {
            Parameter *parameter = inputMapParameters[i];
            std::cout << parameter->getName().toStdString() << std::endl;
            if (parameter)
                addAffection(parameter->getName(), m_outputImageName);
        }
    }

    // set change function for all shader parameters
    unsigned int pass = 0;
    ParameterGroup *shaderParameterGroup = 0;
    while (shaderParameterGroup = getParameterGroup("Shader Parameters Pass " + QString::number(pass) )) {
        if (shaderParameterGroup) {
            QList<Parameter*> shaderParameters = shaderParameterGroup->filterParameters("", true, true);
            for (int i = 0; i < shaderParameters.size(); ++i) {
                Parameter *parameter = shaderParameters[i];
                if (parameter)
                    setChangeFunction(parameter->getName(), SIGNAL(triggerRedraw()));
            }
        }
        pass++;
    }

    // load the resources
    reload();
}


//!
//! Destructor of the AqtBlendNode class.
//!

//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CompositorBaseNode::~CompositorBaseNode ()
{
}


//!
//! Protected Functions
//!

//!
//! Callback when instance of this class is registered as Ogre::CompositorListener.
//! 
//! \param pass_id Id to identifiy current compositor pass.
//! \param mat Material this pass is currently using.
//!
void CompositorBaseNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    //set shader parameters
    Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);


    // set change function for all shader parameters
    ParameterGroup *shaderParameterGroup = getParameterGroup("Shader Parameters Pass " + QString::number(pass_id));
    if (shaderParameterGroup) {
        QList<Parameter*> shaderParameters = shaderParameterGroup->filterParameters("", true, true);
        for (int i = 0; i < shaderParameters.size(); ++i) {
            Parameter *parameter = shaderParameters[i];
            if (parameter) {
                if (parameter->getType() == Parameter::T_Int)
                    setShaderParameter(fpParams, parameter->getName().toStdString(), parameter->getValue().toInt());
                else if (parameter->getType() == Parameter::T_Float)
                    setShaderParameter(fpParams, parameter->getName().toStdString(), parameter->getValue().toFloat());
            }
        }
    }

    // set the input textures to shader input
    ParameterGroup *inputMapsGroup = getParameterGroup("Input Maps");
    if (inputMapsGroup) {
        QList<Parameter*> inputMapParameters = inputMapsGroup->filterParameters("", true, true);
        for (int i = 0; i < inputMapParameters.size(); ++i) {
            Parameter *parameter = inputMapParameters[i];
            if (parameter)
                setTexture(mat, getTextureValue(parameter->getName()), i);
        }
    }
}

///
/// Protected Slots
///

///
/// Reload resources.
///
void CompositorBaseNode::reload ()
{
    // destroy compositor
    if (m_compositor) {
        m_compositor->setEnabled(false);
        Ogre::Compositor *compositor = m_compositor->getCompositor();
        if (compositor) {
            m_compositor->removeListener(this);
            Ogre::CompositorManager::getSingleton().removeCompositorChain(m_viewport);		
            m_compositor = 0;
        }
    }

	// get the resource location and reload the resources
	if (m_resourceGroupName != "") {
        
        // if resource group exists, delete it
        Ogre::StringVector resourceGroups = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
        Ogre::StringVector::iterator  iter = std::find(resourceGroups.begin(), resourceGroups.end(), m_resourceGroupName.toStdString());
        if (iter != resourceGroups.end())
            OgreTools::destroyResourceGroup(m_resourceGroupName);

        // recreate resource group
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup(m_resourceGroupName.toStdString());
        QString resourceLocation = getStringValue("Resource Location"); 
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resourceLocation.toStdString(), "FileSystem", m_resourceGroupName.toStdString());
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(m_resourceGroupName.toStdString());
        Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(m_resourceGroupName.toStdString());
	}
	
	// create the new compositor
	const QString &compositorName = getStringValue("Compositor Name");
    m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, compositorName.toStdString(), 0);
	if (m_compositor) {
		m_compositor->addListener(this);
		m_compositor->setEnabled(true);
	}

    processOutputImage();
    triggerRedraw();
}

///
/// Private Slots
///


//!
//! Processes the node's input data to generate the node's output image.
//!
void CompositorBaseNode::processOutputImage ()
{
    size_t width = 1;
    size_t height = 1;

    // resize internal render texture
    ParameterGroup *inputMapsGroup = getParameterGroup("Input Maps");
    QList<Parameter*> inputMapParameters = inputMapsGroup->filterParameters("", true, true);
    for (int i = 0; i < inputMapParameters.size(); ++i) {
        Parameter *parameter = inputMapParameters[i];
        if (parameter) {
            Ogre::TexturePtr inputTexture = getTextureValue(parameter->getName());
            if (inputTexture.isNull())
                continue;
            size_t textureWidth = inputTexture->getWidth();
            size_t textureHeight = inputTexture->getHeight();
            if (textureWidth > width && textureHeight > height) {
                width = textureWidth;
                height = textureHeight;
            }
        }   
    }

    resizeRenderTexture(width, height);

    //enable compositor (now that the input texture name was set)
    if (m_compositor)
        m_compositor->setEnabled(true);

    m_renderTexture->getBuffer()->getRenderTarget()->update();
    setValue(m_outputImageName, m_renderTexture);
}


//!
//! Redraw of ogre scene has been triggered.
//!
void CompositorBaseNode::redrawTriggered ()
{
	if (m_renderTexture.isNull())
		return;

    Ogre::RenderTexture *renderTarget = m_renderTexture->getBuffer()->getRenderTarget();
    if (renderTarget)
        renderTarget->update();
}
