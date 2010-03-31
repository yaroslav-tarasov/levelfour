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
//! \file "AnimatableMeshNode.h"
//! \brief Implementation file for AnimatableMeshNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       29.06.2009 (last updated)
//!

#include "AnimatableMeshNode.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "SceneNodeParameter.h"
#include "FilenameParameter.h"
#include "OgreManager.h"
#include "OgreTools.h"
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTime>

INIT_INSTANCE_COUNTER(AnimatableMeshNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the AnimatableMeshNode class.
//!
//! \param name The name to give the new mesh node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
AnimatableMeshNode::AnimatableMeshNode ( const QString &name, ParameterGroup *parameterRoot ) :
    GeometryNode(name, parameterRoot, "Mesh"),
    m_sceneNode(0),
    m_entity(0),
    m_entityContainer(0),
    m_oldResourceGroupName("")
{
    // set affections and functions
    addAffection("Geometry File", m_outputGeometryName);
    addAffection("Light Description File", m_outputGeometryName);
    setChangeFunction("Geometry File", SLOT(geometryFileChanged()));
    setChangeFunction("Light Description File", SLOT(loadLightDescriptionFile()));
    setCommandFunction("Geometry File", SLOT(geometryFileChanged()));
    setCommandFunction("Light Description File", SLOT(loadLightDescriptionFile()));
    connect(this, SIGNAL(frameChanged(int)), SLOT(updateAll()));

    // create parameter groups for animation parameters
    m_animationGroup = new ParameterGroup("Skeletal Animation Parameter");
    m_poseGroup = new ParameterGroup("Pose Animation Parameter");
    m_boneGroup = new ParameterGroup("Bone Parameter");
    m_materialGroup = new ParameterGroup("Material Parameter");
    parameterRoot->addParameter(m_animationGroup);
    parameterRoot->addParameter(m_poseGroup);
    parameterRoot->addParameter(m_boneGroup);
    parameterRoot->addParameter(m_materialGroup);

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the AnimatableMeshNode class.
//!
AnimatableMeshNode::~AnimatableMeshNode ()
{
    destroyEntity();
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);
    emit viewNodeUpdated();

    DEC_INSTANCE_COUNTER
}


///
/// Public Slots
///

//!
//! Update an animation.
//!
void AnimatableMeshNode::updateAnimation ()
{
    bool isManuallyControlled = getValue("manual").toBool();
    if (isManuallyControlled)
        updateAll();
}


//!
//! Update bones.
//!
void AnimatableMeshNode::updateBones ()
{
    m_entityContainer->updateCopies();

    if (getBoolValue("manual"))
        updateAll();
}


//!
//! Update poses.
//!
void AnimatableMeshNode::updatePoses ()
{
    if (getBoolValue("manual"))
        updateAll();
}


//!
//! Update animation, bones and poses.
//!
void AnimatableMeshNode::updateAll ()
{
    // iterate over all bones in the mesh
    for (int i = 0; i < m_boneNames.size(); ++i) {
        double tx = 0.0;
        double ty = 0.0;
        double tz = 0.0;
        double rx = 0.0;
        double ry = 0.0;
        double rz = 0.0;
        double sx = 0.0;
        double sy = 0.0;
        double sz = 0.0;

        QString boneName = m_boneNames[i];
        //QVariant value = getValue(QString("AU_%1").arg(boneName));
        Parameter *parameter = getParameter(boneName);
        if (!parameter) return;
        const QVariant &value = parameter->getValue(true);
        QVariantList valueList = value.toList();
        if (valueList.size() == 6 && valueList[0].canConvert(QVariant::Double)) {
            rx += valueList[0].toDouble();
            ry += valueList[1].toDouble();
            rz += valueList[2].toDouble();
            tx += valueList[3].toDouble();
            ty += valueList[4].toDouble();
            tz += valueList[5].toDouble();
        }
        const QVariantList &parameterValueList = parameter->getValueList();
        for (unsigned int i = 1; i < parameterValueList.size(); ++i) {
            const QVariant &value = parameterValueList[i]; 
            QVariantList valueList = value.toList();
            if (valueList.size() == 6 && valueList[0].canConvert(QVariant::Double)) {
                rx += valueList[0].toDouble();
                ry += valueList[1].toDouble();
                rz += valueList[2].toDouble();
                tx += valueList[3].toDouble();
                ty += valueList[4].toDouble();
                tz += valueList[5].toDouble();
            }
        }
        transformBone(boneName, tx, ty, tz, rx, ry, rz);
    }

    for (int i = 0; i < m_animationNames.size(); ++i) {
        QString animationName = m_animationNames[i];
        Parameter *parameter = getParameter(animationName);

        // If AUs are not manually controlled nothing to do here
        double progress = 0.0;
        progress = parameter->getValue(true).toDouble();
        const QVariantList &parameterValueList = parameter->getValueList();
        for (unsigned int i = 1; i < parameterValueList.size(); ++i) {
            const QVariant &value = parameterValueList[i]; 
            progress += value.toDouble();
        }


        if (progress > 100.0)
            progress = 100.0;
        progressAnimation(animationName, progress / 100.0);
        if (getBoolValue("autoPose")) {
            progressAnimation("pose_" + animationName, progress / 100.0);
            if (m_poseNames.contains("pose_" + animationName))
                setValue("pose_" + animationName, progress);
        }
    }

    if (getBoolValue("manual") /*&& !getBoolValue("autoPose")*/)
        for (int i = 0; i < m_poseNames.size(); ++i) {
            QString poseName = m_poseNames[i];
            Parameter *parameter = getParameter(poseName);
            double progress = parameter->getValue().toDouble();
            //if (progress == 0.0)
            //    continue;
            if (progress > 100.0)
                progress = 100.0;
            progressAnimation(poseName, progress/100.0);
        }

        triggerRedraw();
}


///
/// Private Functions
///

//!
//! Initialize all animation states of this object (OGRE-specific).
//!
//! \return True if the animation states where successfully initialized, otherwise False.
//!
bool AnimatableMeshNode::initAnimationStates ()
{
    QMap<QString, Ogre::AnimationState*> animStates;
    QStringList animNames = getAnimationNames();
    for(int i = 0; i < animNames.size(); ++i) {
        QString animName = animNames[i];
        animStates[animName] = m_entity->getAnimationState(animName.toStdString());
        //NILZ: TODO: better enable animation states somewhere else
        animStates[animName]->setLoop(false);
        //animStates[animName]->setEnabled(true);
    }
    m_animStates = animStates;
    return true;
}


//!
//! Loads animation mesh.
//!
//! \return True, if successful loading of ogre mesh
//!            False, otherwise.
//!
bool AnimatableMeshNode::loadMesh ()
{
    QString filename = getStringValue("Geometry File");
    if (filename == "") {
        Log::debug(QString("Geometry file has not been set yet. (\"%1\")").arg(m_name), "AnimatableMeshNode::loadMesh");
        return false;
    }

    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "AnimatableMeshNode::loadMesh");
        return false;
    }

    // destroy an existing OGRE entity for the mesh
    destroyEntity();

    // create new scene node
    m_sceneNode = OgreManager::createSceneNode(m_name);
    if (!m_sceneNode) {
        Log::error(QString("Scene node for node \"%1\" could not be created.").arg(m_name), "AnimatableMeshNode::loadMesh");
        return false;
    }
    setValue(m_outputGeometryName, m_sceneNode, true);

    // check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Mesh file \"%1\" not found.").arg(filename), "AnimatableMeshNode::loadMesh");
        return false;
    }

    // split the absolute filename to path and base filename
    int lastSlashIndex = filename.lastIndexOf('/');
    QString path = "";
    if (lastSlashIndex > -1) {
        path = filename.mid(0, lastSlashIndex);
        filename = filename.mid(lastSlashIndex + 1);
    }
    if (!filename.endsWith(".mesh")) {
        Log::error("The geometry file has to be an OGRE mesh file.", "AnimatableMeshNode::loadMesh");
        return false;
    }

    //// destroy old resource group and generate new one
    //QString resourceGroupName = filename;
    //resourceGroupName.replace(".mesh", "");
    //destroyResourceGroup();
    //m_oldResourceGroupName = resourceGroupName;
    //createResourceGroup(resourceGroupName, path);

    // destroy old resource group and generate new one
    QString resourceGroupName = QString::fromStdString(createUniqueName("ResourceGroup_" + filename + "_AnimatableMeshNode"));
    OgreTools::destroyResourceGroup(m_oldResourceGroupName);
    m_oldResourceGroupName = resourceGroupName;
    OgreTools::createResourceGroup(resourceGroupName, path);

    // create a new OGRE entity for the mesh file
    m_entity = sceneManager->createEntity(m_name.toStdString(), filename.toStdString());
    if (m_entity) {
        // set cumulative blend mode instead of Ogre::ANIMBLEND_AVERAGE which is default
        if (m_entity->hasSkeleton()) {
            Ogre::Skeleton *skeleton = m_entity->getSkeleton();
            skeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
        }
        m_sceneNode->attachObject(m_entity);
    }

    // create a container for the entity
    m_entityContainer = new OgreContainer(m_entity);
    m_entity->setUserAny(Ogre::Any(m_entityContainer));

    updateStatistics();
    Log::info(QString("Mesh file \"%1\" loaded.").arg(filename), "AnimatableMeshNode::loadMesh");
    return true;
}


//!
//! Retrieves the numbers of vertices and triangles from the mesh and stores
//! them in parameters of this node.
//!
void AnimatableMeshNode::updateStatistics ()
{
    unsigned int numVertices = 0;
    unsigned int numTriangles = 0;

    if (m_entity) {
        const Ogre::MeshPtr &mesh = m_entity->getMesh();
        if (!mesh.isNull()) {
            bool sharedAdded = false;
            unsigned int indexCount = 0;

            for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i) {
                Ogre::SubMesh* subMesh = mesh->getSubMesh(i);
                if (subMesh->useSharedVertices) {
                    if (!sharedAdded) {
                        numVertices += (unsigned int) mesh->sharedVertexData->vertexCount;
                        sharedAdded = true;
                    }
                } else {
                    numVertices += (unsigned int) subMesh->vertexData->vertexCount;
                }

                indexCount += (unsigned int) subMesh->indexData->indexCount;
            }

            numTriangles = indexCount / 3;
        }
    }

    blockSignals(true);
    setValue("Number of Vertices", QString("%L1").arg(numVertices), true);
    setValue("Number of Triangles", QString("%L1").arg(numTriangles), true);
    blockSignals(false);
}


//!
//! Removes the OGRE entity containing the mesh geometry from the scene and
//! destroys it along with the OGRE scene node.
//!
void AnimatableMeshNode::destroyEntity ()
{
    if (m_entity) {
        // delete entity container
        if (m_entityContainer) {
            delete m_entityContainer;
            m_entityContainer = 0;
        }

        // remove the entity from the scene node it is attached to
        Ogre::SceneNode *parentSceneNode = m_entity->getParentSceneNode();
        if (parentSceneNode)
            parentSceneNode->detachObject(m_entity);

        // destroy the entity through its scene manager
        Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
        if (sceneManager) {
            sceneManager->destroyEntity(m_entity);
            m_entity = 0;
        }
    }

    if (m_sceneNode) {
        // destroy the scene node through its scene manager
        Ogre::SceneManager *sceneManager = m_sceneNode->getCreator();
        if (sceneManager) {
            sceneManager->destroySceneNode(m_sceneNode);
            m_sceneNode = 0;
            setValue(m_outputGeometryName, m_sceneNode, true);
        }
    }
}


//!
//! Adds the given time value to the animation state with the given name in
//! order to progess the animation (OGRE-specific).
//!
//! \param aUnitName The name of the animation state to progress.
//! \param timeToAdd The time to add to the animation state with the given name.
//!
void AnimatableMeshNode::progressAnimation ( const QString &aUnitName, double timeToAdd )
{
    Ogre::SkeletonInstance *skeletonInstance = m_entity->getSkeleton();
    Ogre::AnimationState *animState = m_animStates[aUnitName];
    const float mult = getDoubleValue("mult");

    if (animState) {
        float animLength = animState->getLength();
        Ogre::Real pos = mult * timeToAdd * animLength;
        animState->setEnabled(pos > 0.0);

        //// manually controlled bones might exist - reset them
        //if (skeletonInstance->hasAnimation(aUnitName.toStdString())) {
        //    Ogre::Animation *animation = skeletonInstance->getAnimation(aUnitName.toStdString());
        //    Ogre::Animation::NodeTrackIterator trackIter = animation->getNodeTrackIterator();
        //    while (trackIter.hasMoreElements()) {
        //        Ogre::NodeAnimationTrack* track = trackIter.getNext();
        //        Ogre::Bone *bone = skeletonInstance->getBone(track->getHandle());
        //        if (bone->isManuallyControlled())
        //            bone->reset();
        //    }
        //}

        animState->setTimePosition(pos);

        // update all copies created from the entity through the entity container
        if (m_entityContainer) {
            m_entityContainer->updateAnimationState(aUnitName, pos);
        }
    }
}

//!
//! Transforms a given bone.
//! 
//! \param name The name of the bone.
//! \param tx Translation x axis.
//! \param ty Translation y axis.
//! \param tz Translation z axis.
//! \param rx Rotation x axis.
//! \param ry Rotation y axis.
//! \param rz Rotation z axis.
//!
void AnimatableMeshNode::transformBone ( const QString &name, double tx, double ty, double tz, double rx, double ry, double rz )
{
    if (m_entity->hasSkeleton()) {
        Ogre::SkeletonInstance *skeletonInstance = m_entity->getSkeleton();
        if (skeletonInstance->hasBone(name.toStdString())) {
            Ogre::Bone *bone = skeletonInstance->getBone(name.toStdString());
            //Ogre::Bone *parentBone = bone->getParent();
            bone->setManuallyControlled(true);
            bone->reset();
            bone->translate(tx, ty, tz);
            bone->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(rx));
            bone->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(ry));
            bone->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(rz));
            m_entityContainer->updateCopies(name, tx, ty, tz, rx, ry, rz);
        }
    }
}

//!
//! Retrieve the names of all animations of skeleton.
//!
//! \return List with names of animations.
//!
QStringList AnimatableMeshNode::getAnimationNames ()
{
    QStringList animNames;

    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "AnimatableMeshNode::getAnimationNames");
        return animNames;
    }

    if (!sceneManager->hasEntity(m_name.toStdString()))
        return QStringList();

    Ogre::Entity *entity = sceneManager->getEntity(m_name.toStdString());

    // proceed only if entity available
    if (!entity)
        return animNames;

    Ogre::AnimationStateSet *animStateSet = m_entity->getAllAnimationStates();
    if (animStateSet) {
        Ogre::AnimationStateIterator animStatesIterator = animStateSet->getAnimationStateIterator();

        while (animStatesIterator.hasMoreElements()) {
            Ogre::AnimationState* animState = animStatesIterator.getNext();
            Ogre::String ogreAnimName = animState->getAnimationName();
            QString animName(ogreAnimName.c_str());
            animNames.append(animName);
        }
    }

    return animNames;
}


//!
//! Returns the names of all bones in the skeleton for the mesh.
//!
//! \return The names of all bones in the skeleton for the mesh.
//!
QStringList AnimatableMeshNode::getBoneNames ()
{
    QStringList result;

    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "AnimatableMeshNode::getBoneNames");
        return result;
    }

    // proceed only if entity available
    if (!sceneManager->hasEntity(m_name.toStdString()))
        return result;
    Ogre::Entity *entity = sceneManager->getEntity(m_name.toStdString());
    if (!entity || !entity->hasSkeleton())
        return result;

    Ogre::Skeleton *skeleton = entity->getSkeleton();
    unsigned short numberOfBones = skeleton->getNumBones();
    for (unsigned short i = 0; i < numberOfBones; ++i) {
        Ogre::Bone *bone = skeleton->getBone(i);
        result.append(QString(bone->getName().c_str()));
    }
    return result;
}


//!
//! Initialize a bunch of bones.
//!
//! \param boneNames The list of names of bones to initialize.
//!
void AnimatableMeshNode::initializeBones ( QStringList boneNames )
{
    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "AnimatableMeshNode::initializeBones");
        return;
    }

    // proceed only if entity and entity skeleton available
    Ogre::Entity *entity = sceneManager->getEntity(m_name.toStdString());
    if (!entity || !entity->hasSkeleton())
        return;

    Ogre::Skeleton *skeleton = entity->getSkeleton();
    for (int i = 0; i < boneNames.size(); ++i) {
        if (skeleton->hasBone(boneNames[i].toStdString())) {
            Ogre::Bone *bone = skeleton->getBone(boneNames[i].toStdString());
            if (bone) {
                bone->setManuallyControlled(true);
                //bone->setBindingPose();
            }
        }
    }
}

//!
//! Parse the Material Parameters and add to Material Parameter Group.
//!
void AnimatableMeshNode::parseMaterialParameters ()
{
    // remove old material parameters
    m_materialGroup->destroyAllParameters();

    // initialize the material number parameters
    const Ogre::MeshPtr meshPtr = m_entity->getMesh();
	if (meshPtr.isNull())
		return;

    Ogre::Mesh::SubMeshIterator subMeshIter = meshPtr->getSubMeshIterator();
    while (subMeshIter.hasMoreElements()) {
        Ogre::SubMesh* subMesh = subMeshIter.getNext();
        Ogre::String subMeshMaterialName = subMesh->getMaterialName();
        ParameterGroup *subMeshGroup = new ParameterGroup(subMeshMaterialName.c_str());
        m_materialGroup->addParameter(subMeshGroup);
        Ogre::MaterialPtr subMeshMaterial = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(subMeshMaterialName);
        if (!subMeshMaterial.isNull()) {
            Ogre::Technique* tech = subMeshMaterial->getTechnique(0);
            if (!tech)
                continue;
            Ogre::Technique::PassIterator passIter = tech->getPassIterator();
            while (passIter.hasMoreElements()) {
                Ogre::Pass* pass = passIter.getNext();
                ParameterGroup *passGroup = new ParameterGroup("Pass: " + QString(pass->getName().c_str()));
                subMeshGroup->addParameter(passGroup);
				// fragment shader
                if (pass->hasFragmentProgram()) {
                    ParameterGroup *progGroup = new ParameterGroup("FP: " + QString(pass->getFragmentProgram()->getName().c_str()));
                    passGroup->addParameter(progGroup);
                    Ogre::GpuProgramParametersSharedPtr fpParams = pass->getFragmentProgramParameters();
                    // NILZ: DEBUG:
                    const Ogre::GpuNamedConstants &namedConstants = fpParams->getConstantDefinitions();
                    const Ogre::GpuConstantDefinitionMap &fpParamMap = namedConstants.map;
                    //Ogre::GpuConstantDefinitionMap fpParamMap = fpParams->getConstantDefinitions().map;
                    Ogre::GpuConstantDefinitionMap::const_iterator fpParamIter;
					for (fpParamIter = fpParamMap.begin(); fpParamIter != fpParamMap.end(); ++fpParamIter) {
                        if	(fpParamIter->second.isFloat() && 
                            (fpParamIter->second.constType == Ogre::GpuConstantType::GCT_FLOAT1) &&
							(fpParamIter->first.find("[0]") == Ogre::String::npos)) {
                                float value = *fpParams->getFloatPointer(fpParamIter->second.physicalIndex);
                                NumberParameter *numberPara = new NumberParameter(QString(fpParamIter->first.c_str()), Parameter::Type::T_Float, value);
                                numberPara->setInputMethod(NumberParameter::IM_SliderPlusSpinBox);
                                value = pow(10.0f, (float) ceil(log10(abs(value+0.001))));
                                numberPara->setMinValue(-value);
                                numberPara->setMaxValue( value);
                                numberPara->setStepSize(value/100.0);
                                progGroup->addParameter(numberPara);
                                numberPara->setChangeFunction(SLOT(setMaterialParameter()));
                        }
                    }
                }
				// vertex shader
                if (pass->hasVertexProgram()) {
                    ParameterGroup *progGroup = new ParameterGroup("VP: " + QString(pass->getVertexProgram()->getName().c_str()));
                    passGroup->addParameter(progGroup);
                    Ogre::GpuProgramParametersSharedPtr vpParams = pass->getVertexProgramParameters();
                    Ogre::GpuConstantDefinitionMap vpParamMap = vpParams->getConstantDefinitions().map;
                    Ogre::GpuConstantDefinitionMap::iterator vpParamIter;
					for (vpParamIter = vpParamMap.begin(); vpParamIter != vpParamMap.end(); ++vpParamIter) {
                        if	(vpParamIter->second.isFloat() && 
                            (vpParamIter->second.constType == Ogre::GpuConstantType::GCT_FLOAT1) &&
							(vpParamIter->first.find("[0]") == Ogre::String::npos)) {
						    float value = *vpParams->getFloatPointer(vpParamIter->second.physicalIndex);
                            NumberParameter *numberPara = new NumberParameter(QString(vpParamIter->first.c_str()), Parameter::Type::T_Float, value);
                            numberPara->setInputMethod(NumberParameter::IM_SliderPlusSpinBox);
                            value = pow(10.0f, (float) ceil(log10(abs(value+0.001))));
                            numberPara->setMinValue(-value);
                            numberPara->setMaxValue( value);
                            numberPara->setStepSize(value/100.0);
                            progGroup->addParameter(numberPara);
                            numberPara->setChangeFunction(SLOT(setMaterialParameter()));
						}
                    }
				}
				// geometry shader
				if (pass->hasGeometryProgram()) {
					ParameterGroup *progGroup = new ParameterGroup("GP: " + QString(pass->getGeometryProgram()->getName().c_str()));
                    passGroup->addParameter(progGroup);
					Ogre::GpuProgramParametersSharedPtr gpParams = pass->getGeometryProgramParameters();
                    Ogre::GpuConstantDefinitionMap gpParamMap = gpParams->getConstantDefinitions().map;
                    Ogre::GpuConstantDefinitionMap::iterator gpParamIter;
					for (gpParamIter = gpParamMap.begin(); gpParamIter != gpParamMap.end(); ++gpParamIter) {
                        if	(gpParamIter->second.isFloat() && 
                            (gpParamIter->second.constType == Ogre::GpuConstantType::GCT_FLOAT1) &&
							(gpParamIter->first.find("[0]") == Ogre::String::npos)) {
						    float value = *gpParams->getFloatPointer(gpParamIter->second.physicalIndex);
                            NumberParameter *numberPara = new NumberParameter(QString(gpParamIter->first.c_str()), Parameter::Type::T_Float, value);
                            numberPara->setInputMethod(NumberParameter::IM_SliderPlusSpinBox);
                            value = pow(10.0f, (float) ceil(log10(abs(value+0.001))));
                            numberPara->setMinValue(-value);
                            numberPara->setMaxValue( value);
                            numberPara->setStepSize(value/100.0);
                            progGroup->addParameter(numberPara);
                            numberPara->setChangeFunction(SLOT(setMaterialParameter()));
						}
                    }
				}
            }
        }
    }

    //initializeBones(m_boneNames);
    // Redraw node - new parameters
    //notifyChange();
}


///
/// Private Slots
///

//!
//! Change function for the Geometry File parameter.
//!
void AnimatableMeshNode::geometryFileChanged ()
{
    // remove old skeletal animation parameters
    QList<Parameter *> &animationParameterList = m_animationGroup->filterParameters("", true, true);
    for (int i = 0; i < animationParameterList.size(); ++i)
        removeParameter(animationParameterList[i]);

    // remove old pose parameters
    QList<Parameter *> &poseParameterList = m_poseGroup->filterParameters("", true, true);
    for (int i = 0; i < poseParameterList.size(); ++i)
        removeParameter(poseParameterList[i]);

    // remove old bone parameters
    QList<Parameter *> &boneParameterList = m_boneGroup->filterParameters("", true, true);
    for (int i = 0; i < boneParameterList.size(); ++i)
        removeParameter(boneParameterList[i]);

    // clear lists
    m_animationNames.clear();
    m_poseNames.clear();
    m_boneNames.clear();

    // load new mesh and skeleton
    loadMesh();

    // initialize the OGRE animation states
    initAnimationStates();

    // iterate over the list of animations
    const QStringList &animationNames = getAnimationNames();
    for (int i = 0; i < animationNames.size(); ++i) {
        // create a new number parameter for the AU
        NumberParameter *animationParameter = new NumberParameter(animationNames[i], Parameter::T_Float, 0.0);
        animationParameter->setInputMethod(NumberParameter::IM_SliderPlusSpinBox);
        animationParameter->setMinValue(0.0);
        animationParameter->setMaxValue(100.0);
        animationParameter->setStepSize(1.0);
        animationParameter->setPinType(Parameter::PT_Input);
        animationParameter->setMultiplicity(Parameter::M_OneOrMore);
        //animationParameter->setSelfEvaluating(true);

        // check if the parameter is a pose parameter
        if (animationNames[i].left(5) == "pose_") {
            m_poseGroup->addParameter(animationParameter);
            animationParameter->setChangeFunction(SLOT(updatePoses()));
            m_poseNames.append(animationNames[i]);
        } else {
            m_animationGroup->addParameter(animationParameter);
            animationParameter->setChangeFunction(SLOT(updateAnimation()));
            m_animationNames.append(animationNames[i]);
        }
    }

    // initialize the list of values for the bone number parameters
    QVariantList values  = QVariantList() << 0.0 << 0.0 << 0.0 << 0.0 << 0.0 << 0.0;

    // iterate over the list of bones
    const QStringList &boneNames = getBoneNames();
    for (int i = 0; i < boneNames.size(); ++i) {
        if (!boneNames[i].contains("MarkerJoint_") ) {
            // create a new number parameter for the bone
            NumberParameter *boneParameter = new NumberParameter(boneNames[i], Parameter::T_Float, 0.0);
            boneParameter->setInputMethod(NumberParameter::IM_SliderPlusSpinBox);
            boneParameter->setMinValue(0.0);
            boneParameter->setMaxValue(100.0);
            boneParameter->setStepSize(1.0);
            boneParameter->setPinType(Parameter::PT_Input);
            boneParameter->setSize(6);
            //boneParameter->setSelfEvaluating(true);
            boneParameter->setMultiplicity(Parameter::M_OneOrMore);
            //boneParameter->setMultiplicity(values.size());
            boneParameter->setValue(QVariant(values));
            m_boneGroup->addParameter(boneParameter);
            boneParameter->setChangeFunction(SLOT(updateBones()));

            m_boneNames.append(boneNames[i]);
        }
    }

    // initialize the material number parameters
    parseMaterialParameters();
}

//!
//! Change function for the Material parameters.
//!
void AnimatableMeshNode::setMaterialParameter ()
{
    Parameter *parameter = dynamic_cast<Parameter *>(sender());
    if (!parameter)
        return;
    else {
        NumberParameter *numberParameter = static_cast<NumberParameter *>(parameter);
        const Ogre::MeshPtr meshPtr = m_entity->getMesh();

        Ogre::Mesh::SubMeshIterator subMeshIter = meshPtr->getSubMeshIterator();
        while (subMeshIter.hasMoreElements()) {
            Ogre::SubMesh* subMesh = subMeshIter.getNext();
            Ogre::String subMeshMaterialName = subMesh->getMaterialName();
            Ogre::MaterialPtr subMeshMaterial = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(subMeshMaterialName);
            if (!subMeshMaterial.isNull()) {
                Ogre::Technique* tech = subMeshMaterial->getTechnique(0);
                if (!tech)
                    continue;
                Ogre::Technique::PassIterator passIter = tech->getPassIterator();
                while (passIter.hasMoreElements()) {
                    Ogre::Pass* pass = passIter.getNext();
                    if (pass->hasFragmentProgram()) {
                        Ogre::GpuProgramParametersSharedPtr fpParams = pass->getFragmentProgramParameters();
                        if (fpParams->_findNamedConstantDefinition(numberParameter->getName().toStdString()) != 0)
                            fpParams->setNamedConstant(numberParameter->getName().toStdString(), (Ogre::Real) numberParameter->getValue().toDouble());
                    }
                    if (pass->hasVertexProgram()) {
                        Ogre::GpuProgramParametersSharedPtr vpParams = pass->getVertexProgramParameters();
                        if (vpParams->_findNamedConstantDefinition(numberParameter->getName().toStdString()) != 0)
                            vpParams->setNamedConstant(numberParameter->getName().toStdString(), (Ogre::Real) numberParameter->getValue().toDouble());
                    }
					if (pass->hasGeometryProgram()) {
						Ogre::GpuProgramParametersSharedPtr gpParams = pass->getGeometryProgramParameters();
                        if (gpParams->_findNamedConstantDefinition(numberParameter->getName().toStdString()) != 0)
                            gpParams->setNamedConstant(numberParameter->getName().toStdString(), (Ogre::Real) numberParameter->getValue().toDouble());
                    }
                }
            }
        }
    }
}


//!
//! Check existing material for BRDF shader and set the loaded light textures.
//! 
//! \return True when BRDF material was found.
//!
bool AnimatableMeshNode::setupMaterial ()
{
    bool found = false;

    const Ogre::MeshPtr meshPtr = m_entity->getMesh();

    Ogre::Mesh::SubMeshIterator subMeshIter = meshPtr->getSubMeshIterator();
    while (subMeshIter.hasMoreElements()) {
        Ogre::SubMesh* subMesh = subMeshIter.getNext();
        Ogre::String subMeshMaterialName = subMesh->getMaterialName();
        Ogre::MaterialPtr subMeshMaterial = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(subMeshMaterialName);
        if (!subMeshMaterial.isNull()) {
            if (subMeshMaterial->getTechnique("BRDF") && subMeshMaterial->getNumSupportedTechniques() > 1)
                subMeshMaterial->removeTechnique(0);
            Ogre::Material::TechniqueIterator techIter = subMeshMaterial->getTechniqueIterator();
            while (techIter.hasMoreElements()) {
                Ogre::Technique* tech = techIter.getNext();
                Ogre::Technique::PassIterator passIter = tech->getPassIterator();
                while (passIter.hasMoreElements()) {
                    Ogre::Pass* pass = passIter.getNext();
                    if (!pass->hasFragmentProgram())
                        continue;
                    Ogre::GpuProgramParametersSharedPtr fpParams = pass->getFragmentProgramParameters();
                    Ogre::Pass::TextureUnitStateIterator texUnitStateIter = pass->getTextureUnitStateIterator();
                    while (texUnitStateIter.hasMoreElements()) {
                        Ogre::TextureUnitState* texUnitState = texUnitStateIter.getNext();
                        if (texUnitState->getName() == "LightDirs") {
                            texUnitState->setTextureName(m_lightDirsTexture->getName());
                            found = true;
                        }
                        if (texUnitState->getName() == "LightPows") {
                            texUnitState->setTextureName(m_lightPowsTexture->getName());
                            found = true;
                        }
                    }
                }
            }
        }
    }

    parseMaterialParameters ();

    /*if (found)
    meshPtr->reload();*/
    return found;
}


//!
//! Loads an light description file.
//!
//! \return True when light description file has been successfully loaded.
//!
bool AnimatableMeshNode::loadLightDescriptionFile ( )
{
    QString path = getStringValue("Light Description File");
    bool result = false;
    QFile inFile(path);

    float* pDestDirs = 0;
    float* pDestPows = 0;

    Ogre::HardwarePixelBufferSharedPtr pixelBufferDirs;
    Ogre::HardwarePixelBufferSharedPtr pixelBufferPows;

    if (inFile.open(QIODevice::ReadOnly | QIODevice::Text) && m_entity) {

        if (!inFile.atEnd()) {
            QString line = inFile.readLine();
            int length = line.section(" ", 1, 1).toInt();

            if(!m_lightDirsTexture.isNull() && Ogre::TextureManager::getSingleton().resourceExists(m_lightDirsTexture->getName()))
                Ogre::TextureManager::getSingleton().remove(m_lightDirsTexture->getName());

            if(!m_lightPowsTexture.isNull() && Ogre::TextureManager::getSingleton().resourceExists(m_lightPowsTexture->getName()))
                Ogre::TextureManager::getSingleton().remove(m_lightPowsTexture->getName());

            Ogre::String lightDirsTextureName = createUniqueName("LightDirsTexture");
            Ogre::String lightPowsTextureName = createUniqueName("LightPowsTexture");

            m_lightDirsTexture = Ogre::TextureManager::getSingleton().createManual(lightDirsTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_1D, length, 1, 0, Ogre::PF_FLOAT32_RGB, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
            m_lightPowsTexture = Ogre::TextureManager::getSingleton().createManual(lightPowsTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_1D, length, 1, 0, Ogre::PF_FLOAT32_RGB, Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
        }

        if (!m_lightDirsTexture.isNull() && !m_lightDirsTexture.isNull()) {
            // Get the pixel buffer
            pixelBufferDirs = m_lightDirsTexture->getBuffer();
            pixelBufferPows = m_lightPowsTexture->getBuffer();

            // Lock the pixel buffer and get a pixel box
            pixelBufferDirs->lock(Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
            pixelBufferPows->lock(Ogre::HardwareBuffer::HBL_NORMAL); // for best performance use HBL_DISCARD!
            const Ogre::PixelBox& pixelBoxDirs = pixelBufferDirs->getCurrentLock();
            const Ogre::PixelBox& pixelBoxPows = pixelBufferPows->getCurrentLock();
            pDestDirs = static_cast<float*>(pixelBoxDirs.data);
            pDestPows = static_cast<float*>(pixelBoxPows.data);
        }

        while (!inFile.atEnd()) {
            QString line = inFile.readLine();
            QString prefix = line.section(" ", 0, 0);
            if (prefix == "Dir:") {
                *pDestDirs++ = (float) line.section(" ", 1, 1).toDouble();
                *pDestDirs++ = (float) line.section(" ", 2, 2).toDouble();
                *pDestDirs++ = (float) line.section(" ", 3, 3).toDouble();
            }
            else if (prefix == "Pow:") {
                *pDestPows++ = (float) line.section(" ", 1, 1).toDouble();
                *pDestPows++ = (float) line.section(" ", 2, 2).toDouble();
                *pDestPows++ = (float) line.section(" ", 3, 3).toDouble();
            }

        }

        // Unlock the pixel buffer
        pixelBufferDirs->unlock();
        pixelBufferPows->unlock();

        result = setupMaterial();
    } 
    else
        Log::error(QString("Document import failed. \"%1\" not found.").arg(path), "LightDescriptionNode::loadReferenceDataFile");

    inFile.close();
    return result;
}
