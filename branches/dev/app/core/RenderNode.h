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
//! \file "RenderNode.h"
//! \brief Header file for RenderNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       26.05.2009 (last updated)
//!

#ifndef RENDERNODE_H
#define RENDERNODE_H

#include "ImageNode.h"

// OGRE
#include <Ogre.h>
#include "OgreTools.h"
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif


//!
//! Base class for all render nodes.
//!
class FRAPPER_CORE_EXPORT RenderNode : public ImageNode
{

    Q_OBJECT

protected: // static constants

    //!
    //! The name of the background color parameter.
    //!
    static const QString BackgroundColorParameterName;

    //!
    //! The default render width.
    //!
    static const int DefaultRenderWidth = 1280;

    //!
    //! The default render height.
    //!
    static const int DefaultRenderHeight = 720;


public: // constructors and destructors

    //!
    //! Constructor of the RenderNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    RenderNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the RenderNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~RenderNode ();

public: // methods
    
    //!
    //! Returns the width of the render target.
    //!
    //! \return The width of the render target.
    //!
    unsigned int getWidth() const;

    //!
    //! Returns the height of the render target.
    //!
    //! \return The height of the render target.
    //!
    unsigned int getHeight() const; 

public slots: //

    //!
    //! Redraw of ogre scene has been triggered.
    //!
    virtual void redrawTriggered ();

//private: // methods
//
//    //!
//    //! Creates a unique name from the given object name by adding the RenderNode
//    //! number.
//    //!
//    Ogre::String createUniqueName ( const QString &objectName );


protected: // methods

	//!
	//! Generates a Ogre RenderTexture by using the createManual(...) function.
	//!
	//! \return The unique name of the new render texture.
	//!
	Ogre::String generateTexture(const QString &name, Ogre::TextureType texType, Ogre::uint width, Ogre::uint height, int num_mips, Ogre::PixelFormat format, int usage=Ogre::TU_DEFAULT);

	//!
	//! Create a viewport for all RenderTextures (RenderTargets) in m_textureMap.
	//!
	//!	\param cam Camera to create a Ogre::Viewport.
	//!	
	void initializeRenderTargets (Ogre::Camera* cam);

	//!
	//!	Resizes all textures in m_textureMap.
	//!
	//!	\param width The width of new render texture.
	//!	\param height The height of new render texture.
	//!
	void resizeAllTexturesInMap(int width, int height);

	//!
	//! Returns one image of the m_textureMap selected by its name
	//!
	//! \return The image selected by its name.
	//!
	//Ogre::TexturePtr getImage ();


	//!
	//!	Resizes the main render texture (m_renderTexture).
	//!
	//!	\param width The width of render texture.
	//!	\param height The height of render texture.
	//!
	void resizeRenderTexture(int width, int height);

	//!
	//! Clean up all textures
	//!
	void finalizeRenderTargets();

	//!
	//! Remove all viewports from the rendertargets of all textures in the m_textureMap
	//!
	void RenderNode::removeViewportsFromRenderTargets();


protected: // data

    //!
    //! The scene manager for building the scene to render.
    //!
    Ogre::SceneManager *m_sceneManager;

    //!
    //! The viewport.
    //!
    Ogre::Viewport *m_viewport;

    //!
    //! The renderer's own camera.
    //!
    Ogre::Camera *m_camera;

    //!
    //! The renderer's render texture.
    //!
    Ogre::TexturePtr m_renderTexture;

    //!
    //! The render texture name.
    //!
    Ogre::String m_renderTextureName;

    //!
    //! The width of the image to render.
    //!
    int m_renderWidth;

    //!
    //! The height of the image to render.
    //!
    int m_renderHeight;

	//!
    //! The default texture.
    //!
    Ogre::TexturePtr m_defaultTexture;

	//! 
	//! Qt Map of Ogre textures.
	//!
	QMap<QString, Ogre::TexturePtr> m_textureMap;

	//! 
	//! Qt Map of Ogre viewports.
	//!
	QMap<QString, Ogre::Viewport*> m_viewportMap;

};


#endif
