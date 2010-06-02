/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

//!
//! \file "CompositorNode.h"
//! \brief Header file for CompositorNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       26.05.2009 (last updated)
//!

#ifndef COMPOSITORNODE_H
#define COMPOSITORNODE_H

#include "FrapperPrerequisites.h"
#include "RenderNode.h"

// OGRE
#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif


//!
//! Base class for all render nodes.
//!
class FRAPPER_CORE_EXPORT CompositorNode : public RenderNode, public Ogre::CompositorInstance::Listener
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the CompositorNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    CompositorNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the CompositorNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~CompositorNode ();

protected slots:
	
	//!
	//! Reload resources.
	//!
	virtual void reload ();

protected: // functions

    //!
    //!	Resizes the main render texture (m_renderTexture).
    //!
    //!	\param width The width of render texture.
    //!	\param height The height of render texture.
    //!
    virtual void resizeRenderTexture(int width, int height, Ogre::PixelFormat pixelFormat = Ogre::PF_FLOAT16_RGBA);

    //!
    //! Get shader parameters.
    //!
    //! \param mat The material.
    //!
    Ogre::GpuProgramParametersSharedPtr getShaderParameters( Ogre::MaterialPtr &mat );

    //!
    //! Assign texture to texture slot.
    //!
    //! \param mat The material.
    //! \param slot The texture slot id.
    //!
    void setTexture( Ogre::MaterialPtr &mat, Ogre::TexturePtr &texture, unsigned int slot );

    //!
    //! Set integer shader parameter.
    //!
    //! \param fpParames The shader parameters.
    //! \param name Name of the parameter.
    //! \param value Integer value of the parameter.
    //!
    void setShaderParameter( Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, int value );

    //!
    //! Set float shader parameter.
    //!
    //! \param fpParams The shader parameters.
    //! \param name Name of the parameter.
    //! \param value Float value of the parameter.
    //!
    void setShaderParameter( Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Real value );

    //!
    //! Set vector (3 components) shader parameter.
    //!
    //! \param fpParams The shader parameters.
    //! \param name Name of the parameter.
    //! \param vector Vector (3 components) value of the parameter.
    //!
    void setShaderParameter( Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Vector3 vector );

    //!
    //! Set vector (4 components) shader parameter.
    //!
    //! \param fpParams The shader parameters.
    //! \param name Name of the parameter.
    //! \param vector Vector (4 components) value of the parameter.
    //!
    void setShaderParameter( Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Vector4 vector );

    //!
    //! Set matrix (4x4 components) shader parameter.
    //!
    //! \param fpParams The shader parameters.
    //! \param name Name of the parameter.
    //! \param vector Matrix (4x4 components) value of the parameter.
    //!
    void setShaderParameter( Ogre::GpuProgramParametersSharedPtr fpParams, Ogre::String name, Ogre::Matrix4 matrix );


    //!
    //! Callback when instance of this class is registered as Ogre::CompositorListener.
    //! 
    //! \param pass_id Id to identifiy current compositor pass.
    //! \param mat Material this pass is currently using.
    //!
    virtual void notifyMaterialRender ( Ogre::uint32 pass_id, Ogre::MaterialPtr &mat );

protected: // data
    
    //!
    //! The compositor to use for rendering.
    //!
    Ogre::CompositorInstance *m_compositor;

	//!
    //! The resource group name.
    //!
    QString m_resourceGroupName;
};


#endif
