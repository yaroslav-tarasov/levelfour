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
//! \file "OgreManager.h"
//! \brief Implementation file for OgreManager class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "OgreManager.h"
#include "Log.h"


///
/// Private Static Data
///


//!
//! The OGRE root object.
//!
Ogre::Root* OgreManager::s_root = 0;

//!
//! The OGRE scene manager.
//!
Ogre::SceneManager* OgreManager::s_sceneManager = 0;

//!
//! Flag that states whether the OgreManager has been initialized.
//!
bool OgreManager::s_initialized = false;

//!
//! Flag that states whether next created render window should be fullscreen or not.
//!
bool OgreManager::s_fullscreen = false;


///
/// Public Static Functions
///


//!
//! Create a render window.
//!
//! \param fullscreen Fullscreen or not.
//! \param width Height of the render window.
//! \param height Width of the render window.
//! \return The created render window.
//!
Ogre::RenderWindow * OgreManager::createRenderWindow ( const Ogre::String &name /* = "OGRE Render Window" */, int width /* = 800 */, int height /* = 600 */, const Ogre::NameValuePairList *miscParams /* = 0 */ )
{
    if (!s_initialized)
        return 0;

    Ogre::RenderWindow *renderWindow = s_root->initialise(false);
    return s_root->createRenderWindow(name, width, height, s_fullscreen, miscParams);
}


//!
//! Executes the OGRE configuration dialog.
//!
void OgreManager::execConfigDialog ()
{
    if (s_initialized)
        s_root->showConfigDialog();
}


//!
//! Initializes private static data of the OgreManager
//!
void OgreManager::initialize ( const Ogre::String &pluginFileName /* = "config/plugins.cfg" */, const Ogre::String &configFileName /* = "config/ogre.cfg" */, const Ogre::String &resourceFileName /* = "config/resources.cfg" */, const Ogre::String &logFileName /* = "logs/ogre.log" */ )
{
    QString adjustedPluginFileName (pluginFileName.c_str());
#ifdef _DEBUG
    adjustedPluginFileName = adjustedPluginFileName.replace(".cfg", "_d.cfg");
#endif

    // create the OGRE system root if it doesn't exist yet
    s_root = Ogre::Root::getSingletonPtr();
    if (!s_root)
        s_root = new Ogre::Root(adjustedPluginFileName.toStdString(), configFileName, logFileName);

    // define the resource locations that will be available to OGRE
    Ogre::ConfigFile configFile;
    configFile.load(resourceFileName);
    Ogre::ResourceGroupManager *resourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
    Ogre::String sectionName, typeName, path;
    Ogre::ConfigFile::SectionIterator sectionIterator = configFile.getSectionIterator();
    while (sectionIterator.hasMoreElements()) {
        sectionName = sectionIterator.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = sectionIterator.getNext();
        for (Ogre::ConfigFile::SettingsMultiMap::iterator iter = settings->begin(); iter != settings->end(); ++iter){
            typeName = iter->first;
            path = iter->second;
            resourceGroupManager->addResourceLocation(path, typeName, sectionName);
        }
    }

    // set up the OGRE render system
    if (s_root && (s_root->restoreConfig() || s_root->showConfigDialog())) {
        // initialize the renderer, but don't create a render window yet (will be created later in ViewportWidget::createRenderWindow)
        s_root->initialise(false);
        // create the main OGRE scene manager
        s_sceneManager = s_root->createSceneManager(Ogre::ST_GENERIC, "sceneManager");
    } else
        Log::error("The OGRE render system could not be set up.", "OgreManager::initialize");

    s_initialized = s_root && s_sceneManager;
}


//!
//! Frees all resources that were used by the OgreManager.
//!
void OgreManager::finalize ()
{
    if (s_root && s_sceneManager) {
        s_root->destroySceneManager(s_sceneManager);
        s_sceneManager = 0;
    }
}


//!
//! Returns the OGRE root object.
//!
//! \return The OGRE root object.
//!
Ogre::Root* OgreManager::getRoot ()
{
    return s_root;
}


//!
//! Returns the OGRE scene manager.
//!
//! \return The OGRE scene manager.
//!
Ogre::SceneManager * OgreManager::getSceneManager ()
{
    return s_sceneManager;
}


//!
//! Creates a new scene node with the given name using the default scene
//! manager.
//!
//! A suffix ("SceneNode") will be added to the name of the scene node.
//!
//! \param name The name for the scene node.
//! \return The new scene node, or 0 if the scene node could not be created.
//!
Ogre::SceneNode * OgreManager::createSceneNode ( const QString &name )
{
    if (!s_initialized)
        return 0;

    Ogre::String sceneNodeName = QString("%1SceneNode").arg(name).toStdString();  // DELIBERATE ERROR IN THIS LINE
    if (!s_sceneManager->hasSceneNode(sceneNodeName))
        return s_sceneManager->createSceneNode(sceneNodeName);
    else {
        Ogre::StringUtil::StrStreamType message;
        message << "A scene node named \"" << sceneNodeName << "\" already exists.";
        Ogre::LogManager::getSingleton().logMessage(message.str(), Ogre::LML_CRITICAL);
        return 0;
    }
}


//!
//! Returns whether fullscreen mode is currently enabled.
//!
//! \return True if fullscreen mode is currently enabled, otherwise False.
//!
bool OgreManager::isFullscreen ()
{
    return s_fullscreen;
}


//!
//! Set Fullscreen.
//!
//! \param fullsreen Fullscreen oder not.
//!
void OgreManager::setFullscreen ( bool fullscreen )
{
    s_fullscreen = fullscreen;
}


//!
//! Returns the given Qt color value as an OGRE color value.
//!
//! \param color The Qt color value to convert.
//! \return The OGRE color value corresponding to the given Qt color value.
//!
Ogre::ColourValue OgreManager::convertColor ( const QColor &color )
{
    return Ogre::ColourValue(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}


//!
//! Unload resource using given resource manager.
//!
//! \param resMgr The resource manager.
//! \param resourceName The resource to unload.
//!
void OgreManager::unloadResource ( Ogre::ResourceManager *resMgr, const std::string &resourceName )
{
    Ogre::ResourcePtr rPtr = resMgr->getByName(resourceName);
    if (rPtr.isNull()) {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }

    rPtr->unload();
    if (rPtr->isLoaded()) {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not unloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    } else {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was unloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }

    resMgr->remove(resourceName);
    rPtr = resMgr->getByName(resourceName);
    if (rPtr.isNull()) {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was removed: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    } else {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not removed: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }

    if (resMgr->resourceExists(resourceName)) {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    } else {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}


//!
//! Load resource using given resource manager.
//!
//! \param resMgr The reource manager.
//! \param resourceName The resource to load.
//! \param resourceGroup The resource group.
//!
void OgreManager::loadResource ( Ogre::ResourceManager *resMgr, const std::string &resourceName, const std::string &resourceGroup )
{
    Ogre::ResourcePtr rPtr = resMgr->load(resourceName, resourceGroup);
    if (rPtr.isNull()) {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
        return;
    }

    rPtr->reload();
    if (rPtr->isLoaded()) {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was reloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    } else {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource was not reloaded: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }

    if (resMgr->resourceExists(resourceName)) {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource still exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    } else {
        Ogre::StringUtil::StrStreamType msg;
        msg << "Resource no longer exists: " << resourceName;
        Ogre::LogManager::getSingleton().logMessage(msg.str());
    }
}


//!
//! Unload materials in material listed file.
//!
//! \param filename The material filename.
//!
void OgreManager::unloadMaterials ( const std::string &filename )
{
    if (filename.empty()) {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }

    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if (!stream.isNull()) {
        try {
            while (!stream->eof()) {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);

                // unload materials
                if (Ogre::StringUtil::startsWith(line, "material")) {
#if (OGRE_VERSION >= 0x010700)
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
#else
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(line," \t:");
#endif                    
					bool skipFirst = true;
#if (OGRE_VERSION >= 0x010700)
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it) {
#else
					for (std::vector<Ogre::String>::iterator it = vec.begin(); it < vec.end(); ++it) {
#endif
                        if (skipFirst) {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty()) {
                            unloadResource(Ogre::MaterialManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e) {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}


//!
//! Unload vertex programs in file.
//!
//! \param filename The vertex program filename.
//!
void OgreManager::unloadVertexPrograms ( const std::string &filename )
{
    if (filename.empty()) {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }

    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if (!stream.isNull()) {
        try {
            while(!stream->eof()) {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);

                // unload vertex programs
                if (Ogre::StringUtil::startsWith(line, "vertex_program") && !Ogre::StringUtil::startsWith(line, "vertex_program_ref")) {
#if (OGRE_VERSION >= 0x010700)
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
#else
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(line," \t:");
#endif  
                    bool skipFirst = true;
#if (OGRE_VERSION >= 0x010700)
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it) {
#else
					for (std::vector<Ogre::String>::iterator it = vec.begin(); it < vec.end(); ++it) {
#endif
                        if (skipFirst) {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty()) {
                            unloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e) {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}


//!
//! Unload fragment programs file.
//!
//! \param filename The fragment program filename.
//!
void OgreManager::unloadFragmentPrograms ( const std::string &filename )
{
    if (filename.empty()) {
        Ogre::LogManager::getSingleton().logMessage("Material filename is empty.");
        return;
    }

    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
    if (!stream.isNull()) {
        try {
            while(!stream->eof()) {
                std::string line = stream->getLine();
                Ogre::StringUtil::trim(line);

                // unload fragment programs
                if (Ogre::StringUtil::startsWith(line, "fragment_program") && !Ogre::StringUtil::startsWith(line, "fragment_program_ref")) {
#if (OGRE_VERSION >= 0x010700)
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
#else
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(line," \t:");
#endif 
                    bool skipFirst = true;
#if (OGRE_VERSION >= 0x010700)
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it) {
#else
					for (std::vector<Ogre::String>::iterator it = vec.begin(); it < vec.end(); ++it) {
#endif
                        if (skipFirst) {
                            skipFirst = false;
                            continue;
                        }
                        std::string match = (*it);
                        Ogre::StringUtil::trim(match);
                        if (!match.empty()) {
                            unloadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match);
                            break;
                        }
                    }
                }
            }
        }
        catch (Ogre::Exception &e) {
            Ogre::StringUtil::StrStreamType msg;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
    stream->close();
}


//!
//! Reload material.
//!
//! \param materialName The material's name.
//! \param groupName The group name.
//! \param filename The filename.
//! \param parseMaterialScript Parse the material script or not.
//!
void OgreManager::reloadMaterial ( const std::string &materialName, const std::string &groupName, const std::string &filename, bool parseMaterialScript )
{
    if (materialName.empty()) {
        Ogre::LogManager::getSingleton().logMessage("Material name is empty.");
        return;
    }

    if (groupName.empty()) {
        Ogre::LogManager::getSingleton().logMessage("Group name is empty.");
        return;
    }

    if (filename.empty()) {
        Ogre::LogManager::getSingleton().logMessage("Filename is empty.");
        return;
    }

    unloadMaterials(filename);
    unloadVertexPrograms(filename);
    unloadFragmentPrograms(filename);

    if (parseMaterialScript) {
        Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if (!stream.isNull()) {
            try {
                Ogre::MaterialManager::getSingleton().parseScript(stream, groupName);
                Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(materialName);
                if (!materialPtr.isNull()) {
                    materialPtr->compile();
                    materialPtr->load();
                }
            }
            catch (Ogre::Exception &e) {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();

        // reload material script contents
        stream = Ogre::ResourceGroupManager::getSingleton().openResource(filename);
        if (!stream.isNull()) {
            try {
                // reload all material contents in file
                while(!stream->eof())
                {
                    std::string line = stream->getLine();
                    Ogre::StringUtil::trim(line);

                    // reload materials
                    if (Ogre::StringUtil::startsWith(line, "material")) {
#if (OGRE_VERSION >= 0x010700)
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
#else
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(line," \t:");
#endif;
                        bool skipFirst = true;
#if (OGRE_VERSION >= 0x010700)
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it) {
#else
					for (std::vector<Ogre::String>::iterator it = vec.begin(); it < vec.end(); ++it) {
#endif
                            if (skipFirst) {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty()) {
                                loadResource(Ogre::MaterialManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }

                    // reload vertex programs
                    if (Ogre::StringUtil::startsWith(line, "vertex_program") && !Ogre::StringUtil::startsWith(line, "vertex_program_ref")) {
#if (OGRE_VERSION >= 0x010700)
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
#else
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(line," \t:");
#endif
                        bool skipFirst = true;
#if (OGRE_VERSION >= 0x010700)
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it) {
#else
					for (std::vector<Ogre::String>::iterator it = vec.begin(); it < vec.end(); ++it) {
#endif
                            if (skipFirst) {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty()) {
                                loadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }

                    // reload fragment programs
                    if (Ogre::StringUtil::startsWith(line, "fragment_program") && !Ogre::StringUtil::startsWith(line, "fragment_program_ref")) {
#if (OGRE_VERSION >= 0x010700)
					Ogre::vector<Ogre::String>::type vec = Ogre::StringUtil::split(line," \t:");
#else
					std::vector<Ogre::String> vec = Ogre::StringUtil::split(line," \t:");
#endif
                        bool skipFirst = true;
#if (OGRE_VERSION >= 0x010700)
					for (Ogre::vector<Ogre::String>::type::iterator it = vec.begin(); it < vec.end(); ++it) {
#else
					for (std::vector<Ogre::String>::iterator it = vec.begin(); it < vec.end(); ++it) {
#endif
                            if (skipFirst) {
                                skipFirst = false;
                                continue;
                            }
                            std::string match = (*it);
                            Ogre::StringUtil::trim(match);
                            if (!match.empty()) {
                                loadResource(Ogre::HighLevelGpuProgramManager::getSingletonPtr(), match, groupName);
                                break;
                            }
                        }
                    }
                }
            }
            catch (Ogre::Exception &e) {
                Ogre::StringUtil::StrStreamType msg;
                msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
                Ogre::LogManager::getSingleton().logMessage(msg.str());
            }
        }
        stream->close();

        try {
            // do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
            return;
        }
        catch (Ogre::Exception &e) {
            unloadVertexPrograms(filename);

            Ogre::StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading vertex programs." << std::endl;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }

        try {
            // do a render test if it fails, leave materials unloaded
            Ogre::Root::getSingleton().renderOneFrame();
        }
        catch (Ogre::Exception &e) {
            // don't load the script this time
            reloadMaterial(materialName, groupName, filename, false);

            Ogre::StringUtil::StrStreamType msg;
            msg << "Render test failed. Unloading materials." << std::endl;
            msg << "Exception: FILE: " << __FILE__ << " LINE: " << __LINE__ << " DESC: " << e.getFullDescription() << std::endl;
            Ogre::LogManager::getSingleton().logMessage(msg.str());
        }
    }
}

