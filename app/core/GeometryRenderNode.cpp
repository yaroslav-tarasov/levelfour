/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "GeometryRenderNode.cpp"
//! \brief Implementation file for GeometryRenderNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       26.05.2009 (last updated)
//!

#include "GeometryRenderNode.h"
#include "Parameter.h"
#include "SceneNodeParameter.h"
#include "OgreManager.h"


///
/// Protected Static Constants
///


//!
//! The name of the input geometry parameter.
//!
const QString GeometryRenderNode::InputGeometryParameterName = "Geometry to Render";

//!
//! The name of the input lights parameter.
//!
const QString GeometryRenderNode::InputLightsParameterName = "Lights";

//!
//! The name of the input camera parameter.
//!
const QString GeometryRenderNode::InputCameraParameterName = "Camera";

//!
//! The name of the camera light toggle parameter.
//!
const QString GeometryRenderNode::CameraLightToggleParameterName = "Use camera light instead of input lights";

//!
//! The name of the background color parameter.
//!
const QString GeometryRenderNode::BackgroundColorParameterName = "Background Color";


///
/// Constructors and Destructors
///


//!
//! Constructor of the GeometryRenderNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
GeometryRenderNode::GeometryRenderNode ( const QString &name, ParameterGroup *parameterRoot ) :
    RenderNode(name, parameterRoot),
    m_cameraSceneNode(0),
    m_lightSceneNode(0)
{
    // create the mandatory geometry input parameter - multiplicity *
    GeometryParameter *geometryParameter = new GeometryParameter(InputGeometryParameterName);
    geometryParameter->setMultiplicity(1);
    geometryParameter->setPinType(Parameter::PT_Input);
    geometryParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(geometryParameter);
    connect(geometryParameter, SIGNAL(dirtied()), SLOT(checkViewFlag()));

    // create the mandatory light input parameter - multiplicity *
    LightParameter *lightsParameter = new LightParameter(InputLightsParameterName);
    lightsParameter->setMultiplicity(Parameter::M_OneOrMore);
    lightsParameter->setPinType(Parameter::PT_Input);
    lightsParameter->setSelfEvaluating(true);
    parameterRoot->addParameter(lightsParameter);
    connect(lightsParameter, SIGNAL(dirtied()), SLOT(checkViewFlag()));

    // create the mandatory camera input parameter - multiplicity 1
    CameraParameter *cameraParameter = new CameraParameter(InputCameraParameterName);
    cameraParameter->setPinType(Parameter::PT_Input);
    parameterRoot->addParameter(cameraParameter);
    geometryParameter->setSelfEvaluating(true);
    connect(cameraParameter, SIGNAL(dirtied()), SLOT(checkViewFlag()));

    // create the background color parameter
    Parameter *backgroundColorParameter = new Parameter(BackgroundColorParameterName, Parameter::T_Color, QColor(Qt::black));
    parameterRoot->addParameter(backgroundColorParameter, true);

    // create the camera light toggle parameter
    Parameter *cameraLightToggleParameter = new Parameter(CameraLightToggleParameterName, Parameter::T_Bool, true);
    parameterRoot->addParameter(cameraLightToggleParameter, true);

    // set up parameter affections for output image parameter
    Parameter *outputImageParameter = getParameter(m_outputImageName);
    if (outputImageParameter) {
        outputImageParameter->addAffectingParameter(geometryParameter);
        outputImageParameter->addAffectingParameter(lightsParameter);
        outputImageParameter->addAffectingParameter(cameraParameter);
        outputImageParameter->addAffectingParameter(cameraLightToggleParameter);
        outputImageParameter->addAffectingParameter(backgroundColorParameter);
    }
}


//!
//! Destructor of the GeometryRenderNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
GeometryRenderNode::~GeometryRenderNode ()
{
}
