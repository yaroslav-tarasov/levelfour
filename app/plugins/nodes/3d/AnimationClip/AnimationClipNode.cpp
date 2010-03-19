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
//! \file "AnimationClipNode.cpp"
//! \brief Implementation file for MeshNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       30.06.2009 (last updated)
//!

#include "AnimationClipNode.h"
#include "NumberParameter.h"
#include "OgreManager.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDir>

INIT_INSTANCE_COUNTER(AnimationClipNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the AnimationClipNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
AnimationClipNode::AnimationClipNode ( QString name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot),
    m_animation(0),
    m_isPlaying(false),
    m_timer(0),
    m_lastTimestamp(QTime::currentTime()),
    m_progress(0)
{
    // Define group suffixes.
    m_groupSuffixes << "translateX"
                    << "translateY"
                    << "translateZ"
                    << "rotateX"
                    << "rotateY"
                    << "rotateZ";

    // set groups
    m_animationGroup = new ParameterGroup("Skeletal Animations");
    m_boneGroup = new ParameterGroup("Bone Animations");
    getParameterRoot()->addParameter(m_animationGroup);
    getParameterRoot()->addParameter(m_boneGroup);

    // set animation timer
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), SLOT(updateTimer()));
    m_timer->start(1);

    // set affections and callback functions
    setChangeFunction("Animation Clip File", SLOT(animationClipFileChanged()));
    setCommandFunction("Animation Clip File", SLOT(triggerReload()));
    setChangeFunction("splineInterpolation", SLOT(toggleInterpolationMode()));
}



//!
//! Destructor of the AnimationClipNode class.
//!
AnimationClipNode::~AnimationClipNode ()
{
    Parameter *inputTimeParameter = getParameter("time");
    if (inputTimeParameter && m_timeParameter) {
        inputTimeParameter->removeAffectingParameter(m_timeParameter);
        m_timeParameter = 0;
    }
}

///
/// Public Methods
///

//!
//! Sets up time dependencies for node parameters.
//!
//! \param timeParameter The parameter representing the current scene time.
//!
void AnimationClipNode::setUpTimeDependencies ( Parameter *timeParameter )
{
    Parameter *inputTimeParameter = getParameter("time");
    if (inputTimeParameter && timeParameter) {
        inputTimeParameter->addAffectingParameter(timeParameter);
    }
    m_timeParameter = timeParameter;
}


///
/// Public Slots
///

//!
//! Loads the animation clip from the selected file.
//!
//! Is called when the animation clip file has been changed.
//!
void AnimationClipNode::animationClipFileChanged ()
{
    // remove old parameters
    removeParameters();
    // load animation clip from file
    loadAnimationClip();
    // generate new parameters based on animation curves from file
    generateParameters();
    // redraw node to display new parameters
    notifyChange();
}

//!
//! Slot which is called when animation parameter needs to be updated.
//!
void AnimationClipNode::processAnimationParameter()
{
    Parameter *parameter = dynamic_cast<Parameter *>(sender());
    if (!parameter)
        return;
    
    QString auName(parameter->getName());
    auName.replace("AU_", "");

    // If curve name ends with "_xxxxx" group suffix
    // add subParameters.
    QMap<QString, QMap<QString, double> >::iterator iter = m_parameterMap.find(auName);
    if ( iter != m_parameterMap.end()) {
        QMap<QString, double> &subParameterMap = iter.value();

        Parameter *timeParameter = getParameter("time");
        double length = getDoubleValue("length");

        double time = 0.0;
        if (m_timeParameter && !timeParameter->isConnected())
            time = m_timeParameter->getValue().toDouble();
        else
            time = getDoubleValue("time");

        if (getBoolValue("externallyControlled")) {
            if (time < 0.00001)
                time = 0.0;
            if (time > 100000.0)
                time = 100000.0;
            time = (float) time / 100.0 * length;
        } else {
            time = m_progress * length;
        }

        double weight = getDoubleValue("weight", true);

        if (subParameterMap.size() > 0) {
            QMap<QString, double>::iterator subIter = subParameterMap.begin();
            unsigned int index = 0;
            QVariantList subParameterList;
            while (subIter != subParameterMap.end()) {
                QString suffix = subIter.key();
                QMap<QString, unsigned short>::iterator idMapIter = m_forwCurveMap.find(auName + "_" + suffix);
                double progress = 0.0;
                // if entry exists in m_forwCurveMap interpolate progress, else progress = 0.0
                if (idMapIter != m_forwCurveMap.end()) {
                    unsigned short id = m_forwCurveMap[auName + "_" + suffix];
                    Ogre::NodeAnimationTrack* track = m_animation->getNodeTrack(id);
                    if (track) {
                        Ogre::TransformKeyFrame interpKeyFrame(NULL, time);
                        Ogre::TimeIndex timeIndex(time);
                        track->getInterpolatedKeyFrame(timeIndex, &interpKeyFrame);
                        progress = interpKeyFrame.getScale().x * weight;
                    }
                }
                subParameterList.append(QVariant(progress));
                ++index;
                ++subIter;
            }
            parameter->setValue(QVariant(subParameterList));
        } else {
            unsigned short id = m_forwCurveMap[auName];
            Ogre::NodeAnimationTrack *track = m_animation->getNodeTrack(id);
            if (track) {
                Ogre::TransformKeyFrame interpKeyFrame(NULL, time);
                Ogre::TimeIndex timeIndex(time);
                track->getInterpolatedKeyFrame(timeIndex, &interpKeyFrame);
                double progress = interpKeyFrame.getScale().x * weight;
                parameter->setValue(QVariant(progress));
            }
        }
    }
}

//!
//! Slot which is called on interpolation mode change.
//!
void AnimationClipNode::toggleInterpolationMode()
{
    // set interpolation type
    if (m_animation) {
        if (getBoolValue("splineAnimation"))
            m_animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
        else
            m_animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
    }
}

//!
//! Slot which is called on timer timeout.
//!
void AnimationClipNode::updateTimer ()
{
    bool isPlaying = getBoolValue("play", true);
    QTime currentTimestamp;

    if (isPlaying && !m_isPlaying) {
        m_isPlaying = true;
        m_progress = 0.0;
        setValue("isPlaying", true, true);
        m_lastTimestamp = QTime::currentTime();
    }

    if (m_isPlaying) {
        double scale = getDoubleValue("scale", true);
        currentTimestamp = QTime::currentTime();
        double progress = ((double) m_lastTimestamp.msecsTo(currentTimestamp)) / 1000.0;
        double length = getDoubleValue("length");
        double newProgress = m_progress + (progress * scale)/length;
        if (newProgress <= scale) {
            m_progress = newProgress;
            m_lastTimestamp = currentTimestamp;
        } else {
            bool isLooping = getValue("loop", true).toBool();
            if (!isLooping) {
                m_isPlaying = false;
                setValue("play", false);
                setValue("isPlaying", false, true);
                m_progress = 0.0;
            } else {
                m_progress = 0.0;
            }
        }
    }
}


//!
//! Trigger anim file reload.
//!
void AnimationClipNode::triggerReload ()
{
    process("Animation Clip File");
}


///
/// Protected Functions
///

//!
//! Reads all animation curves from the file with the given name.
//!
//! \param filename The name of the file with the animation curves to parse.
//!
void AnimationClipNode::parseAnimationFile ( const QString &filename )
{
    m_animCurves.clear();
    m_forwCurveMap.clear();
    m_backwCurveMap.clear();

    AnimCurve* animCurve;

    double animationLength = 0.0;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in (&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        for (int i = 0; i < line.length(); ++i)
            if (line.left(1) == " " || line.left(1) == "\t")
                line.remove(0, 1);
            else
                break;

        QRegExp sep( "( |\t)+" );
        QString anim = line.section( sep, 0, 0 );

        // look for the "anim" tag
        if (anim == "anim") {
            QString curveName = line.section( sep, 1, 1 );

            // due to naming problems in Maya
            if (curveName.left(5) == "Clip_")
                curveName = curveName.replace(0, 5, "");
            else if(curveName.left(9) == "BoneClip_")
                curveName = curveName.replace(0, 9, "");

            std::cout << line.section( sep, 1, 1 ).toStdString() << std::endl;

            animCurve = new AnimCurve();
            animCurve->name = curveName;
            animCurve->length = 0.0;
            m_animCurves[curveName] = animCurve;

            QMap<double, double> keys;

            if (!in.atEnd())
                line = in.readLine();
            else
                break;

            for (int i = 0; i < line.length(); ++i)
                if (line.left(1) == " " || line.left(1) == "\t")
                    line.remove(0, 1);
                else
                    break;

            if (line == "{") {
                while (!in.atEnd()) {
                    line = in.readLine();
                    for (int i=0; i<line.length(); ++i)
                        if (line.left(1) == " " || line.left(1) == "\t")
                            line.remove(0, 1);
                        else
                            break;

                    QString section1 = line.section(sep, 0, 0);

                    if (section1.left(1) == "#" || (section1.left(2) == "//"))
                        continue;
                    else if(section1 == "}") {
                        break;
                    }

                    QString section2 = line.section(sep, 1, 1);
                    double time = section1.toDouble();
                    double value = section2.toDouble();
                    animCurve->keys[time] = value;
                }
            }
        }
    }
}


//!
//! Generates OGRE animation curves (Ogre::NumericAnimationTrack).
//!
//! \param clipName Name of the clip.
//! \param animationLength Overall length of animation clip.
//!
void AnimationClipNode::createAnimationCurves ( QString clipName, double clipLength )
{
    if (m_animation)
        delete m_animation;

    m_animation = new Ogre::Animation(clipName.toStdString(), clipLength);
    if (m_animation) {
        if (getBoolValue("splineAnimation"))
            m_animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
        else
            m_animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
    }

    unsigned short animTrackId = 0;

    QMap<QString, AnimCurve*>::const_iterator curveIter = m_animCurves.constBegin();
    while (curveIter != m_animCurves.constEnd()) {
        //Ogre::NumericAnimationTrack *animTrack = m_animation->createNumericTrack(animTrackId);
        Ogre::NodeAnimationTrack *animTrack = m_animation->createNodeTrack(animTrackId);
        AnimCurve *tmpCurve = curveIter.value();
        m_forwCurveMap[tmpCurve->name] = animTrackId;
        m_backwCurveMap[animTrackId] = tmpCurve->name;
        QMap<double, double>::const_iterator i = (tmpCurve->keys).constBegin();
        while (i != (tmpCurve->keys).constEnd()) {
            double time = i.key();
            double value = i.value();
            // Use scalingVector to save key values in TransformKeyFrame, because
            // spline interpolation is not available in NumericKeyFrame.
            Ogre::TransformKeyFrame *keyFrame = animTrack->createNodeKeyFrame(time);
            Ogre::Vector3 valueVec (value, 0.0, 0.0);
            keyFrame->setScale(valueVec);
            //std::cout << (tmpCurve->name).toStdString() << ":" << time << ": " << value << std::endl;
            ++i;
        }
        ++animTrackId;
        ++curveIter;
        //std::cout << (tmpCurve->name).toStdString() << " : " << animTrackId << std::endl;
    }
}


//!
//! Loads animation clip.
//!
bool AnimationClipNode::loadAnimationClip ()
{
    QString filename = getStringValue("Animation Clip File");
    if (filename == "") {
        Log::debug(QString("No animation clip file has been specified. (\"%1\")").arg(m_name), "AnimationClipNode::process");
        return false;
    }

    // obtain the OGRE scene manager
    Ogre::SceneManager *sceneManager = OgreManager::getSceneManager();
    if (!sceneManager) {
        Log::error("Could not obtain OGRE scene manager.", "AnimationClipNode::loadAnimationClip");
        return false;
    }

    // check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Mesh file \"%1\" not found.").arg(filename), "AnimationClipNode::loadAnimationClip");
        return false;
    }

    // split the absolute filename to path and base filename
    int lastSlashIndex = filename.lastIndexOf('/');
    QString path = "";
    if (lastSlashIndex > -1) {
        path = filename.mid(0, lastSlashIndex);
        filename = filename.mid(lastSlashIndex + 1);
    }
    if (!filename.endsWith(".anim")) {
        Log::error("The given file has to be an OGRE anim file.", "AnimationClipNode::loadAnimationClip");
        return false;
    }

    filename = path + "/" + filename;

    // load animation file
    parseAnimationFile(filename);
    double animClipLength = 0.0;
    QMap<QString, AnimCurve *>::iterator animCurvesIter = m_animCurves.find("Length");
    if (animCurvesIter != m_animCurves.end()) {
        AnimCurve *lengthAnimCurve = animCurvesIter.value();
        if (lengthAnimCurve) {
            QMap<double, double> keyMap = lengthAnimCurve->keys;
            QMap<double, double>::iterator keyMapIterator = keyMap.begin();
            if (keyMapIterator != keyMap.end()) {
                animClipLength = keyMapIterator.key();
            }
        }
        m_animCurves.erase(animCurvesIter);
    }

    setValue("length", animClipLength);
    createAnimationCurves("AnimationClip", animClipLength);

    Log::info(QString("Animation clip file \"%1\" loaded.").arg(filename), "AnimationClipNode::loadAnimationClip");
    return true;
}


//!
//! Remove dynamic parameters.
//!
bool AnimationClipNode::removeParameters ()
{
    Parameter *inputTimeParameter = getParameter("time");
    const QList<QString>& oldCurveNames = m_forwCurveMap.keys();
    for(int i = 0; i < oldCurveNames.size(); ++i)
    {
        Parameter* parameter = getParameter("AU_" + oldCurveNames[i]);
        if(parameter) {
            // Remove old affections.
            inputTimeParameter->removeAffectedParameter(parameter);
            // Remove old parameters
            removeParameter("AU_" + oldCurveNames[i]);
        }
    }

    m_parameterMap.clear();
    Log::info("Old dynamic parameters removed.", "AnimationClipNode::removeParameters");
    return true;
}


//!
//! Generate dynamic parameters.
//!
bool AnimationClipNode::generateParameters ()
{
    // generate parameter template as QDomElement
    QDomDocument dummyDocument;
    QDomElement parameterDescription = dummyDocument.createElement("parameter");
    parameterDescription.setAttribute("inputMethod", "SliderPlusSpinBox");
    parameterDescription.setAttribute("type", "Float");
    parameterDescription.setAttribute("minValue", 0.0);
    parameterDescription.setAttribute("maxValue", 100.0);
    parameterDescription.setAttribute("stepSize", 1.0);
    parameterDescription.setAttribute("pin", "out");
    parameterDescription.setAttribute("visible", "true");

    // create a connector and parameter for each animation name
    //int indexOffset = getParameterMap().size();
    Parameter *inputTimeParameter = getParameter("time");
    Parameter *inputWeightParameter = getParameter("weight");
    Parameter *outputReadyParameter = getParameter("ready");
    ParameterGroup* parameterGroup = getParameterRoot();
    const QList<QString>& curveNames = m_forwCurveMap.keys();
    for (int i = 0; i < curveNames.size(); ++i) {
        QString curveName = curveNames[i];
        QString suffix = curveName.section("_", -1, -1);

        // if curve name ends with "_xxxxx" group suffix add subParameters
        if (m_groupSuffixes.contains(suffix)) {
            QString prefix = curveName.section("_", 0, -2);
            QMap<QString, QMap<QString, double> >::iterator iter = m_parameterMap.find(prefix);
            if ( iter != m_parameterMap.end()) {
                QMap<QString, double> &subParameterMap = iter.value();
                QMap<QString, double>::iterator subIter = subParameterMap.find(suffix);
                if (subIter != subParameterMap.end()) {
                    double &value = subIter.value();
                    value = 0.0;
                }
            } else {
                QMap<QString, double> subParameterMap;
                for (int i = 0; i < m_groupSuffixes.size(); ++i)
                    subParameterMap.insert(m_groupSuffixes[i], 0.0);
                m_parameterMap.insert(prefix, subParameterMap);
            }
        } else
            // use curve name as parameter name
            m_parameterMap.insert(curveName, QMap<QString, double>());
    }

    QMap<QString, QMap<QString, double> >::iterator iter = m_parameterMap.begin();
    while (iter != m_parameterMap.end()) {
        parameterDescription.setAttribute("name", "AU_" + iter.key());

        NumberParameter* parameter = static_cast<NumberParameter *>(Parameter::create(parameterDescription));
        if (!parameter)
            continue;

        parameter->setNode(this);

        // add affections
        inputTimeParameter->addAffectedParameter(parameter);
        inputWeightParameter->addAffectedParameter(parameter);

        QVariantList valueList;
        QMap<QString, double> subParameterMap = iter.value();
        if (subParameterMap.size() > 0) {
            QMap<QString, double>::iterator subParameterIter = subParameterMap.begin();
            while (subParameterIter != subParameterMap.end()) {
                valueList.append(0.0);
                ++subParameterIter;
            }
            m_boneGroup->addParameter(parameter);
            parameter->setMultiplicity(subParameterMap.size());
            parameter->setValue(valueList);
        } else
            m_animationGroup->addParameter(parameter);
        
        parameter->setProcessingFunction(SLOT(processAnimationParameter()));
        ++iter;
    }

    Log::info("New dynamic parameters added.", "AnimationClipNode::generateParameters");
    return true;
}
