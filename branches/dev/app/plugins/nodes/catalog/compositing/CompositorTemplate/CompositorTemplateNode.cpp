//!
//! \file "CompositorTemplateNode.cpp"
//! \brief Implementation file for CompositorTemplateNode class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       16.10.2009 (last updated)
//!


#include "CompositorTemplateNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"


///
/// Constructors and Destructors
///

//!
//! Constructor of the AqtBlendNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CompositorTemplateNode::CompositorTemplateNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "CompositorTemplateCompositor", 0);

    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    addAffection("Input Map Scene", m_outputImageName);
    addAffection("Input Map Normal", m_outputImageName);
    addAffection("Input Map Depth", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    setChangeFunction("Param 0", SIGNAL(triggerRedraw())); 
    setChangeFunction("Param 1", SIGNAL(triggerRedraw())); 
    setChangeFunction("Param 2", SIGNAL(triggerRedraw())); 
    setChangeFunction("Param 3", SIGNAL(triggerRedraw())); 
    setChangeFunction("Param 4", SIGNAL(triggerRedraw())); 
}


//!
//! Destructor of the AqtBlendNode class.
//!

//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CompositorTemplateNode::~CompositorTemplateNode ()
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
void CompositorTemplateNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    Ogre::TexturePtr inputTextureScene = getTextureValue("Input Map Scene");
    Ogre::TexturePtr inputTextureNormal = getTextureValue("Input Map Normal");
    Ogre::TexturePtr inputTextureDepth = getTextureValue("Input Map Depth");

    if (pass_id == 0000)
    {
        //set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);

        float Param0 = getDoubleValue("Param 0");
        float Param1 = getDoubleValue("Param 1");
        float Param2 = getDoubleValue("Param 2");
        float Param3 = getDoubleValue("Param 3");
        float Param4 = getDoubleValue("Param 4");

        setShaderParameter(fpParams, "param0", Param0);
        setShaderParameter(fpParams, "param1", Param1);
        setShaderParameter(fpParams, "param2", Param2);
        setShaderParameter(fpParams, "param3", Param3);
        setShaderParameter(fpParams, "param4", Param4);

        //set texture name
        setTexture(mat, inputTextureScene, 0);
        setTexture(mat, inputTextureNormal, 1);
        setTexture(mat, inputTextureDepth, 2);
    }
}

///
/// Private Slots
///


//!
//! Processes the node's input data to generate the node's output image.
//!
void CompositorTemplateNode::processOutputImage ()
{
    // obtain input image
    Ogre::TexturePtr inputTexture = getTextureValue("Input Map Scene");
    if (inputTexture.isNull()) {
        //disable compositor (now that the input texture name was set)
        if (m_compositor)
            m_compositor->setEnabled(false);

        //render and set output
        m_renderTexture->getBuffer()->getRenderTarget()->update();
        setValue("Image", m_defaultTexture);
        Log::warning("No input image connected.", "AqtFilterSobelNode::processOutputImage");
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


//!
//! Redraw of ogre scene has been triggered.
//!
void CompositorTemplateNode::redrawTriggered ()
{
	if (m_renderTexture.isNull())
		return;

    Ogre::RenderTexture *renderTarget = m_renderTexture->getBuffer()->getRenderTarget();
    if (renderTarget)
        renderTarget->update();

}
