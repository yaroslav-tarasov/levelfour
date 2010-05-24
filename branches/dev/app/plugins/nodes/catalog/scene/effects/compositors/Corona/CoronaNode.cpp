/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "CoronaNode.cpp"
//! \brief Implementation file for CoronaNode class.
//!

#include "CoronaNode.h"
#include "CompositorNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the CoronaNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \param outputImageName The name of the geometry output parameter.
//!
CoronaNode::CoronaNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "CoronaCompositor", 0);
    
    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    addAffection("Input Map", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    
	// setChangeFunction("Speed1", SIGNAL(triggerRedraw()));
	// setChangeFunction("Speed2", SIGNAL(triggerRedraw()));
	// setChangeFunction("ScratchIntensity", SIGNAL(triggerRedraw()));
	// setChangeFunction("IS", SIGNAL(triggerRedraw()));
	
}


//!
//! Destructor of the CoronaNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CoronaNode::~CoronaNode ()
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
void CoronaNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
        Ogre::TexturePtr inputTexture = getTextureValue("Input Map");
    if (inputTexture.isNull())
        return;

    //downscaled pass
    if (pass_id == 0000)
    {
        //set texture name
        setTexture(mat, inputTexture, 0);
    }

    //smooth downscaled pass
    if (pass_id == 0001)
    {
        //set gpu parameter
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull())
        {
            // double downscaleFactor = 0.125;
            // setShaderParameter(fpParams, "pixelSize", Ogre::Vector4(1.0 / (double)(downscaleFactor * getWidth()), 1.0 / (double)(downscaleFactor * getHeight()), 0.0, 0.0));
            // setShaderParameter(fpParams, "domainRadius", (Ogre::Real)(getDoubleValue("Extent") / 10.0 + 1.0));
        }
		setTexture(mat, inputTexture, 0);
    }

	if (pass_id == 0002)
    {
        //set gpu parameter
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull())
        {
            // double downscaleFactor = 0.125;
            // setShaderParameter(fpParams, "pixelSize", Ogre::Vector4(1.0 / (double)(downscaleFactor * getWidth()), 1.0 / (double)(downscaleFactor * getHeight()), 0.0, 0.0));
            // setShaderParameter(fpParams, "domainRadius", (Ogre::Real)(getDoubleValue("Extent") / 10.0 + 1.0));
        }
		setTexture(mat, inputTexture, 0);
    }

    //indication pass
    if (pass_id == 0003)
    {
        //set gpu parameter
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull())
        {
           //  setShaderParameter(fpParams, "pixelSize", Ogre::Vector4(1.0 / (double)getWidth(), 1.0 / (double)getHeight(), 0.0, 0.0));
        }

        //set texture name
        setTexture(mat, inputTexture, 0);
    }
}


///
/// Private Slots
///

//!
//! Processes the node's input data to generate the node's output image.
//!
void CoronaNode::processOutputImage ()
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
        Log::warning("No input image connected.", "CoronaNode::processOutputImage");
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


