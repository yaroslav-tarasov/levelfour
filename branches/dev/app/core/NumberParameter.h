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
//! \file "NumberParameter.h"
//! \brief Header file for NumberParameter class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       27.07.2009 (last updated)
//!

#ifndef NUMBERPARAMETER_H
#define NUMBERPARAMETER_H

#include "FrapperPrerequisites.h"
#include "Parameter.h"
#include "Key.h"

//!
//! Class representing parameters for numeric values.
//!
class FRAPPER_CORE_EXPORT NumberParameter : public Parameter
{

public: // nested enumerations

    //!
    //! Nested enumeration for the different input methods for editing number
    //! parameters.
    //!
    static enum InputMethod {
        IM_SpinBox = 0,
        IM_Slider,
        IM_SliderPlusSpinBox,

        IM_NumInputMethods
    };

public: // static functions

    //!
    //! Decodes the given input method string to an input method value.
    //!
    //! \param inputMethodString The input method string to decode.
    //! \return An input method corresponding to the given input method string.
    //!
    static InputMethod decodeInputMethod ( const QString &inputMethodString );

public: // constructors and destructors

    //!
    //! Constructor of the NumberParameter class.
    //!
    //! \param name The name of the parameter.
    //! \param type The type of the parameter's value(s).
    //! \param value The parameter's value.
	//! \param animated The parameter's animation status.
    //!
    NumberParameter ( const QString &name, Type type, const QVariant &value );

    //!
	//! Copy constructor of the NumberParameter class.
	//!
	//! \param parameter The parameter to copy.
	//! \param node The parent node.
	//!
    NumberParameter ( const NumberParameter &parameter, Node* node = 0  );

    //!
    //! Destructor of the FilenameParameter class.
    //!
    virtual ~NumberParameter ();

public: // functions

    //!
    //! Creates an exact copy of the parameter.
    //!
    //! \return An exact copy of the parameter.
    //!
    virtual AbstractParameter * clone ();

    //!
    //! Sets whether the parameter's value has changed.
    //!
    //! \param dirty The new value for the parameter's dirty flag.
    //!
    virtual void setDirty ( const bool dirty );

	//!
	//! Sets whether the controls for editing the parameter or parameter group
	//! in the UI should be enabled.
	//!
	//! \param enabled The value for the parameter's enabled flag.
	//!
	virtual void setEnabled ( const bool enabled );

    //!
    //! Returns the desired input method of numeric values of the parameter.
    //!
    //! \return The input method for numeric values of the parameter.
    //!
    InputMethod getInputMethod () const;

    //!
    //! Sets the desired input method of numeric values of the parameter.
    //!
    //! \param inputMethod The input method for numeric values of the parameter.
    //!
    void setInputMethod ( InputMethod inputMethod );

    //!
    //! Returns the lower limit for numeric values of the parameter.
    //!
    //! \return The lower limit for numeric values of the parameter.
    //!
    QVariant getMinValue () const;

    //!
    //! Sets the lower limit for numeric values of the parameter.
    //!
    //! \param minValue The lower limit for numeric values of the parameter.
    //!
    void setMinValue (const QVariant &minValue );

    //!
    //! Returns the upper limit for numeric values of the parameter.
    //!
    //! \return The upper limit for numeric values of the parameter.
    //!
    QVariant getMaxValue () const;

    //!
    //! Returns the absolute upper limit for numeric values of the parameter.
    //!
    //! \return The absolute upper limit for numeric values of the parameter.
    //!
    float getAbsMaxValue () const;

    //!
    //! Sets the upper limit for numeric values of the parameter.
    //!
    //! \param maxValue The upper limit for numeric values of the parameter.
    //!
    void setMaxValue ( const QVariant &maxValue );

    //!
    //! Returns the step size numeric values of the parameter.
    //!
    //! \return The step size for numeric values of the parameter.
    //!
    QVariant getStepSize () const;

    //!
    //! Sets the step size numeric values of the parameter.
    //!
    //! \param stepSize The step size for numeric values of the parameter.
    //!
    void setStepSize ( const QVariant &stepSize );

    //!
    //! Returns the unit of the parameter.
    //!
    //! \return The unit of the parameter (an empty string if no unit has been set).
    //!
    QString getUnit () const;

    //!
    //! Sets the unit for the parameter.
    //!
    //! \param unit The unit for the parameter.
    //!
    void setUnit ( const QString &unit );

    //!
    //! Sets the current time for the parameter.
    //!
    //! \param timeValue The current time for the parameter.
    //!
    void setTime ( const QVariant &timeValue );

    //!
    //! Calculate the limits for numeric values of the parameter.
    //!
    void calculateExtrema ();

    //!
    //! Scales all values from the parameter
    //!
    //! \param The scale factor.
    //!
    void scaleValues( const float &scaleFactor);

public: // animation functions

    //!
    //! Returns whether the parameter contains keys.
    //!
    //! \return True if the parameter contains keys, otherwise False.
    //!
    bool isAnimated () const;

    //!
    //! Adds the given parameter to the given time in the list of keys.
    //!
    //! \param key The key to add to the list.
    //!
    void addKey ( Key *key );

    //!
    //! Adds the actuel numberparameter value to the given time in the list of keys.
    //!
    //! \param time The time index to add the value to the list.
    //!
    void addKey ( const int &time );

    //!
    //! Adds the given parameter to the back of the list of keys.
    //!
    //! \param value The value to add to the list.
    //!
    void addKey ( const QVariant &value );

    //!
    //! Adds the given parameter to the back of the list of keys.
    //!
    //! \param value The value to add to the list
    //!
    void addKey ( const float &value );

    //!
    //! Adds the given parameter to the given time in the list of keys, without sorting.
    //!
    //! \param key The key to add to the list
    //!
    void addKeyPresorted ( Key *key );

    //!
    //! Sets the given list in the given steps in the list of keys.
    //!
    //! \param keys The value list to add to the key list.
    //!
    void setKeys (const QList<float> *keys );

    //!
    //! Returns the number of keys in the list.
    //!
    //! \return The number of keys in the list.
    //!
    unsigned int numKeys () const;

    //!
    //! Returns whether the list of keys is empty.
    //!
    //! \return True if the list of keys is empty, otherwise False.
    //!
    bool isEmpty () const;

    //!
    //! Removes the key at the given time.
    //!
    //! \param time The time of the key to remove.
    //!
    void removeKey ( const float &time );

    //!
    //! Removes the key at the given index.
    //!
    //! \param index The index for the key to remove.
    //!
    void removeKey ( const int &index );

    //!
    //! Removes the keys at the given range.
    //!
    //! \param minTime The start time for the keys to remove.
    //! \param maxTime The end time for the keys to remove.
    //!
    void removeKeys ( const float &minTime, const float &maxTime );

    //!
    //! Removes the keys at the given range.
    //!
    //! \param minIndex The start time for the keys to remove.
    //! \param maxIndex The end time for the keys to remove.
    //!
    void removeKeys ( const int &minIndex, const int &maxIndex );

    //!
    //! Removes all keys.
    //!
    void clearKeys ( );

    //!
    //! Returns the key at the given time.
    //!
    //! \param time The time to the key.
    //! \return The key at the given time.
    //!
    Key * getKey ( const float &time );

    //!
    //! Returns the key at the given index.
    //!
    //! \param index The index to the Key.
    //! \return The key at the given index.
    //!
    Key * getKey ( const unsigned int &index );

    //!
    //! Returns a pointer to the list of keys.
    //!
    //! \return A pointer to the list of keys.
    //!
    QList<Key *> * getKeys ();

    //!
    //! Returns the size of the list of keys.
    //!
    //! \return The size of the list of keys.
    //!
    unsigned int getKeysSize () const;

    //!
    //! Returns the interpolated value of the key at the given time.
    //!
    //! \param time The time of the key.
    //! \return The interpolated value of the key at the given time.
    //!
    inline QVariant getKeyValueInterpol ( const QVariant &time );

    //!
    //! Returns the value of the key at the given time.
    //!
    //! \param time The time to the Key.
    //! \return The value at the given time.
    //!
    QVariant getKeyValuePos ( const float &time );

    //!
    //! Returns the value of the key at the given index.
    //!
    //! \param index The index to the Key.
    //! \return The value at the given index.
    //!
    QVariant getKeyValueIndex ( const unsigned int &index ) const;

    //!
    //! Returns the parameter's value while optionally triggering the
    //! evaluation chain.
    //!
    //! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
    //! \return The parameter's value.
    //!
    QVariant getValue ( bool triggerEvaluation = false ) ;



protected: // functions

    //!
    //! Returns the sorted index from keylist by evaluating the time value.
    //!
    //! \param time The time of the Key.
    //! \return A list iterator to the index in list.
    //!
    inline QList<Key *>::iterator findIndex ( const float &time );

    //!
    //! Returns the sorted index from keylist by evaluating the time value of the given key.
    //!
    //! \param key The Key to add.
    //! \return A list iterator to the index in list.
    //!
    inline QList<Key *>::iterator findIndex ( const Key *key );

protected: // helper functions

    //!
    //! Less than definition Function for comparing unique icon Key *
    //!
    //! \param k1 The first key for comparison.
    //! \param k2 The second key for comparison.
    //! \return A boolean as result from comparison.
    //!
    static inline bool lessThan ( const Key *k1, const Key *k2 );

private: // data

    //!
    //! The input method for numeric values of the parameter.
    //!
    InputMethod m_inputMethod;

    //!
    //! The lower limit for numeric values of the parameter.
    //!
    QVariant m_minValue;

    //!
    //! The upper limit for numeric values of the parameter.
    //!
    QVariant m_maxValue;

    //!
    //! The step size for numeric values of the parameter.
    //!
    QVariant m_stepSize;

    //!
    //! The parameter's unit (an empty string if no unit has been set).
    //!
    QString m_unit;

    //!
    //! The list of keys for animating the parameter's numeric value.
    //!
    QList<Key *> m_keys;
};


#endif
