/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "ScratchedFilmNode.cpp"
//! \brief Implementation file for ScratchedFilmNode class.
//!

#include "ScratchedFilmNode.h"
#include "CompositorNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the ScratchedFilmNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \param outputImageName The name of the geometry output parameter.
//!
ScratchedFilmNode::ScratchedFilmNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "ScratchedFilmCompositor", 0);
    
    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    addAffection("Input Map", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    
	setChangeFunction("Speed1", SIGNAL(triggerRedraw()));
	setChangeFunction("Speed2", SIGNAL(triggerRedraw()));
	setChangeFunction("ScratchIntensity", SIGNAL(triggerRedraw()));
	setChangeFunction("IS", SIGNAL(triggerRedraw()));
	
}


//!
//! Destructor of the ScratchedFilmNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ScratchedFilmNode::~ScratchedFilmNode ()
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
void ScratchedFilmNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    Ogre::TexturePtr inputTexture = getTextureValue("Input Map");
    if (inputTexture.isNull())
        return;

    // watercolor pass
    if (pass_id == 0000) {
        // set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull()) {
            
			setShaderParameter(fpParams, "speed1", (Ogre::Real)(getDoubleValue("Speed1") / 100.0));
			setShaderParameter(fpParams, "speed2", (Ogre::Real)(getDoubleValue("Speed2") / 100.0));
			setShaderParameter(fpParams, "speed1", (Ogre::Real)(getDoubleValue("Speed1") / 100.0));
			setShaderParameter(fpParams, "scratchIntensity", (Ogre::Real)(getDoubleValue("ScratchIntensity") / 100.0));
			setShaderParameter(fpParams, "is", (Ogre::Real)(getDoubleValue("IS") / 100.0));
			
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
void ScratchedFilmNode::processOutputImage ()
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
        Log::warning("No input image connected.", "ScratchedFilmNode::processOutputImage");
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


