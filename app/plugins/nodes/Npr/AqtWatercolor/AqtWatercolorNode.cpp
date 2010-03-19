/*
-----------------------------------------------------------------------------
This source file is part of the �aQtree� cooperation project between the Institute of Animation, Brainpets GbR and University of Konstanz.
Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation & University of Konstanz and brainpets GbR

www.aQtree.de

aQtree nodes function in FRAPPER, the framework application developed at the Institute of Animation.
FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper
Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "AqtWatercolorNode.cpp"
//! \brief Implementation file for AqtWatercolorNode class.
//!
//! \author     Thomas Luft <thomas.luft@web.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "AqtWatercolorNode.h"
#include "CompositorNode.h"
#include "Parameter.h"
#include "OgreTools.h"
#include "OgreManager.h"

///
/// Constructors and Destructors
///


//!
//! Constructor of the AqtWatercolorNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \param outputImageName The name of the geometry output parameter.
//!
AqtWatercolorNode::AqtWatercolorNode ( const QString &name, ParameterGroup *parameterRoot ) :
    CompositorNode(name, parameterRoot)
{
    // create compositor
    if (m_viewport)
        m_compositor = Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "WatercolorCompositor", 0);
    
    // add listener
    if (m_compositor) {
        m_compositor->addListener(this);
    }

    addAffection("Input Map", m_outputImageName);
    setProcessingFunction(m_outputImageName, SLOT(processOutputImage()));
    setChangeFunction("Noise Scale", SIGNAL(triggerRedraw()));
    setChangeFunction("Noise Intensity", SIGNAL(triggerRedraw()));
    setChangeFunction("Paper Scale", SIGNAL(triggerRedraw()));
    setChangeFunction("Paper Intensity", SIGNAL(triggerRedraw()));
    setChangeFunction("Granulation Scale", SIGNAL(triggerRedraw()));
    setChangeFunction("Granulation Intensity", SIGNAL(triggerRedraw()));
    setChangeFunction("Edge Darkening", SIGNAL(triggerRedraw()));
    setChangeFunction("Opacity", SIGNAL(triggerRedraw()));
}


//!
//! Destructor of the AqtWatercolorNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
AqtWatercolorNode::~AqtWatercolorNode ()
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
void AqtWatercolorNode::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    Ogre::TexturePtr inputTexture = getTextureValue("Input Map");
    if (inputTexture.isNull())
        return;

    // watercolor pass
    if (pass_id == 0000) {
        // set shader parameters
        Ogre::GpuProgramParametersSharedPtr fpParams = getShaderParameters(mat);
        if (!fpParams.isNull()) {
            setShaderParameter(fpParams, "pixelSize", Ogre::Vector4(1.0 / (double)getWidth(), 1.0 / (double)getHeight(), 0.0, 0.0));
            setShaderParameter(fpParams, "noiseScale", (Ogre::Real)(getDoubleValue("Noise Scale") / 20.0 + 1.0));
            setShaderParameter(fpParams, "noiseIntensity", (Ogre::Real)getDoubleValue("Noise Intensity"));
            setShaderParameter(fpParams, "paperScale", (Ogre::Real)(getDoubleValue("Paper Scale") / 20.0 + 1.0));
            setShaderParameter(fpParams, "paperIntensity", (Ogre::Real)(getDoubleValue("Paper Intensity") / 100.0));
            setShaderParameter(fpParams, "granulationScale", (Ogre::Real)(getDoubleValue("Granulation Scale") / 20.0 + 1.0));
            setShaderParameter(fpParams, "granulationIntensity", (Ogre::Real)(getDoubleValue("Granulation Intensity") / 100.0));
            setShaderParameter(fpParams, "edgeDarkening", (Ogre::Real)(getDoubleValue("Edge Darkening") / 100.0));
            setShaderParameter(fpParams, "opacity", (Ogre::Real)(getDoubleValue("Opacity") / 100.0));
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
void AqtWatercolorNode::processOutputImage ()
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


