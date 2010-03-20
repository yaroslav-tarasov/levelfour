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
//! \file "Node.cpp"
//! \brief Implementation file for Node class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       05.08.2009 (last updated)
//!

#include "Node.h"
#include "Log.h"

INIT_INSTANCE_COUNTER(Node)
Q_DECLARE_METATYPE(ParameterGroup *);
Q_DECLARE_METATYPE(Ogre::Vector3);
Q_DECLARE_METATYPE(Ogre::SceneNode *);
Q_DECLARE_METATYPE(Ogre::TexturePtr);


///
/// Constructors and Destructors
///


//!
//! Constructor of the Node class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
Node::Node ( const QString &name, ParameterGroup *parameterRoot ) :
    m_name(name),
    m_timeParameter(0),
    m_typeName(""),
    m_typeUnknown(false),
    m_evaluate(true),
    m_selected(false),
    m_selfEvaluating(false),
	m_saveable(false),
    m_parameterRoot(parameterRoot),
    m_searchText("")
{
    if (m_parameterRoot)
        // tell all parameters in the tree that this node is their parent
        m_parameterRoot->setNode(this);

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the Node class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
Node::~Node ()
{
	finalize();
    DEC_INSTANCE_COUNTER
}

void Node::finalize()
{
	if (m_parameterRoot) {
        //QList<Parameter *> parameterList = m_parameterRoot->filterParameters("", true, true);
        //for (int i = 0; i < parameterList.size(); ++i) {
        //    const Parameter *parameter = parameterList[i];
        //    if (parameter->getPinType() == Parameter::PT_Output && parameter->isConnected()) {
        //        QList<Connection *> connectionList = parameter->getConnectionMap().values();
        //        for (int j = 0; j < connectionList.size(); ++j) {
        //            Connection *connection = connectionList[j];
        //            if (!connection)
        //                continue;
        //            Parameter *targetParameter = connection->getTargetParameter();
        //            if (targetParameter)
        //                targetParameter->reset();
        //        }
        //    }
        //}
        delete m_parameterRoot;
    }
}


///
/// Public Functions
///

//########### obsolete - just for performance testing
//!
//! Processes the node's input data to generate the data for the parameter
//! with the given name.
//!
//! \param parameterName The name of the parameter whose data should be generated.
//! \return True if generating the data succeeded, otherwise False.
//!
bool Node::process ( const QString &parameterName )
{
    return true;
}

//!
//! Returns the actual timeline parameter.
//!
//! \return The actual timeline intex.
//!
Parameter *Node::getTimeParameter () const
{
    return m_timeParameter;
}

//!
//! Returns the name of the node.
//! The name must be unique in the node network so that each node object can be uniquely identified by its name.
//!
//! \return The name of the node.
//!
QString Node::getName () const
{
    return m_name;
}


//!
//! Sets the name of the node.
//! The name must be unique in the node network so that each node object can be uniquely identified by its name.
//!
//! \param name The new name for the node.
//!
void Node::setName ( const QString &name )
{
    if (m_name != name) {
        QString oldName = m_name;
        m_name = name;

        // notify connected objects that the node has changed
        emit nodeChanged();
        emit nodeNameChanged(oldName);
    }
}


//!
//! Returns the name of the node's type.
//!
//! \return The name of the node's type.
//!
QString Node::getTypeName () const
{
    return m_typeName;
}


//!
//! Set the name of the node's type.
//!
//! This function should normally only be called once directly after the
//! node has been created in the NodeFactory::createNode function.
//!
//! \param typeName The name of the node's type.
//! \see NodeFactory::createNode
//!
void Node::setTypeName ( const QString &typeName )
{
    m_typeName = typeName;
}


//!
//! Returns whether a plugin for the node's type is available.
//!
//! \return True if a plugin for the node's type is available, otherwise False.
//!
bool Node::isTypeUnknown () const
{
    return m_typeUnknown;
}


//!
//! Sets the flag that indicates whether a plugin for the node's type is
//! available.
//!
//! \param typeUnknown Flag to indicate whether a plugin for the node's type is available.
//!
void Node::setTypeUnknown ( bool typeUnknown /* = true */ )
{
    m_typeUnknown = typeUnknown;
}


//!
//! Returns whether this node should be evaluated in the network.
//!
//! \return True if this node should be evaluated in the network, otherwise False.
//!
bool Node::isEvaluated () const
{
    return m_evaluate;
}


//!
//! Returns whether this node is currently selected.
//!
//! \return True if this node is currently selected, otherwise False.
//!
bool Node::isSelected () const
{
    return m_selected;
}


//!
//! Sets whether this node is currently selected.
//!
//! \param The new selected value for the node.
//!
void Node::setSelected ( bool selected )
{
    if (m_selected != selected) {
        m_selected = selected;

        // notify connected objects that the selected state has changed
        emit selectedChanged(m_selected);
    }
}


//!
//! Returns whether the node is self-evaluating.
//!
//! \return True if the node is self-evaluating, otherwise false.
//!
bool Node::isSelfEvaluating () const
{
    return m_selfEvaluating;
}


//!
//! Returns whether the node is saveable.
//!
//! \return True if the node is saveable, otherwise false.
//!
bool Node::isSaveable () const
{
	return m_saveable;
}


//!
//! Sets whether the node is self-evaluating.
//!
//! \param selfEvaluating The new value for the self-evaluating flag.
//!
void Node::setSelfEvaluating ( bool selfEvaluating )
{
    if (m_selfEvaluating != selfEvaluating) {
        m_selfEvaluating = selfEvaluating;

        // notify connected objects that the node has changed
        emit nodeChanged();
    }
}


//!
//! Sets whether the node is saveable.
//!
//! \param saveable The new value for the saveable flag.
//!
void Node::setSaveable ( bool saveable )
{
    m_saveable = saveable;
}


//!
//! Returns the search text currently set for the node.
//!
//! \return The search text currently set for the node.
//!
QString Node::getSearchText () const
{
    return m_searchText;
}


//!
//! Sets the text to use when filtering parameters of the node before
//! creating editing widgets in a parameter editor panel.
//!
//! \param searchText The text to use for filtering parameters.
//!
void Node::setSearchText ( const QString &searchText )
{
    m_searchText = searchText;
}


///
/// Public Parameter Group Functions
///


//!
//! Returns the root of the tree of parameters and parameter groups for the
//! node.
//!
//! \return The root of the tree of parameters for the node.
//!
ParameterGroup * Node::getParameterRoot () const
{
    return m_parameterRoot;
}


//!
//! Returns the parameter group with the given name.
//!
//! \param name The name of the parameter group to return.
//! \return The parameter group with the given name.
//!
ParameterGroup * Node::getParameterGroup ( const QString &name ) const
{
    if (m_parameterRoot)
        return m_parameterRoot->getParameterGroup(name);
    else
        return 0;
}


//!
//! Iterates over the children of the parameter root and fills the given
//! lists with input and output parameters and parameter groups containing
//! input and output parameters.
//!
//! \param inputParameters The list to fill with input parameters and parameter groups containing input parameters.
//! \param outputParameters The list to fill with output parameters and parameter groups containing output parameters.
//!
void Node::fillParameterLists ( AbstractParameter::List *inputParameters, AbstractParameter::List *outputParameters )
{
    const AbstractParameter::List *parameterList = m_parameterRoot->getParameterList();
    for (int i = 0; i < parameterList->size(); ++i)
        if (parameterList->at(i)->isGroup()) {
            ParameterGroup *parameterGroup = dynamic_cast<ParameterGroup *>(parameterList->at(i));
            if (parameterGroup->contains(Parameter::PT_Input))
                inputParameters->append(parameterGroup);
            else if (parameterGroup->contains(Parameter::PT_Output))
                outputParameters->append(parameterGroup);
        } else {
            Parameter *parameter = dynamic_cast<Parameter *>(parameterList->at(i));
            if (parameter->getPinType() == Parameter::PT_Input)
                inputParameters->append(parameter);
            else if (parameter->getPinType() == Parameter::PT_Output)
                outputParameters->append(parameter);
        }
}


//!
//! Iterates over the children of the parameter root and fills the given
//! lists with connected input and output parameters and parameter groups
//! containing connected input and output parameters.
//!
//! \param connectedInputParameters The list to fill with connected input parameters and parameter groups containing connected input parameters.
//! \param connectedOutputParameters The list to fill with connected output parameters and parameter groups containing connected output parameters.
//! \param unconnectedInputParametersAvailable Flag that returns whether there are unconnected input parameters contained in the node.
//! \param unconnectedOutputParametersAvailable Flag that returns whether there are unconnected output parameters contained in the node.
//!
void Node::fillParameterLists ( AbstractParameter::List *connectedInputParameters, AbstractParameter::List *connectedOutputParameters, bool *unconnectedInputParametersAvailable, bool *unconnectedOutputParametersAvailable )
{
    *unconnectedInputParametersAvailable = false;
    *unconnectedOutputParametersAvailable = false;
    const AbstractParameter::List *parameterList = m_parameterRoot->getParameterList();
    for (int i = 0; i < parameterList->size(); ++i)
        if (parameterList->at(i)->isGroup()) {
            ParameterGroup *parameterGroup = dynamic_cast<ParameterGroup *>(parameterList->at(i));
            if (parameterGroup->contains(Parameter::PT_Input, true))
                connectedInputParameters->append(parameterGroup);
            else if (parameterGroup->contains(Parameter::PT_Output, true))
                connectedOutputParameters->append(parameterGroup);
            if (!*unconnectedInputParametersAvailable && parameterGroup->contains(Parameter::PT_Input, false))
                *unconnectedInputParametersAvailable = true;
            if (!*unconnectedOutputParametersAvailable && parameterGroup->contains(Parameter::PT_Output, false))
                *unconnectedOutputParametersAvailable = true;
        } else {
            Parameter *parameter = dynamic_cast<Parameter *>(parameterList->at(i));
            if (parameter->getPinType() == Parameter::PT_Input) {
                if (parameter->isConnected())
                    connectedInputParameters->append(parameter);
                else
                    if (!*unconnectedInputParametersAvailable)
                        *unconnectedInputParametersAvailable = true;
            } else if (parameter->getPinType() == Parameter::PT_Output)
                if (parameter->isConnected())
                    connectedOutputParameters->append(parameter);
                else
                    if (!*unconnectedOutputParametersAvailable)
                        *unconnectedOutputParametersAvailable = true;
        }
}


//!
//! Returns a list of parameters contained in the given parameter group
//! that are of the given pin type. If the respective flag is set only
//! connected parameters will be returned.
//!
//! \param pinType Only parameters of this pin type will be returned.
//! \param connectedOnly Flag that controls whether only connected parameters should be returned.
//! \param parameterGroup The root of the parameter tree to get parameters from.
//! \return A list of (connected) parameters of the given pin type contained in the given parameter group.
//!
Parameter::List Node::getParameters ( Parameter::PinType pinType, bool connectedOnly /* = false */, ParameterGroup *parameterGroup /* = 0 */ )
{
    if (parameterGroup == 0)
        parameterGroup = m_parameterRoot;

    Parameter::List result;

    const AbstractParameter::List *parameterList = parameterGroup->getParameterList();
    for (int i = 0; i < parameterList->size(); ++i) {
        AbstractParameter *abstractParameter = parameterList->at(i);
        if (abstractParameter->isGroup()) {
            // recursively get the parameters from the current parameter group
            Parameter::List parameterList = getParameters(pinType, connectedOnly, dynamic_cast<ParameterGroup *>(abstractParameter));
            // merge the obtained list with the result of this function
            for (int n = 0; n < parameterList.size(); ++n)
                result.append(parameterList[n]);
        } else {
            Parameter *parameter = dynamic_cast<Parameter *>(abstractParameter);
            if (parameter->getPinType() == pinType && (!connectedOnly || parameter->isConnected()))
                result.append(parameter);
        }
    }

    return result;
}


///
/// Public Parameter Functions
///


//!
//! Returns whether the node contains a parameter with the given name.
//!
//! \param name The name of the parameter to check for existence.
//! \return True if a parameter with the given name exists in the node, otherwise False.
//!
bool Node::hasParameter ( const QString &name ) const
{
    return m_parameterRoot->contains(name);
}


//!
//! Returns the parameter with the given name.
//!
//! \param name The name of the parameter to return.
//! \return The parameter with the given name.
//!
Parameter * Node::getParameter ( const QString &name ) const
{
    return m_parameterRoot->getParameter(name);
}


//!
//! Returns the number parameter with the given name.
//!
//! Convenience function that uses getParameter and performs type casting.
//!
//! \param name The name of the number parameter to return.
//! \return The number parameter with the given name.
//!
NumberParameter * Node::getNumberParameter ( const QString &name ) const
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        return dynamic_cast<NumberParameter *>(parameter);
    else
        return 0;
}


//!
//! Returns the filename parameter with the given name.
//!
//! Convenience function that uses getParameter and performs type casting.
//!
//! \param name The name of the filename parameter to return.
//! \return The filename parameter with the given name.
//!
FilenameParameter * Node::getFilenameParameter ( const QString &name ) const
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        return dynamic_cast<FilenameParameter *>(parameter);
    else
        return 0;
}


//!
//! Returns the enumeration parameter with the given name.
//!
//! Convenience function that uses getParameter and performs type casting.
//!
//! \param name The name of the enumeration parameter to return.
//! \return The enumeration parameter with the given name.
//!
EnumerationParameter * Node::getEnumerationParameter ( const QString &name ) const
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        return dynamic_cast<EnumerationParameter *>(parameter);
    else
        return 0;
}


//!
//! Returns the geometry parameter with the given name.
//!
//! Convenience function that uses getParameter and performs type casting.
//!
//! \param name The name of the geometry parameter to return.
//! \return The geometry parameter with the given name.
//!
GeometryParameter * Node::getGeometryParameter ( const QString &name ) const
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        return dynamic_cast<GeometryParameter *>(parameter);
    else
        return 0;
}


//!
//! Returns the light parameter with the given name.
//!
//! Convenience function that uses getParameter and performs type casting.
//!
//! \param name The name of the light parameter to return.
//! \return The light parameter with the given name.
//!
LightParameter * Node::getLightParameter ( const QString &name ) const
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        return dynamic_cast<LightParameter *>(parameter);
    else
        return 0;
}


//!
//! Returns the camera parameter with the given name.
//!
//! Convenience function that uses getParameter and performs type casting.
//!
//! \param name The name of the camera parameter to return.
//! \return The camera parameter with the given name.
//!
CameraParameter * Node::getCameraParameter ( const QString &name ) const
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        return dynamic_cast<CameraParameter *>(parameter);
    else
        return 0;
}


//!
//! Sets the dirty state of the parameter with the given name to the given
//! state.
//!
//! \param name The name of the parameter to set the dirty state for.
//! \param dirty The new value for the parameter's dirty state.
//!
void Node::setDirty ( const QString &name, bool dirty )
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        parameter->setDirty(dirty);
    else
        Log::error(QString("A parameter named \"%1\" could not be found.").arg(name), "Node::setDirty");
}

//!
//! Sets the dirty state of the node.
//!
//! \param dirty The new value for the ´node's dirty state.
//!
void Node::setDirty ( bool dirty )
{
    Parameter::List parameterList = getParameters(Parameter::PinType::PT_Output, true, getParameterRoot());
    for (int i = 0; i < parameterList.size(); ++i) {
        Parameter *parameter = (Parameter *) parameterList[i];
        parameter->setDirty(true);
    }
    m_dirty = dirty;
}

//!
//! Returns the dirty state of the node.
//!
//! \return The value for the node's dirty state.
//!
bool Node::isDirty ()
{
    return m_dirty;
}


//!
//! Removes and deletes the parameter with the given name.
//!
//! \param name The name of the parameter to remove and delete.
//!
void Node::removeParameter ( const QString &name, bool diveInGroups /*= true*/ )
{
	Parameter* parameter = m_parameterRoot->getParameter(name);
    m_parameterRoot->removeParameter(name, diveInGroups);
}


//!
//! Removes and deletes the given parameter.
//!
//! \param parameter The parameter to remove and delete.
//!
void Node::removeParameter ( Parameter *parameter, bool diveInGroups /*= true*/ )
{
	QList<Connection *> &connectionList = parameter->getConnectionMap().values();
    m_parameterRoot->removeParameter(parameter, diveInGroups);
}


//!
//! Adds a affection between two parameters.
//!
void Node::addAffection ( const QString &parameterName, const QString &affectedParameterName )
{
    Parameter *parameter = getParameter(parameterName);
    if(!parameter) {
        Log::warning(QString("Could not obtain parameter \"%1\" on node \"%2\".").arg(parameterName, getName()), "Node::addAffection");
        return;
    }
    Parameter *affectedParameter = getParameter(affectedParameterName);
    if(!affectedParameter) {
        Log::warning(QString("Could not obtain parameter \"%1\" on node \"%2\".").arg(affectedParameterName, getName()), "Node::addAffection");
        return;
    }
    // set up parameter affections
    affectedParameter->addAffectingParameter(parameter);
    affectedParameter->addAffectingParameter(parameter);
}


///
/// Public Value Getter Functions
///


//!
//! Returns the value of the parameter with the given name while optionally
//! triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
QVariant Node::getValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    if (name == "Name") {
        Log::debug("Node::getValue() was called with \"Name\" as parameter name. Consider using Node::getName() instead.", "Node::getValue");
        return QVariant(m_name);
    }

    Parameter *parameter = getParameter(name);
    if (!parameter)
        return QVariant();

    return parameter->getValue(triggerEvaluation);
}


//!
//! Convenience function for getting the value of a boolean parameter while
//! optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
bool Node::getBoolValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.type() == QVariant::Bool)
        return value.toBool();
    else {
        Log::warning(QString("Parameter \"%1\" does not contain a boolean value, but a value of type <%2>.").arg(name, QVariant::typeToName(value.type())), "Node::getBoolValue");
        return false;
    }
}


//!
//! Convenience function for getting the value of an integer parameter
//! while optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
int Node::getIntValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.type() == QVariant::Int)
        return value.toInt();
    else {
        Log::warning(QString("Parameter \"%1\" does not contain an integer value, but a value of type <%2>.").arg(name, QVariant::typeToName(value.type())), "Node::getIntValue");
        return 0;
    }
}


//!
//! Convenience function for getting the value of an unsigned integer
//! parameter while optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
unsigned int Node::getUnsignedIntValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.type() == QVariant::UInt)
        return value.toUInt();
    else {
        Log::warning(QString("Parameter \"%1\" does not contain an unsigned integer value, but a value of type <%2>.").arg(name, QVariant::typeToName(value.type())), "Node::getUnsignedIntValue");
        return 0;
    }
}


//!
//! Convenience function for getting the value of a double-precision
//! floating point parameter while optionally triggering the evaluation
//! chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
double Node::getDoubleValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.type() == QVariant::Double)
        return value.toDouble();
    else {
        Log::warning(QString("Parameter \"%1\" does not contain a double-precision floating point value, but a value of type <%2>.").arg(name, QVariant::typeToName(value.type())), "Node::getDoubleValue");
        return 0.0;
    }
}


//!
//! Convenience function for getting the value of a string parameter while
//! optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
QString Node::getStringValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.type() == QVariant::String)
        return value.toString();
    else {
        Log::warning(QString("Parameter \"%1\" does not contain a string value, but a value of type <%2>.").arg(name, QVariant::typeToName(value.type())), "Node::getStringValue");
        return QString("");
    }
}


//!
//! Convenience function for getting the value of a color parameter while
//! optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
QColor Node::getColorValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.canConvert<QColor>())
        return value.value<QColor>();
    else {
        Log::warning(QString("Could not convert value of parameter \"%1\" to a color.").arg(name), "Node::getColorValue");
        return QColor(Qt::black);
    }
}


//!
//! Convenience function for getting the value of an OGRE vector parameter
//! while optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
Ogre::Vector3 Node::getVectorValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.canConvert<Ogre::Vector3>())
        return value.value<Ogre::Vector3>();
    else {
        Log::warning(QString("Could not convert value of parameter \"%1\" to an Ogre::Vector3.").arg(name), "Node::getVectorValue");
        return Ogre::Vector3();
    }
}


//!
//! Convenience function for getting the value of a scene node parameter
//! while optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
Ogre::SceneNode * Node::getSceneNodeValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.canConvert<Ogre::SceneNode *>())
        return value.value<Ogre::SceneNode *>();
    else {
        Log::warning(QString("Could not convert value of parameter \"%1\" to an Ogre::SceneNode pointer.").arg(name), "Node::getSceneNodeValue");
        return 0;
    }
}


//!
//! Convenience function for getting the value of an OGRE texture parameter
//! while optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
Ogre::TexturePtr Node::getTextureValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.canConvert<Ogre::TexturePtr>())
        return value.value<Ogre::TexturePtr>();
    else {
        Log::warning(QString("Could not convert value of parameter \"%1\" to an Ogre::TexturePtr.").arg(name), "Node::getTextureValue");
        return Ogre::TexturePtr(0);
    }
}


//!
//! Convenience function for getting the value of a parameter group
//! parameter while optionally triggering the evaluation chain.
//!
//! \param name The name of the parameter whose value to return.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The value of the parameter with the given name.
//!
ParameterGroup * Node::getGroupValue ( const QString &name, bool triggerEvaluation /* = false */ )
{
    QVariant value = getValue(name, triggerEvaluation);
    if (value.canConvert<ParameterGroup *>())
        return value.value<ParameterGroup *>();
    else {
        Log::warning(QString("Could not convert value of parameter \"%1\" to a ParameterGroup pointer.").arg(name), "Node::getGroupValue");
        return 0;
    }
}


///
/// Public Value Setter Functions
///


//!
//! Sets the value of the parameter with the given name to the given value
//! while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The new value for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, const QVariant &value, bool triggerDirtying /* = false */ )
{
    QString parameterName = name;

    // check if the parameter name ends with an index into value list
    QRegExp indexExpression ("\\[(\\d+)\\]$");
    int index = -1;
    if (indexExpression.indexIn(name) > -1) {
        index = indexExpression.cap(1).toInt();
        parameterName = name.left(name.indexOf('['));
    }

    // obtain the parameter with the given name
    Parameter *parameter = getParameter(parameterName);
    if (parameter) {
        // set the value for the parameter
        if (index == -1)
            parameter->setValue(value);
        else
            parameter->setValue(index, value);

        // optionally trigger the dirtying chain
        if (triggerDirtying)
            parameter->propagateDirty(this);
    } else
        Log::error(QString("Parameter \"%1.%2\" not found.").arg(m_name, name), "Node::setValue");
}


//!
//! Convenience function for setting the value of a boolean parameter
//! while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, bool value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of an integer parameter
//! while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, int value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of an unsigned integer
//! parameter while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, unsigned int value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of a double-precision
//! floating point parameter while optionally triggering the dirtying
//! chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, double value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of a string parameter while
//! optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, char *value, bool triggerDirtying /* = false */ )
{
    setValue(name, QString(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of a string parameter while
//! optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, const char *value, bool triggerDirtying /* = false */ )
{
    setValue(name, QString(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of a string parameter while
//! optionally triggering the dirtying chain.
//!
//! If the parameter with the given name is not a string parameter the
//! given value will be converted according to the parameter's type.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, const QString &value, bool triggerDirtying /* = false */)
{
    // obtain the parameter with the given name
    Parameter *parameter = getParameter(name);

    // check if the parameter exists
    if (!parameter) {
        // create the parameter if the intended type for the node is unsupported
        if (m_typeUnknown) {
            Parameter *parameter = new Parameter(name, Parameter::T_String, value);
            parameter->setDefaultValue("");     // to make sure the parameter is considered when saving the scene
            parameter->setReadOnly(true);
            m_parameterRoot->addParameter(parameter);
            Log::debug(QString("The \"%1\" parameter has been automatically added to the node \"%2\".").arg(name, m_name), "Node::setValue");
        } else
            Log::error(QString("Parameter \"%1.%2\" not found.").arg(m_name, name), "Node::setValue");
        return;
    }

    // find out if the value contains a separator
    QString separator;
    if (value.contains(", "))
        separator = ", ";
    else if (value.contains(" "))
        separator = " ";

    // check the type of the parameter and try converting the given value according to the parameter's type if it's not a string-based type
    switch (parameter->getType()) {

        // character string-based parameters
        case Parameter::T_String:
        case Parameter::T_Filename:
        case Parameter::T_TextInfo:
            setValue(name, QVariant(value), triggerDirtying);
            break;

        // boolean parameters
        case Parameter::T_Bool:
            setValue(name, value == "true", triggerDirtying);
            break;

        // integer number parameters
        case Parameter::T_Int:
            if (separator.isEmpty()) {
                // convert a single value
                bool ok = true;
                int intValue = value.toInt(&ok);
                if (ok)
                    setValue(name, intValue, triggerDirtying);
                else
                    Log::error(QString("The value \"%1\" for parameter \"%2\" could not be converted to an integer number.").arg(value, name), "Node::setValue");
            } else {
                // convert a list of values
                QStringList valueParts = value.split(separator);
                bool ok = true;
                for (int i = 0; i < valueParts.size(); ++i) {
                    int intValue = valueParts[i].toInt(&ok);
                    if (ok)
                        setValue(QString("%1[%2]").arg(name).arg(i), intValue, triggerDirtying);
                    else
                        Log::error(QString("The value part \"%1\" for parameter \"%2\" could not be converted to an integer number.").arg(valueParts[i], name), "Node::setValue");
                }
            }
            break;

        // unsigned integer number parameters
        case Parameter::T_UnsignedInt:
            if (separator.isEmpty()) {
                // convert a single value
                bool ok = true;
                unsigned int unsignedIntValue = value.toUInt(&ok);
                if (ok)
                    setValue(name, unsignedIntValue, triggerDirtying);
                else
                    Log::error(QString("The value \"%1\" for parameter \"%2\" could not be converted to an unsigned integer number.").arg(value, name), "Node::setValue");
            } else {
                // convert a list of values
                QStringList valueParts = value.split(separator);
                bool ok = true;
                for (int i = 0; i < valueParts.size(); ++i) {
                    unsigned int unsignedIntValue = valueParts[i].toUInt(&ok);
                    if (ok)
                        setValue(QString("%1[%2]").arg(name).arg(i), unsignedIntValue, triggerDirtying);
                    else
                        Log::error(QString("The value part \"%1\" for parameter \"%2\" could not be converted to an unsigned integer number.").arg(valueParts[i], name), "Node::setValue");
                }
            }
            break;

        // floating-point number parameters
		case Parameter::T_Float:
            if (separator.isEmpty()) {
                // convert a single value
                bool ok = true;
                double doubleValue = value.toDouble(&ok);
                if (ok)
                    setValue(name, doubleValue, triggerDirtying);
                else
                    Log::error(QString("The value \"%1\" for parameter \"%2\" could not be converted to a double-precision floating-point number.").arg(value, name), "Node::setValue");
            } else {
                // convert a list of values
                QStringList valueParts = value.split(separator);
                bool ok = true;
                for (int i = 0; i < valueParts.size(); ++i) {
                    double doubleValue = valueParts[i].toDouble(&ok);
                    if (ok)
                        setValue(QString("%1[%2]").arg(name).arg(i), doubleValue, triggerDirtying);
                    else
                        Log::error(QString("The value part \"%1\" for parameter \"%2\" could not be converted to a double-precision floating-point number.").arg(valueParts[i], name), "Node::setValue");
                }
            }
            break;

        // color parameters
        case Parameter::T_Color:
            setValue(name, QColor(value), triggerDirtying);
            break;

        // enumeration parameters
        case Parameter::T_Enumeration:
            setValue(name, value.toInt(), triggerDirtying);
            break;

        // unsupported parameters
        default:
            Log::warning(QString("Parameter \"%1\" is not a string parameter. The value \"%2\" should be converted to a <%3>.").arg(name, value).arg(Parameter::getTypeName(parameter->getType())), "Node::setValue");
    }
}


//!
//! Convenience function for setting the value of a color parameter while
//! optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, const QColor &value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of an OGRE vector parameter
//! while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, const Ogre::Vector3 &value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant::fromValue<Ogre::Vector3>(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of a scene node parameter
//! while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, Ogre::SceneNode *value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant::fromValue<Ogre::SceneNode *>(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of an OGRE texture parameter
//! while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, const Ogre::TexturePtr value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant::fromValue<Ogre::TexturePtr>(value), triggerDirtying);
}


//!
//! Convenience function for setting the value of a parameter group
//! parameter while optionally triggering the dirtying chain.
//!
//! \param name The name of the parameter to set the value for.
//! \param value The value to set for the parameter with the given name.
//! \param triggerDirtying Flag to control whether to trigger the dirtying chain.
//!
void Node::setValue ( const QString &name, ParameterGroup *value, bool triggerDirtying /* = false */ )
{
    setValue(name, QVariant::fromValue<ParameterGroup *>(value), triggerDirtying);
}


//!
//! Convenience function for setting the enabled flag of a string parameter
//!
//! \param name The name of the parameter to set the value for.
//! \param enabled The enabled flag to set for the parameter with the given name.
//!
void Node::setParameterEnabled ( const QString &name, bool enabled )
{
    // obtain the parameter with the given name
    Parameter *parameter = getParameter(name);

	if (parameter)
		parameter->setEnabled(enabled);
	else
		Log::error(QString("Parameter \"%1.%2\" not found.").arg(m_name, name), "Node::setParameterEnabled");
}

///
/// Public Callback Setter Functions
///


//!
//! Sets the function to call when a parameter's value changes.
//!
//! Convenience function that calls setChangeFunction for the parameter
//! with the given name.
//!
//! \param name The name of the parameter to set the change function for.
//! \param changeFunction The name of the function to call when the parameter's value changes.
//!
void Node::setChangeFunction ( const QString &name, const char *changeFunction )
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        parameter->setChangeFunction(changeFunction);
    else
        Log::warning(QString("Could not set change function for parameter \"%1\": Parameter could not be found.").arg(name), "Node::setChangeFunction");
}


//!
//! Sets the function to use for computing a parameter's value.
//!
//! Convenience function that calls setProcessingFunction for the parameter
//! with the given name.
//!
//! \param name The name of the parameter to set the processing function for.
//! \param processingFunction The name of the function to use for computing the parameter's value.
//!
void Node::setProcessingFunction ( const QString &name, const char *processingFunction )
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        parameter->setProcessingFunction(processingFunction);
    else
        Log::warning(QString("Could not set processing function for parameter \"%1\": Parameter could not be found.").arg(name), "Node::setProcessingFunction");
}


//!
//! Sets the function to use for executing the command represented by a
//! parameter.
//!
//! Convenience function that calls setCommandFunction for the parameter
//! with the given name.
//!
//! \param name The name of the parameter to set the command function for.
//! \param commandFunction The function to use for executing the command represented by the parameter.
//!
void Node::setCommandFunction ( const QString &name, const char *commandFunction )
{
    Parameter *parameter = getParameter(name);
    if (parameter)
        parameter->setCommandFunction(commandFunction);
    else
        Log::warning(QString("Could not set command function for parameter \"%1\": Parameter could not be found.").arg(name), "Node::setCommandFunction");
}


///
/// Public Misc Functions
///


//!
//! Notify when something has changed in the node.
//! (Parameters added/deleted)
//!
void Node::notifyChange ()
{
    emit nodeChanged();
}


//!
//! Sets up time dependencies for node parameters.
//!
//! Can be implemented in derived classes to set up parameter affections
//! from the given time parameter to specific parameters of the node that
//! should be evaluated when the scene time represented by the given time
//! parameter changes.
//!
//! The default implementation does nothing.
//!
//! \param timeParameter The parameter representing the current scene time.
//! \param slot The slot which should be triggered when frame has changed
//!
void Node::setUpTimeDependencies ( Parameter *timeParameter )
{
    m_timeParameter = timeParameter;
}


//!
//! Evaluates the given new connection for the node.
//!
//! \param connection The new connection to evaluate.
//!
void Node::evaluateConnection ( Connection *connection )
{
    // noop
}


//!
//! Signal that is emitted in order to delete a connection (through SceneModel)
//!
//! \param name The name of the object.
//!
void Node::deleteConnection ( Connection *connection)
{
	emit sendDeleteConnection(connection);
}

///
/// Public Slots
///


//!
//! Sets whether this node should be evaluated in the network.
//!
//! \param evaluate The new value for eval flag of this node.
//!
void Node::setEvaluate ( bool evaluate )
{
    if (m_evaluate != evaluate) {
        m_evaluate = evaluate;

        // notify connected objects that the node has changed
        emit nodeChanged();
    }
}

///
/// Protected Methods
///

//!
//! Creates a unique name from the given object name by adding the Node
//! number.
//!
Ogre::String Node::createUniqueName ( const QString &objectName )
{
    return QString("%1_%2").arg(s_totalNumberOfInstances).arg(objectName).toStdString();
}



