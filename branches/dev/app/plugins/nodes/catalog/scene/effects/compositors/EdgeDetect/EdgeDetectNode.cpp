//!
//! \file "EdgeDetectNode.cpp"
//! \brief Implementation file for EdgeDetectNode class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       16.10.2009 (last updated)
//!


#include "EdgeDetectNode.h"
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
EdgeDetectNode::EdgeDetectNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "EdgeDetectCompositor", 0);

    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    addAffection("Input Map Scene", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    setChangeFunction("NPixels", SIGNAL(triggerRedraw())); 
    setChangeFunction("Threshhold", SIGNAL(triggerRedraw())); 
    
}


//!
//! Destructor of the AqtBlendNode class.
//!

//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
EdgeDetectNode::~EdgeDetectNode ()
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
void EdgeDetectNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    Ogre::TexturePtr inputTextureScene = getTextureValue("Input Map Scene");
    
    if (pass_id == 0000)
    {
        //set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);

        float Param0 = getDoubleValue("NPixels");
        float Param1 = getDoubleValue("Threshhold");
        
        setShaderParameter(fpParams, "param0", Param0);
        setShaderParameter(fpParams, "param1", Param1);
        
        //set texture name
        setTexture(mat, inputTextureScene, 0);
        
    }
}

///
/// Private Slots
///


//!
//! Processes the node's input data to generate the node's output image.
//!
void EdgeDetectNode::processOutputImage ()
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
        Log::warning("No input image connected.", "EdgeDetectNode::processOutputImage");
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
void EdgeDetectNode::redrawTriggered ()
{
	if (m_renderTexture.isNull())
		return;

    Ogre::RenderTexture *renderTarget = m_renderTexture->getBuffer()->getRenderTarget();
    if (renderTarget)
        renderTarget->update();

}
