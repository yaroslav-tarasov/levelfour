/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "CameraShakeNode.cpp"
//! \brief Implementation file for CameraShakeNode class.
//!

#include "CameraShakeNode.h"
#include "CompositorNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the CameraShakeNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \param outputImageName The name of the geometry output parameter.
//!
CameraShakeNode::CameraShakeNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "CameraShakeCompositor", 0);
    
    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    addAffection("Input Map", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    
	setChangeFunction("Speed", SIGNAL(triggerRedraw()));
	setChangeFunction("Shake", SIGNAL(triggerRedraw()));
	setChangeFunction("Sharpness", SIGNAL(triggerRedraw()));
	// setChangeFunction("DarkColor", SIGNAL(triggerRedraw()));
	
}


//!
//! Destructor of the CameraShakeNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CameraShakeNode::~CameraShakeNode ()
{
}


///
/// Protected Functions
///

//!
//! Callback when instance of this class is registered as Ogre::CompositorListener.
//! 
//! \param pass_id Id to identifiy current compositor pass.
//! \param mat Material this pass is currently using.
//!
void CameraShakeNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    Ogre::TexturePtr inputTexture = getTextureValue("Input Map");
    if (inputTexture.isNull())
        return;

    // watercolor pass
    if (pass_id == 0000) {
        // set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull()) {
            
			setShaderParameter(fpParams, "speed", (Ogre::Real)(getDoubleValue("Speed") / 100.0));
			setShaderParameter(fpParams, "shake", (Ogre::Real)(getDoubleValue("Shake") / 100.0));
			setShaderParameter(fpParams, "sharpness", (Ogre::Real)(getDoubleValue("Sharpness") / 100.0));
			// setShaderParameter(fpParams, "speed1", (Ogre::Real)(getDoubleValue("Speed1") / 100.0));
			// setShaderParameter(fpParams, "scratchIntensity", (Ogre::Real)(getDoubleValue("ScratchIntensity") / 100.0));
			
			
        }

        // set texture name
        setTexture(mat, inputTexture, 0);
    }
}


///
/// Private Slots
///

//!
//! Processes the node's input data to generate the node's output image.
//!
void CameraShakeNode::processOutputImage ()
{
    // obtain input image
    Ogre::TexturePtr inputTexture = getTextureValue("Input Map");
    if (inputTexture.isNull()) {
        //disable compositor (now that the input texture name was set)
        if (m_compositor)
            m_compositor->setEnabled(false);

        //render and set output
        m_renderTexture->getBuffer()->getRenderTarget()->update();
        setValue("Image", m_defaultTexture);
        Log::warning("No input image connected.", "CameraShakeNode::processOutputImage");
    }
    else if (!m_renderTexture.isNull()) {
        //resize render texture
        size_t width = inputTexture->getWidth();
        size_t height = inputTexture->getHeight();
        resizeRenderTexture(width, height);
        
        //enable compositor (now that the input texture name was set)
        if (m_compositor)
            m_compositor->setEnabled(true);

        m_renderTexture->getBuffer()->getRenderTarget()->update();
        setValue("Image", m_renderTexture);
    }
}


