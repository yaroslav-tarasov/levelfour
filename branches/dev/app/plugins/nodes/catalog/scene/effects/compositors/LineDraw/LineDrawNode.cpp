/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "LineDrawNode.cpp"
//! \brief Implementation file for LineDrawNode class.
//!

#include "LineDrawNode.h"
#include "CompositorNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the LineDrawNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \param outputImageName The name of the geometry output parameter.
//!
LineDrawNode::LineDrawNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "LineDrawCompositor", 0);
    
    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    addAffection("Normal Map", m_outputImageName);
    // addAffection("Deep Map", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    
	/*
	setChangeFunction("Desat", SIGNAL(triggerRedraw()));
	setChangeFunction("Toned", SIGNAL(triggerRedraw()));
	setChangeFunction("LightColor", SIGNAL(triggerRedraw()));
	setChangeFunction("DarkColor", SIGNAL(triggerRedraw()));
	*/
}


//!
//! Destructor of the LineDrawNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
LineDrawNode::~LineDrawNode ()
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
void LineDrawNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    Ogre::TexturePtr inputTextureNormal = getTextureValue("Normal Map");
	// Ogre::TexturePtr inputTextureDeep = getTextureValue("Deep Map");
    if (inputTextureNormal.isNull())
        return;

    // watercolor pass
    if (pass_id == 0000) {
        // set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull()) {
            /*
			setShaderParameter(fpParams, "desat", (Ogre::Real)(getDoubleValue("Desat") / 100.0));
			setShaderParameter(fpParams, "toned", (Ogre::Real)(getDoubleValue("Toned") / 100.0));
			// setShaderParameter(fpParams, "speed1", (Ogre::Real)(getDoubleValue("Speed1") / 100.0));
			// setShaderParameter(fpParams, "scratchIntensity", (Ogre::Real)(getDoubleValue("ScratchIntensity") / 100.0));
			*/
			
        }

        // set texture name
        // setTexture(mat, inputTextureNormal, 0);
		// setTexture(mat, inputTextureDeep, 1);
    }

	// watercolor pass
    if (pass_id == 0001) {
        // set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull()) {
            /*
			setShaderParameter(fpParams, "desat", (Ogre::Real)(getDoubleValue("Desat") / 100.0));
			setShaderParameter(fpParams, "toned", (Ogre::Real)(getDoubleValue("Toned") / 100.0));
			// setShaderParameter(fpParams, "speed1", (Ogre::Real)(getDoubleValue("Speed1") / 100.0));
			// setShaderParameter(fpParams, "scratchIntensity", (Ogre::Real)(getDoubleValue("ScratchIntensity") / 100.0));
			*/
			
        }

        // set texture name
        // setTexture(mat, inputTextureNormal, 0);
		// setTexture(mat, inputTextureDeep, 1);
    }

	if (pass_id == 0002) {
        // set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull()) {
            /*
			setShaderParameter(fpParams, "desat", (Ogre::Real)(getDoubleValue("Desat") / 100.0));
			setShaderParameter(fpParams, "toned", (Ogre::Real)(getDoubleValue("Toned") / 100.0));
			// setShaderParameter(fpParams, "speed1", (Ogre::Real)(getDoubleValue("Speed1") / 100.0));
			// setShaderParameter(fpParams, "scratchIntensity", (Ogre::Real)(getDoubleValue("ScratchIntensity") / 100.0));
			*/
			
        }

        // set texture name
        setTexture(mat, inputTextureNormal, 0);
		// setTexture(mat, inputTextureDeep, 1);
    }
}


///
/// Private Slots
///

//!
//! Processes the node's input data to generate the node's output image.
//!
void LineDrawNode::processOutputImage ()
{
    // obtain input image
    Ogre::TexturePtr inputTextureNormal = getTextureValue("Normal Map");
	Ogre::TexturePtr inputTextureDeep = getTextureValue("Deep Map");
    if (inputTextureNormal.isNull()) {
        //disable compositor (now that the input texture name was set)
        if (m_compositor)
            m_compositor->setEnabled(false);

        //render and set output
        m_renderTexture->getBuffer()->getRenderTarget()->update();
        setValue("Image", m_defaultTexture);
        Log::warning("No input image connected.", "LineDrawNode::processOutputImage");
    }
    else if (!m_renderTexture.isNull()) {
        //resize render texture
        size_t width = inputTextureNormal->getWidth();
        size_t height = inputTextureNormal->getHeight();
        resizeRenderTexture(width, height);
        
        //enable compositor (now that the input texture name was set)
        if (m_compositor)
            m_compositor->setEnabled(true);

        m_renderTexture->getBuffer()->getRenderTarget()->update();
        setValue("Image", m_renderTexture);
    }
}


