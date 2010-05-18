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
//! \file "NumberParameter.cpp"
//! \brief Implementation file for NumberParameter class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       05.08.2009 (last updated)
//!

#include "NumberParameter.h"
#include "Node.h"


///
/// Public Static Functions
///


//!
//! Decodes the given input method string to an input method value.
//!
//! \param inputMethodString The input method string to decode.
//! \return An input method corresponding to the given input method string.
//!
NumberParameter::InputMethod NumberParameter::decodeInputMethod ( const QString &inputMethodString )
{
    if (inputMethodString == "SliderPlusSpinBox")
        return IM_SliderPlusSpinBox;
    else
        return IM_SpinBox;
}


///
/// Constructors and Destructors
///


//!
//! Constructor of the FilenameParameter class.
//!
//! \param name The name of the parameter.
//! \param type The type of the parameter's value(s).
//! \param value The parameter's value.
//! \param animated The parameter's animation status.
//!
NumberParameter::NumberParameter ( const QString &name, Type type, const QVariant &value) :
    Parameter(name, type, value),
    m_inputMethod(IM_SpinBox),
    m_minValue(type == Parameter::T_UnsignedInt ? 0 : -100),
    m_maxValue(100),
    m_stepSize(1),
    m_unit("")
{
}


//!
//! Copy constructor of the NumberParameter class.
//!
//! \param parameter The parameter to copy.
//! \param node The parent node.
//!
NumberParameter::NumberParameter ( const NumberParameter &parameter, Node* node /*= 0*/ ) :
    Parameter(parameter),
    m_inputMethod(parameter.m_inputMethod),
    m_minValue(parameter.m_minValue),
    m_maxValue(parameter.m_maxValue),
    m_stepSize(parameter.m_stepSize),
    m_unit(parameter.m_unit)
{
	foreach (Key *key, parameter.m_keys) {
		Key *newKey = new Key(*key);
		newKey->setParameter(this);
		m_keys.append(newKey);
	}
	m_node = node;
	if (node)
		m_node->getTimeParameter()->addAffectedParameter(this);
}


//!
//! Destructor of the FilenameParameter class.
//!
NumberParameter::~NumberParameter ()
{
	clearKeys();
}


///
/// Public Functions
///


//!
//! Creates an exact copy of the parameter.
//!
//! \return An exact copy of the parameter.
//!
AbstractParameter * NumberParameter::clone ()
{
    return new NumberParameter(*this);
}


//!
//! Sets whether the parameter's value has changed.
//!
//! \param dirty The new value for the parameter's dirty flag.
//!
void NumberParameter::setDirty (const bool dirty )
{
    Parameter::setDirty(dirty);
    // check if the parameter is animated and has become dirty
    if (dirty && m_enabled && !m_keys.empty()) {
        // update the value depending on the current time index
        NumberParameter *timeParameter = static_cast<NumberParameter *>(m_node->getTimeParameter());
		if ( timeParameter ){
			setValue(getKeyValueInterpol(timeParameter->getValue()));
		}
    }
}

//!
//! Sets whether the controls for editing the parameter or parameter group
//! in the UI should be enabled.
//!
//! \param enabled The value for the parameter's enabled flag.
//!
void NumberParameter::setEnabled (const bool enabled )
{
    m_enabled = enabled;
	if (!enabled) setValue(0.0);
	emit valueChanged();
}


//!
//! Returns the desired input method of numeric values of the parameter, e.g. Slider, etc.
//!
//! \return The input method for numeric values of the parameter.
//!
NumberParameter::InputMethod NumberParameter::getInputMethod () const
{
    return m_inputMethod;
}


//!
//! Sets the desired input method of numeric values of the parameter, e.g. Slider, etc.
//!
//! \param stepSize The input method for numeric values of the parameter.
//!
void NumberParameter::setInputMethod ( NumberParameter::InputMethod inputMethod )
{
    m_inputMethod = inputMethod;
}


//!
//! Returns the lower limit for numeric values of the parameter.
//!
//! \return The lower limit for numeric values of the parameter.
//!
QVariant NumberParameter::getMinValue () const
{
    return m_minValue;
}


//!
//! Sets the lower limit for numeric values of the parameter.
//!
//! \param minValue The lower limit for numeric values of the parameter.
//!
void NumberParameter::setMinValue (const QVariant &minValue )
{
    m_minValue = minValue;
}


//!
//! Returns the upper limit for numeric values of the parameter.
//!
//! \return The upper limit for numeric values of the parameter.
//!
QVariant NumberParameter::getMaxValue () const
{
    return m_maxValue;
}


//!
//! Sets the upper limit for numeric values of the parameter.
//!
//! \param maxValue The upper limit for numeric values of the parameter.
//!
void NumberParameter::setMaxValue (const QVariant &maxValue )
{
    m_maxValue = maxValue;
}


//!
//! Returns the absolute upper limit for numeric values of the parameter.
//!
//! \return The absolute upper limit for numeric values of the parameter.
//!
float NumberParameter::getAbsMaxValue () const
{
    if (abs(m_minValue.toDouble()) > m_maxValue.toDouble())
        return abs(m_minValue.toDouble());
    else
        return m_maxValue.toDouble();
}


//!
//! Returns the step size numeric values of the parameter.
//!
//! \return The step size for numeric values of the parameter.
//!
QVariant NumberParameter::getStepSize () const
{
    return m_stepSize;
}


//!
//! Sets the step size numeric values of the parameter.
//!
//! \param stepSize The step size for numeric values of the parameter.
//!
void NumberParameter::setStepSize ( const QVariant &stepSize )
{
    m_stepSize = stepSize;
}


//!
//! Returns the unit of the parameter.
//!
//! \return The unit of the parameter.
//!
QString NumberParameter::getUnit () const
{
    return m_unit;
}


//!
//! Sets the unit for the parameter.
//!
//! \param unit The unit for the parameter.
//!
void NumberParameter::setUnit ( const QString &unit )
{
    m_unit = unit;
}


//!
//! Calculate the limits for numeric values of the parameter.
//!
void NumberParameter::calculateExtrema ()
{
    m_maxValue = -HUGE;
    m_minValue = +HUGE;
    register float keyvalue;

    foreach (Key *key, m_keys) {
        keyvalue = key->getValue().toDouble();
        if (keyvalue > m_maxValue.toDouble())
            m_maxValue = keyvalue;
        if (keyvalue < m_minValue.toDouble())
            m_minValue = keyvalue;
    }
}

//!
//! Scales all values from the parameter
//!
//! \param The scale factor.
//!
void NumberParameter::scaleValues( const float &scaleFactor )
{
    foreach (Key *key, m_keys)
        *key /= scaleFactor;
    m_value.setValue(m_value.toDouble() / scaleFactor);
    m_minValue.setValue(m_minValue.toDouble() / scaleFactor);
    m_maxValue.setValue(m_maxValue.toDouble() / scaleFactor);
}


///
/// Public Animation Functions
///


//!
//! Returns whether the parameter contains keys.
//!
//! \return True if the parameter contains keys, otherwise False.
//!
bool NumberParameter::isAnimated () const
{
    return !m_keys.isEmpty();
}


//!
//! Adds the given parameter to the given time in the list of keys.
//!
//! \param key The key to add to the list
//!
void NumberParameter::addKey ( Key *key )
{
	key->setParameter(this);
	m_keys.insert(findIndex(key->getIndex()), key);
}

//!
//! Adds the actuel numberparameter value to the given time in the list of keys.
//!
//! \param time The time index to add the value to the list.
//!
void NumberParameter::addKey ( const int &time )
{
    const float index = time * m_stepSize.toDouble();
	m_keys.insert(findIndex(index), new Key(index, m_value, this));
}


//!
//! Adds the given parameter to the back of the list of keys.
//!
//! \param value The value to add to the list
//!
void NumberParameter::addKey ( const QVariant &value )
{
    m_keys.push_back(new Key(m_keys.size() * m_stepSize.toDouble(), value, this));
}

//!
//! Adds the given parameter to the back of the list of keys.
//!
//! \param value The value to add to the list
//!
void NumberParameter::addKey ( const float &value )
{
    m_keys.push_back(new Key((m_keys.size()) * m_stepSize.toDouble(), value, this));
    m_node->getTimeParameter()->addAffectedParameter(this);
}


//!
//! Adds the given parameter to the given time in the list of keys, without sorting.
//!
//! \param key The key to add to the list
//!
void NumberParameter::addKeyPresorted ( Key *key )
{
	key->setParameter(this);
    m_keys.push_back(key);
	if (m_node)
		m_node->getTimeParameter()->addAffectedParameter(this);
}


//!
//! Sets the given List in the given steps in the list of keys.
//!
//! \param key The value list to add to the key list
//!
void NumberParameter::setKeys (const QList<float> *keys )
{
    m_keys.clear();
    register float step = 0.0;
    foreach (float key, *keys)
    {
        m_keys.push_back(new Key(step, key, this));
        step += m_stepSize.toDouble();
    }
	if (m_node)
		m_node->getTimeParameter()->addAffectedParameter(this);
}


//!
//! Returns the number of keys in list.
//!
//! \return The number of keys.
//!
unsigned int NumberParameter::numKeys () const
{
    return (unsigned int) m_keys.size();
}


//!
//! Check for empty list.
//!
//! \return True if list is empty.
//!
bool NumberParameter::isEmpty () const
{
    return m_keys.isEmpty();
}


//!
//! Removes the key at the given time.
//!
//! \param time The time for the key to remove.
//!
void NumberParameter::removeKey ( const float &time )
{
    QList<Key *>::iterator iter = findIndex(time);

    if ((*iter)->getIndex() == time)
        m_keys.erase(iter);
    else
        Log::error("Key not in list.", "NumberParameter::removeKey");
}


//!
//! Removes the key at the given index.
//!
//! \param index The index for the key to remove.
//!
void NumberParameter::removeKey ( const int &index )
{
	if (index < m_keys.size()) {
		delete m_keys.at(index);
        m_keys.removeAt(index);
	}
    else
        Log::error("List index out of range.", "NumberParameter::removeKey");
}


//!
//! Removes the keys at the given range.
//!
//! \param minTime The start time for the keys to remove.
//! \param maxTime The end time for the keys to remove.
//!
void NumberParameter::removeKeys ( const float &minTime, const float &maxTime )
{
    QList<Key *>::iterator iter_start = findIndex(minTime);
    QList<Key *>::iterator iter_end = findIndex(maxTime);

    m_keys.erase(iter_start, iter_end);
}


//!
//! Removes the keys at the given range.
//!
//! \param minIndex The start time for the keys to remove.
//! \param maxIndex The end time for the keys to remove.
//!
void NumberParameter::removeKeys ( const int &minIndex, const int &maxIndex )
{
    if ((minIndex < m_keys.size()) && (maxIndex < m_keys.size()) && (minIndex <= maxIndex))
        m_keys.erase(m_keys.begin()+minIndex, m_keys.begin()+maxIndex);
    else
        Log::error("Index out of range.", "NumberParameter::removeKeys");
}


//!
//! Removes all keys.
//!
void NumberParameter::clearKeys ( )
{
    foreach (Key* key, m_keys)
		delete key;
	m_keys.clear();
}


//!
//! Returns the key at the given time.
//!
//! \param time The time to the key.
//! \return The key at the given time.
//!
Key *NumberParameter::getKey ( const float &time )
{
    QList<Key *>::iterator iter = findIndex(time);

    if ((*iter)->getIndex() == time)
        return *iter;
    else {
        Log::error("Key not in list.", "NumberParameter::getKey");
        return 0;
    }
}


//!
//! Returns the key at the given index.
//!
//! \param index The index to the Key.
//! \return The key at the given index.
//!
Key *NumberParameter::getKey ( const unsigned int &index )
{
    if (index < (unsigned int) m_keys.size())
        return m_keys[index];
    else {
        Log::error("List index out of range.", "NumberParameter::getKey");
        return 0;
    }
}


//!
//! Returns a pointer to the list of keys.
//!
//! \return A pointer to the list of keys.
//!
QList<Key *> * NumberParameter::getKeys ()
{
    return &m_keys;
}


//!
//! Returns the size of the list of keys.
//!
//! \return The size of the list of keys.
//!
unsigned int NumberParameter::getKeysSize () const
{
    if (m_keys.empty())
        return 0;
    else
        return m_keys.size();
}


//!
//! Returns the value of the key at the given time.
//!
//! \param time The time of the key.
//! \return The value at the given time.
//!
QVariant NumberParameter::getKeyValuePos ( const float &time )
{
    QList<Key *>::iterator iter = findIndex(time);
    
    if ((*iter)->getIndex() == time)
        return (*iter)->getValue();
    else {
        Log::error("KeyValue not in list.", "NumberParameter::getKeyValue");
        return 0;
    }
}


//!
//! Returns the interpolated value of the key at the given time.
//!
//! \param time The time of the key.
//! \return The interpolated value of the key at the given time.
//!
inline QVariant NumberParameter::getKeyValueInterpol ( const QVariant &time )
{
	const float pos = time.toDouble() * m_stepSize.toDouble();
    QList<Key *>::iterator iter = findIndex(pos);
    if (pos > 2500)
        int x = 50;

    if (pos > m_keys.last()->getIndex())
        return (m_keys.last())->getValue(); 
    else if ( m_keys.begin() < iter ) {
        Key *debugKey = (*iter);
        //register const float inBetween = pos - (*iter)->getIndex();
        register const float lowerX    = (*(iter-1))->getIndex();
        register const float upperX    = (*iter)->getIndex();;
        register const float lowerY    = (*(iter-1))->getValue().toDouble();
        register const float upperY    = (*iter)->getValue().toDouble();

        // linear interpolation
        register const float factor = (pos - lowerX) / (upperX - lowerX);
        register const float result = lowerY + (upperY - lowerY) * factor;
        return QVariant(result);
    }
    else {
        return (m_keys.first())->getValue();
    }
}


//!
//! Returns the value of the key at the given index.
//!
//! \param index The index to the Key.
//! \return The value at the given index.
//!
QVariant NumberParameter::getKeyValueIndex ( const unsigned int &index ) const
{
    if (index < (unsigned int) m_keys.size())
        return m_keys[index]->getValue();
    else {
        Log::error("List index out of range.", "NumberParameter::getKey");
        return 0;
    }
}


//!
//! Returns the parameter's value while optionally triggering the
//! evaluation chain.
//!
//! \param Trigger evaluation.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The parameter's value.
//!
QVariant NumberParameter::getValue ( bool triggerEvaluation /* = false */ )
{
    // optionally trigger the evaluation chain
    if (triggerEvaluation) {
        Parameter::PinType pinType = getPinType();
        if (pinType == Parameter::PT_Output || pinType == Parameter::PT_Input)
            propagateEvaluation(0);
    }

    return m_value;
}


///
/// Protected Functions
///


//!
//! Returns the sorted index from keylist by evaluating the time value.
//!
//! \parameter pos The time of the Key.
//! \return A list iterator to the index in list.
//!
inline QList<Key *>::iterator NumberParameter::findIndex ( const float &pos )
{
    return qUpperBound(m_keys.begin(), m_keys.end(), &Key(pos, 0.0f), lessThan);
}


//!
//! Returns the sorted index from keylist by evaluating the time value of the given key.
//!
//! \parameter key The Key to add.
//! \return A list iterator to the index in list.
//!
inline QList<Key *>::iterator NumberParameter::findIndex ( const Key *key )
{
    return qUpperBound(m_keys.begin(), m_keys.end(), key, lessThan);
}


///
/// Protected Helper Functions
///


//!
//! Less than definition Function for comparing unique icon Key *
//!
//! \parameter k1 The first key for comparison.
//! \parameter k2 The second key for comparison.
//! \return A boolean as result from comparison.
//!
inline bool NumberParameter::lessThan ( const Key *k1, const Key *k2 )
{
    return k1->getIndex() < k2->getIndex();
}
