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
//! \file "ImageNode.cpp"
//! \brief Implementation file for ImageNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       30.06.2009 (last updated)
//!

#include "ImageNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the ImageNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \param cacheEnabled Flag to enable or disable the image cache for this node.
//! \param outputImageName The name of the geometry output parameter.
//!
ImageNode::ImageNode ( const QString &name, ParameterGroup *parameterRoot, bool cacheEnabled /* = true */, const QString &outputImageName /* = "Image" */ ) :
    ViewNode(name, parameterRoot),
    m_cacheEnabled(cacheEnabled),
    m_cacheInvalid(false),
    m_outputImageName(outputImageName),
    m_numberOfImagesToCache(NumberUnknown)
{
    if (m_cacheEnabled) {
        // create use image cache parameter
        Parameter *useImageCacheParameter = new Parameter("Use Image Cache", Parameter::T_Bool, true);

        // create cache status parameter
        Parameter *cacheStatusParameter = new Parameter("Status", Parameter::T_TextInfo, QString(""));

        // create clear image cache parameter
        Parameter *clearImageCacheParameter = new Parameter("Clear Image Cache", Parameter::T_Command, QVariant());

        // create cache parameter group
        ParameterGroup *cacheParameterGroup = new ParameterGroup("Cache");
        cacheParameterGroup->addParameter(useImageCacheParameter);
        cacheParameterGroup->addParameter(cacheStatusParameter);
        cacheParameterGroup->addParameter(clearImageCacheParameter);
        parameterRoot->addParameter(cacheParameterGroup);

        // set up functions to use for parameters
        clearImageCacheParameter->setCommandFunction(SLOT(clearImageCache()));
    }

    // add the output image parameter
    addOutputParameter(Parameter::createImageParameter(m_outputImageName));

    // initialize the cache status (if the cache is enabled)
    if (m_cacheEnabled)
        updateCacheStatus();
}


//!
//! Destructor of the ImageNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ImageNode::~ImageNode ()
{
    // obtain the output texture from the output image parameter
    Ogre::TexturePtr outputTexture = getTextureValue(m_outputImageName);

    // check if the output texture exists
    if (!outputTexture.isNull()) {
        // destroy the output texture before re-creating it
        Ogre::TextureManager::getSingletonPtr()->remove(outputTexture->getHandle());
        outputTexture.setNull();
    }

    if (m_cacheEnabled)
        clearImageCache();
}


///
/// Public Functions
///



//!
//! Returns the image that is generated by this node.
//!
//! \return The image that is generated by this node.
//!
Ogre::TexturePtr ImageNode::getImage ()
{
    Parameter *parameter = getParameter(m_outputImageName);
    if (parameter)
        parameter->propagateAuxDirty();
    return getTextureValue(m_outputImageName, true);
}


//!
//! Returns whether a cached image is available for the frame with the
//! given index.
//!
//! \param index The index of the frame for which to test whether a cached image is available.
//! \return True if a cached image is available for the frame with the given index, otherwise False.
//!
bool ImageNode::isCached ( int index ) const
{
    if (m_cacheEnabled)
        return m_imageCache.contains(index);
    else
        return false;
}


//!
//! Returns the image cached for the frame with the given index.
//!
//! \param index The index of the frame for which to return the cached image.
//! \return The cached image, or 0 if no cached image is available for the frame with the given index.
//!
Ogre::Image * ImageNode::getCachedImage ( int index ) const
{
    if (m_cacheEnabled && m_imageCache.contains(index))
        return m_imageCache[index];
    else
        return 0;
}


///
/// Public Slots
///


//!
//! Clears the image cache by freeing all memory allocated for the cache.
//!
void ImageNode::clearImageCache ()
{
    foreach (Ogre::Image *image, m_imageCache.values())
        delete image;

    m_imageCache.clear();

    updateCacheStatus();
}


///
/// Protected Functions
///


//!
//! Sets the number of images to cache to the given value.
//!
//! \param numberOfImagesToCache The number of images to cache.
//!
void ImageNode::setNumberOfImagesToCache ( unsigned int numberOfImagesToCache )
{
    m_numberOfImagesToCache = numberOfImagesToCache;
}


//!
//! Creates an image from the given texture.
//!
//! \param texturePointer The texture from which to copy the image data.
//! \return The image created from the given texture, or 0 if the given texture is invalid.
//!
Ogre::Image * ImageNode::createImageFromTexture ( Ogre::TexturePtr texturePointer )
{
    if (texturePointer.isNull()) {
        Log::error("Could not create image from texture: Invalid texture given.", "ImageNode::createImageFromTexture");
        return 0;
    }

    // obtain dimensions of given texture
    size_t width = texturePointer->getWidth();
    size_t height = texturePointer->getHeight();

    // create an empty image with the requested resolution
    Ogre::PixelFormat pixelFormat = Ogre::PF_A8R8G8B8;
    size_t memorySize = Ogre::PixelUtil::getMemorySize(width, height, 1, pixelFormat);
    Ogre::uchar *imageData = OGRE_ALLOC_T(Ogre::uchar, memorySize, Ogre::MEMCATEGORY_GENERAL);
    Ogre::Image *result = new Ogre::Image();
    result->loadDynamicImage(imageData, width, height, 1, pixelFormat, true);

    // copy the render texture's image data to the image
    texturePointer->getBuffer()->blitToMemory(result->getPixelBox());

    return result;
}


//!
//! Stores the given image in the cache for the frame with the given index.
//!
//! \param index The index of the frame for which to cache the image.
//! \param image The image to cache for the frame with the given index.
//!
void ImageNode::cacheImage ( int index, Ogre::Image *image )
{
    if (!image) {
        Log::error("Invalid image given.", "ImageNode::cacheImage");
        return;
    }

    // destroy a previously cached image
    if (m_imageCache.contains(index))
        delete m_imageCache[index];

    // store the given image in the cache
    m_imageCache[index] = image;

    updateCacheStatus();
}


//!
//! Sets the given image as the output image in the output image parameter.
//!
//! \param image The image containing the pixel data to use for the texture.
//!
void ImageNode::setOutputImage ( Ogre::Image *image )
{
    if (!image) {
        Log::error("Invalid image given.", "ImageNode::setOutputImage");
        return;
    }

    // obtain the output texture from the output image parameter
    Ogre::TexturePtr outputTexture = getTextureValue(m_outputImageName);

    // check if the output texture exists
    if (!outputTexture.isNull()) {
        // destroy the output texture before re-creating it
        Ogre::TextureManager::getSingletonPtr()->remove(outputTexture->getHandle());
        outputTexture.setNull();
    }

    // create the output texture
    Ogre::String textureName = QString("%1OutputTexture").arg(m_name).toStdString();
    Ogre::uint width = (Ogre::uint) image->getWidth();
    Ogre::uint height = (Ogre::uint) image->getHeight();
    Ogre::PixelFormat pixelFormat = image->getFormat();
    outputTexture = Ogre::TextureManager::getSingletonPtr()->createManual(textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, width, height, 0, pixelFormat, Ogre::TU_DYNAMIC);

    // load the image data into the output texture
    Ogre::TextureManager::getSingletonPtr()->setVerbose(false);
    outputTexture->loadImage(*image);
    Ogre::TextureManager::getSingletonPtr()->setVerbose(true);

    // update the output image parameter
    setValue(m_outputImageName, outputTexture);
    setDirty(m_outputImageName, false);
}


///
/// Private Functions
///


//!
//! Updates the cache status.
//!
void ImageNode::updateCacheStatus ()
{
    Parameter *cacheStatusParameter = getParameter("Cache > Status");
    if (!cacheStatusParameter) {
        Log::warning("Cache status parameter could not be obtained. The cache status will not be updated.", "ImageNode::updateCacheStatus");
        return;
    }

    unsigned int numberOfImagesCached = m_imageCache.size();
    if (numberOfImagesCached > 0) {
        // retrieve the size of the images from the first image in the cache
        unsigned int imageSize = (unsigned int) m_imageCache.values()[0]->getSize();
        double sizeOfImagesCached = numberOfImagesCached * imageSize / 1024.0 / 1024.0;
        if (m_numberOfImagesToCache == NumberUnknown) {
            cacheStatusParameter->setValue(QString("%1 images cached (%3 MB)").arg(numberOfImagesCached).arg(sizeOfImagesCached, 0, 'f', 2));
        } else {
            double sizeOfAllImages = m_numberOfImagesToCache * imageSize / 1024.0 / 1024.0;
            cacheStatusParameter->setValue(QString("%1 of %2 images cached (%3 of %4 MB)").arg(numberOfImagesCached).arg(m_numberOfImagesToCache).arg(sizeOfImagesCached, 0, 'f', 2).arg(sizeOfAllImages, 0, 'f', 2));
        }
    } else
        cacheStatusParameter->setValue("No images have been cached yet");
}
