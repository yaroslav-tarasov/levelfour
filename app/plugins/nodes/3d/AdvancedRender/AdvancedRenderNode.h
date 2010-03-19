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
//! \file "AdvancedRenderNode.h"
//! \brief Header file for AdvancedRenderNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author		Felix Bucella <felix.bucella@filmakademie.de>
//! \version    1.0
//! \date       16.10.2009 (last updated)
//!

#ifndef ADVANCEDRENDERNODE_H
#define ADVANCEDRENDERNODE_H

#include "GeometryRenderNode.h"
#include "OgreTools.h"

// OGRE
#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif

//!
//! Forward declaration for a advanced render node.
//!
class AdvancedRenderNode;

// render updates
class RenderListener : public Ogre::RenderTargetListener
{
public:
	RenderListener(Ogre::MaterialManager::Listener* materialListener);

private:
	void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

	Ogre::MaterialManager::Listener *m_materialListener;
};

//!
//! Class for providing camera parameter listener functions.
//!
class Depth3DRenderListener : public Ogre::RenderTargetListener
{
public:
	Depth3DRenderListener(Ogre::MaterialManager::Listener* materialListener, AdvancedRenderNode* node);

private:
	void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

	void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
	
protected: // data
	Ogre::MaterialManager::Listener *m_materialListener;
	AdvancedRenderNode *m_node;
};


//!
//! class to handle material switching without having to modify scene materials individually
//!
class MaterialSwitcher : public Ogre::MaterialManager::Listener
{

public: // constructors and destructors

    //!
    //! Constructor of the MaterialSwitcher class.
    //!
	MaterialSwitcher ();

    //!
    //! Destructor of the MaterialSwitcher class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~MaterialSwitcher ();

public: // methods
	
    //!
    //! Returns the width of the render target.
    //!
    //! \return The width of the render target.
    //!
	Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex,
										  const Ogre::String& schemeName,
										  Ogre::Material* originalMaterial,
					                      unsigned short lodIndex,
										  const Ogre::Renderable* rend);

protected: // data

	Ogre::Technique *m_normalsTechnique;
	Ogre::Technique *m_depthTechnique;
	Ogre::Technique *m_depth3DTechnique;
};


//!
//! Class for a basic render node.
//!
class AdvancedRenderNode : public GeometryRenderNode
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the AdvancedRenderNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    AdvancedRenderNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the AdvancedRenderNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~AdvancedRenderNode ();


public slots: //

    //!
    //! Processes the node's input data to generate the node's output image.
    //!
    void processOutputImage ();

protected: // data

    Ogre::Light *m_sceneLight;
    Ogre::Image m_defaultTextureImage;
	MaterialSwitcher *m_materialSwitchListener;

	//!
    //! The name of the second image output parameter.
    //!
    QString m_outputSecondImageName;

	//!
    //! The name of the third image output parameter.
    //!
    QString m_outputThirdImageName;

	//!
    //! The name of the  image output parameter.
    //!
    QString m_outputFourthImageName;

	Ogre::String m_renderTextureNormalName;

	Ogre::String m_renderTextureDepthName;

	Ogre::String m_renderTextureDepth3DName;

	Ogre::RenderTexture *m_renderTarget;

    //!
    //! Camera for RenderNode scene.
    //!
	Ogre::Camera *m_cameraCopy;
};

#endif