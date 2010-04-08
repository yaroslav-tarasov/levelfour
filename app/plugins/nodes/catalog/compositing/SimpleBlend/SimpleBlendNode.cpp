//!
//! \file "SimpleBlendNode.cpp"
//! \brief Implementation file for SimpleBlendNode class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       16.10.2009 (last updated)
//!

#include "SimpleBlendNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"


///
/// Constructors and Destructors
///

//!
//! Constructor of the SimpleBlendNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
SimpleBlendNode::SimpleBlendNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "SimpleBlendCompositor", 0);

    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    // set affections and callback functions
    addAffection("Input Map One", m_outputImageName);
    addAffection("Input Map Two", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    setChangeFunction("Operator", SIGNAL(triggerRedraw()));
    setChangeFunction("Linear Blend Factor", SIGNAL(triggerRedraw()));
}


//!
//! Destructor of the SimpleBlendNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
SimpleBlendNode::~SimpleBlendNode ()
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
void SimpleBlendNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
	Ogre::TexturePtr inputTextureOne = getTextureValue("Input Map One");
    Ogre::TexturePtr inputTextureTwo = getTextureValue("Input Map Two");

	if (inputTextureOne.isNull() || inputTextureTwo.isNull())
		return;

    // Blend pass
    if (pass_id == 0)
    {	
	    Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        double blendFactor = getDoubleValue("Linear Blend Factor");
 
        //set shader parameters
	    setShaderParameter(fpParams, "blendFactor", (float)blendFactor);

        // obtain the operator parameter
        EnumerationParameter *operatorParameter = getEnumerationParameter("Operator");
        if (operatorParameter) {
            // get the value of the currently selected item in the enumeration
            QString value = operatorParameter->getCurrentValue();
            if (!value.isNull()) {
                if (value == "Add")
                    setShaderParameter(fpParams, "op", (Ogre::Real) 1.0);
                else if (value == "Mul")
                    setShaderParameter(fpParams, "op", (Ogre::Real) 2.0);
                else if (value == "A over B")
                    setShaderParameter(fpParams, "op", (Ogre::Real) 3.0);
				else if (value == "Stencil")
                    setShaderParameter(fpParams, "op", (Ogre::Real) 4.0);
            }
        }

        //set texture name
        setTexture(mat, inputTextureOne, 0);
        setTexture(mat, inputTextureTwo, 1);
	}
}


///
/// Private Slots
///

//!
//! Processes the node's input data to generate the node's output image.
//!
void SimpleBlendNode::processOutputImage ()
{
    // obtain input image
    Ogre::TexturePtr inputTextureOne = getTextureValue("Input Map One");
    Ogre::TexturePtr inputTextureTwo = getTextureValue("Input Map Two");

    if (inputTextureOne.isNull() && inputTextureTwo.isNull()) {
        // disable compositor (now that the input texture name was set)
        if (m_compositor)
            m_compositor->setEnabled(false);
        
        // render and set output
        m_renderTexture->getBuffer()->getRenderTarget()->update();
        setValue("Image", m_defaultTexture);
        Log::warning("Not all input images connected.", "SimpleBlendNode::processOutputImage");
    }
    else if (inputTextureTwo.isNull()) {
        setValue("Image", inputTextureOne);
        return;
    }
    else if (inputTextureOne.isNull()) {
        setValue("Image", inputTextureTwo);
        return;
    }
	else if (!m_renderTexture.isNull()) {
        // resize render texture if necessary
        m_renderWidth = inputTextureOne->getWidth();
        m_renderHeight = inputTextureOne->getHeight();
        resizeRenderTexture(m_renderWidth, m_renderHeight);

        //enable compositor (now that the input texture name was set)
        if (m_compositor)
            m_compositor->setEnabled(true);

        //render and set output
        m_renderTexture->getBuffer()->getRenderTarget()->update();
        Log::info(m_name + " rendered", "SimpleBlendNode::processOutputImage");
        setValue("Image", m_renderTexture);
    }
}

//!
//! Redraw of ogre scene has been triggered.
//!
void SimpleBlendNode::redrawTriggered ()
{
	if (m_renderTexture.isNull())
		return;

    Ogre::RenderTexture *renderTarget = m_renderTexture->getBuffer()->getRenderTarget();

    if (renderTarget)
        renderTarget->update();

}