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
//! \file "Parameter.cpp"
//! \brief Implementation file for Parameter class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       05.08.2009 (last updated)
//!

#include "Parameter.h"
#include "NumberParameter.h"
#include "FilenameParameter.h"
#include "EnumerationParameter.h"
#include "SceneNodeParameter.h"
#include "ParameterPlugin.h"
#include "Node.h"
#include "Log.h"
#include <QtGui/QColor>
#include "MotionDataNode.h"

INIT_INSTANCE_COUNTER(Parameter)
Q_DECLARE_METATYPE(Ogre::Vector3)
Q_DECLARE_METATYPE(Ogre::TexturePtr)
Q_DECLARE_METATYPE(ParameterGroup *)


///
/// Macro Definitions
///


//!
//! The maximum number of values of a given type a parameter can store.
//!
#define MAX_SIZE 255


///
/// Global Variables with File Scope
///


//!
//! List of strings of names of parameter types.
//!
static const char *ParameterTypeNames[Parameter::T_NumTypes] = {
    "Bool",
    "Int",
    "UnsignedInt",
    "Float",
    "String",
    "Filename",
    "Color",
    "Enumeration",
    "TextInfo",
    "Command",
    "Geometry",
    "Light",
    "Camera",
    "Image",
    "Group",
	"PlugIn"
};


//!
//! List of colors of parameter types.
//!
static const QColor ParameterTypeColors[Parameter::T_NumTypes] = {
    QColor(20, 20, 20),      // Bool
    QColor(109, 255, 109),      // Int
    QColor(158, 255, 158),      // UnsignedInt
    QColor(109, 173, 158),      // Float
    QColor(109, 255, 255),      // String
    QColor(173, 173, 255),      // Filename
    QColor(230, 45, 137),       // Color
    QColor(0, 172, 236),        // Enumeration
    QColor(174, 174, 174),      // TextInfo
    QColor( 40,  40, 140),      // Command
    QColor(255, 0, 0),          // Geometry
    QColor(255, 255, 0),        // Light
    QColor(255, 103, 0),        // Camera
    QColor(173, 255, 173),      // Image
    QColor(100, 100, 100),       // Group
	QColor(0, 255, 0)       // Plugin

// (230, 45, 137) // Pink
// (255, 0, 0) // Red
// (255, 103, 0) // Orange
// (255, 255, 0) // Yellow
// (101, 255, 50) // Green
// (0, 195, 136) // Aqua
// (125, 43, 139) // Purple
// (0, 172, 236) // Light Blue

};


//!
//! List of parameter types that support multiplicity values bigger than 1.
//!
static const QList<Parameter::Type> ParameterMultiplicityTypes = QList<Parameter::Type>()
    << Parameter::T_Int
    << Parameter::T_UnsignedInt
    << Parameter::T_Float
    << Parameter::T_Geometry
    << Parameter::T_Light
    << Parameter::T_Camera
    << Parameter::T_Image
	<< Parameter::T_Group
;


//!
//! List of default values for all parameter types.
//!
static const QVariantList ParameterDefaultValues = QVariantList()
    << false                    // Bool
    << (int) 0                  // Int
    << (unsigned int) 0         // UnsignedInt
    << 0.0                      // Float
    << ""                       // String
    << ""                       // Filename
    << QColor(Qt::black)        // Color
    << (int) 0                  // Enumeration
    << ""                       // TextInfo
    << QVariant()               // Command
    << GeometryParameter::DefaultValue
    << LightParameter::DefaultValue
    << CameraParameter::DefaultValue
    << QVariant::fromValue<Ogre::TexturePtr>(Ogre::TexturePtr(0))   // Image
    << QVariant::fromValue<ParameterGroup *>(0)                     // Group
	<< QVariant()				// PlugIn
;


///
/// Public Static Functions
///


//!
//! Returns the name of the given parameter type.
//!
//! \param type The type for which to return the name.
//! \return The name of the given parameter type.
//!
QString Parameter::getTypeName ( Parameter::Type type )
{
    if (type != Parameter::T_Unknown)
        return QString(ParameterTypeNames[type]);
    else
        return tr("Unknown");
}


//!
//! Returns the color of the given parameter type.
//!
//! \param type The type for which to return the color.
//! \return The color of the given parameter type.
//!
QColor Parameter::getTypeColor ( Parameter::Type type )
{
    if (type != Parameter::T_Unknown)
        return ParameterTypeColors[type];
    else
        return QColor(Qt::black);
}


//!
//! Returns the parameter type corresponding to the given name.
//!
//! \param typeName The name of a parameter type.
//! \return The parameter type corresponding to the given name.
//!
Parameter::Type Parameter::getTypeByName ( const QString &typeName )
{
    if (typeName != tr("Unknown"))
        for (int i = 0; i < T_NumTypes; ++i)
            if (ParameterTypeNames[(Type) i] == typeName)
                return (Type) i;

    return T_Unknown;
}


//!
//! Returns the default value for parameters of the given type.
//!
//! \param type The parameter type for which to return the default value.
//! \return The default value for parameters of the given type.
//!
QVariant Parameter::getDefaultValue ( Parameter::Type type )
{
    if (type != Parameter::T_Unknown)
        return ParameterDefaultValues[type];
    else
        return QVariant();
}


//!
//! Converts the given string to a color value.
//! The string should contain comma-separated integer values for the red,
//! green and blue channels respectively.
//!
//! \param color The string to convert to a color.
//! \return A color value corresponding to the given string, or an invalid color when an error occured during conversion.
//!
QColor Parameter::decodeIntColor ( const QString &color )
{
    QColor result = QColor();

    QStringList parts = color.split(',');
    bool ok = parts.size() >= 3;
    if (ok) {
        QString red = parts[0];
        int r = red.toInt(&ok);
        if (ok) {
            QString green = parts[1];
            int g = green.toInt(&ok);
            if (ok) {
                QString blue = parts[2];
                int b = blue.toInt(&ok);
                if (ok)
                    result = QColor(r, g, b);
            }
        }
    }
    //if (!ok)
    //    Log::error(QString("Error converting value \"%1\" to a color value."), "Parameter::decodeIntColor");

    return result;
}


//!
//! Converts the given string to a color value.
//! The string should contain comma-separated double-precision floating
//! point values for the red, green and blue channels respectively.
//!
//! \param color The string to convert to a color.
//! \return A color value corresponding to the given string, or an invalid color when an error occured during conversion.
//!
QColor Parameter::decodeDoubleColor ( const QString &color )
{
    QColor result = QColor();

    QStringList parts = color.split(',');
    bool ok = parts.size() >= 3;
    if (ok) {
        QString red = parts[0];
        double r = red.toDouble(&ok);
        if (ok) {
            QString green = parts[1];
            double g = green.toDouble(&ok);
            if (ok) {
                QString blue = parts[2];
                double b = blue.toDouble(&ok);
                if (ok)
                    result = QColor::fromRgbF(r, g, b);
            }
        }
    }
    //if (!ok)
    //    Log::error(QString("Error converting value \"%1\" to a color value."), "Parameter::decodeDoubleColor");

    return result;
}


//!
//! Converts the given string to a size value.
//!
//! \param size The string to convert to a size value.
//! \return A size value corresponding to the given string.
//!
Parameter::Size Parameter::decodeSize ( const QString &size )
{
    Size result = 1;
    bool ok = true;
    unsigned int u = size.toUInt(&ok);
    if (ok) {
        if (u <= MAX_SIZE)
            result = (Size) u;
        else {
            Log::warning(QString("Parameters can only contain %1 values of a given type. The size value \"%2\" will be clamped to %1.").arg(MAX_SIZE).arg(size).arg(MAX_SIZE), "Parameter::decodeSize");
            u = MAX_SIZE;
        }
    } else
        Log::error(QString("The parameter's size value \"%1\" could not be converted to an unsigned integer number.").arg(size), "Parameter::decodeSize");
    return result;
}


//!
//! Converts the given string to a multiplicity value.
//!
//! \param multiplicity The string to convert to a multiplicity value.
//! \return A multiplicity value corresponding to the given string.
//!
int Parameter::decodeMultiplicity ( const QString &multiplicity )
{
    int result;
    if (multiplicity == "*")
        result = M_OneOrMore;
    else {
        bool ok;
        int value = multiplicity.toInt(&ok);
        if (ok)
            result = value;
        else
            result = M_Invalid;
    }
    return result;
}


//!
//! Converts the given string to a pin type value.
//!
//! \param pinTypeString The string to convert to a pin type value.
//! \return A pin type value corresponding to the given string.
//!
Parameter::PinType Parameter::decodePinType ( const QString &pinTypeString )
{
    if (pinTypeString == "in")
        return PT_Input;
    else if (pinTypeString == "out")
        return PT_Output;
    else
        return PT_None;
}


//!
//! Creates a parameter with the given name and type.
//!
//! Depending on the given type an instance of a Parameter-derived class
//! may be returned, e.g. a NumberParameter object if type equals T_Float.
//!
//! \param name The name to use for the new parameter.
//! \param type The type to use for the new parameter.
//! \param defaultValue The default value to use for the new parameter.
//! \return A new parameter with the given name and type.
//!
Parameter * Parameter::create ( const QString &name, Parameter::Type type, QVariant defaultValue /* = QVariant() */ )
{
    // use a default value depending on the given type if no default value is given
    if (!defaultValue.isValid())
        defaultValue = Parameter::getDefaultValue(type);

    switch (type) {
        case T_Int:
        case T_UnsignedInt:
        case T_Float:
            return new NumberParameter(name, type, defaultValue);
        case T_Filename:
            return new FilenameParameter(name, defaultValue);
        case T_Enumeration:
            return new EnumerationParameter(name, defaultValue);
        case T_Geometry:
            return new GeometryParameter(name);
        case T_Light:
            return new LightParameter(name);
        case T_Camera:
            return new CameraParameter(name);
		case T_PlugIn:
			return new ParameterPlugin(name, defaultValue);
        default:
            return new Parameter(name, type, defaultValue);
    }
}


//!
//! Creates a parameter from the given XML element.
//!
//! Depending on the type described in the XML element an instance of a
//! Parameter-derived class may be returned, e.g. a NumberParameter object
//! if the type equals T_Float.
//!
//! \param element The element in a DOM tree containing information about the parameter to create.
//! \return A new parameter corresponding to the given XML element, or 0 if the parameter description is invalid.
//!
Parameter * Parameter::create ( const QDomElement &element )
{
    // get attributes of the given element as strings
    QString name = element.attribute("name");
    QString typeName = element.attribute("type");
    QString sizeString = element.attribute("size", "1");
    QString multiplicityString = element.attribute("multiplicity", "1");
    QString defaultValueString = element.attribute("defaultValue");
    QString visibleString = element.attribute("visible", "true");
    QString enabledString = element.attribute("enabled", "true");
    QString readOnlyString = element.attribute("readOnly", "false");
    QString selfEvaluatingString = element.attribute("selfEvaluating", "false");
    QString affects = element.attribute("affects");
    QString description = element.attribute("description");
    QString unit = element.attribute("unit");
    QString pinTypeString = element.attribute("pin");
    QString inputMethodString = element.attribute("inputMethod");
    QString minValueString = element.attribute("minValue", typeName == "UnsignedInt" ? "0" : "-100");
    QString maxValueString = element.attribute("maxValue", "100");
    QString stepSizeString = element.attribute("stepSize", "1");
    QString filterString = element.attribute("filter");

    // check if there are illegal characters in the parameter's name
    if (name.contains(QRegExp("[\\[\\]]"))) {
        Log::error(QString("Illegal characters in parameter name \"%1\": '[' or ']'.").arg(name), "Parameter::create");
        return 0;
    }

    // decode string values to integer-based values
    Type parameterType = Parameter::getTypeByName(typeName);
    Size size = Parameter::decodeSize(sizeString);
    int multiplicity = Parameter::decodeMultiplicity(multiplicityString);
    PinType pinType = Parameter::decodePinType(pinTypeString);

    // make sure the type of parameter is supported
    if (parameterType <= T_Unknown || parameterType >= T_NumTypes) {
        Log::error(QString("Error creating parameter \"%1\": Type \"%2\" not supported.").arg(name).arg(typeName), "Parameter::create");
        return 0;
    }

    // decode flag values
    bool visible = visibleString != "false";
    bool enabled = enabledString != "false";
    bool readOnly = readOnlyString != "false";
    bool selfEvaluating = selfEvaluatingString != "false";

    // check if the default value is empty
    if (defaultValueString.isEmpty()) {
        if (parameterType == T_Int || parameterType == T_UnsignedInt || parameterType == T_Float) {
            // create a default value string consisting of comma-separated 0s for the number of values specified by the parameter's size
            defaultValueString = "0";
            for (Size i = 1; i < size; ++i)
                defaultValueString += ", 0";
        } else if (parameterType == T_Color)
            defaultValueString = "0, 0, 0";
    }

    // store the default value in a variant
    QVariant defaultValue;
    bool converted = true;
    switch (parameterType) {
        case T_Bool:
            defaultValue = QVariant::fromValue(defaultValueString == "true");
            break;
        case T_Int:
            if (size == 1) {
                // convert single default value
                defaultValue = QVariant::fromValue(defaultValueString.toInt(&converted));
                if (!converted)
                    Log::error(QString("Error converting parameter default value to integer number: \"%1\"").arg(defaultValueString), "Parameter::create");
            } else {
                // convert multiple values in default value
                QStringList parts = defaultValueString.split(',');
                QVariantList defaultValues;
                Size i = 0;
                converted = true;
                while (converted && i < size && (int) i < parts.size()) {
                    unsigned int value = parts[i++].toInt(&converted);
                    if (converted)
                        defaultValues << QVariant(value);
                }
                if (converted)
                    defaultValue = QVariant::fromValue(defaultValues);
                else
                    Log::error(QString("Error converting parameter default value to integer number: \"%1\"").arg(parts[i]), "Parameter::create");
            }
            break;
        case T_UnsignedInt:
            if (size == 1) {
                // convert single default value
                defaultValue = QVariant::fromValue(defaultValueString.toUInt(&converted));
                if (!converted)
                    Log::error(QString("Error converting parameter default value to unsigned integer number: \"%1\"").arg(defaultValueString), "Parameter::create");
            } else {
                // convert multiple values in default value
                QStringList parts = defaultValueString.split(',');
                QVariantList defaultValues;
                Size i = 0;
                converted = true;
                while (converted && i < size && (int) i < parts.size()) {
                    unsigned int value = parts[i++].toUInt(&converted);
                    if (converted)
                        defaultValues << QVariant(value);
                }
                if (converted)
                    defaultValue = QVariant::fromValue(defaultValues);
                else
                    Log::error(QString("Error converting parameter default value to unsigned integer number: \"%1\"").arg(parts[i]), "Parameter::create");
            }
            break;
        case T_Float:
            if (size == 1) {
                // convert single default value
                defaultValue = QVariant::fromValue(defaultValueString.toDouble(&converted));
                if (!converted)
                    Log::error(QString("Error converting parameter default value to double-precision floating point number: \"%1\"").arg(defaultValueString), "Parameter::create");
            } else {
                // convert multiple values in default value
                QStringList parts = defaultValueString.split(',');
                QVariantList defaultValues;
                Size i = 0;
                converted = true;
                while (converted && i < size && i < parts.size()) {
                    double value = parts[i++].toDouble(&converted);
                    if (converted)
                        defaultValues << QVariant(value);
                }
                if (converted) {
                    if (defaultValues.size() == 3) {
                        // store the values in an Ogre vector instead of a QVariantList
                        Ogre::Real x = defaultValues[0].toDouble();
                        Ogre::Real y = defaultValues[1].toDouble();
                        Ogre::Real z = defaultValues[2].toDouble();
                        defaultValue = QVariant::fromValue<Ogre::Vector3>(Ogre::Vector3(x, y, z));
                    } else
                        defaultValue = QVariant::fromValue(defaultValues);
                } else
                    Log::error(QString("Error converting parameter default value to double-precision floating point number: \"%1\"").arg(parts[i]), "Parameter::create");
            }
            break;
        case T_String:
        case T_Filename:
            defaultValue = QVariant::fromValue(defaultValueString);
            break;
        case T_Color:
            {
                QColor color = decodeDoubleColor(defaultValueString);
                if (!color.isValid()) {
                    color = QColor(0, 0, 0);
                    converted = false;
                    Log::error(QString("Error converting parameter default value to color: \"%1\"").arg(defaultValueString), "Parameter::create");
                }
                defaultValue = QVariant::fromValue<QColor>(color);
            }
            break;
        case T_TextInfo:
            defaultValue = QVariant::fromValue(defaultValueString);
            break;
        case T_Image:
            defaultValue = QVariant::fromValue<Ogre::TexturePtr>(Ogre::TexturePtr(0));
            break;
    }

    // don't create a parameter if converting the parameter's default value failed
    if (!converted)
        return 0;

    // create the parameter
    Parameter *parameter = create(name, parameterType, defaultValue);
    switch (parameterType) {
        case T_Int:
        case T_UnsignedInt:
        case T_Float:
            {
                // convert the number parameter's limit and step size values
                QVariant minValue;
                QVariant maxValue;
                QVariant stepSize;
                bool minValueConverted = true;
                bool maxValueConverted = true;
                bool stepSizeConverted = true;
                QString typeDescription;
                switch (parameterType) {
                    case T_Int:
                        typeDescription = "integer number";
                        minValue = QVariant::fromValue(minValueString.toInt(&minValueConverted));
                        maxValue = QVariant::fromValue(maxValueString.toInt(&maxValueConverted));
                        stepSize = QVariant::fromValue(stepSizeString.toInt(&stepSizeConverted));
                        break;
                    case T_UnsignedInt:
                        typeDescription = "unsigned integer number";
                        minValue = QVariant::fromValue(minValueString.toUInt(&minValueConverted));
                        maxValue = QVariant::fromValue(maxValueString.toUInt(&maxValueConverted));
                        stepSize = QVariant::fromValue(stepSizeString.toUInt(&stepSizeConverted));
                        break;
                    case T_Float:
                        typeDescription = "double-precision floating point number";
                        minValue = QVariant::fromValue(minValueString.toDouble(&minValueConverted));
                        maxValue = QVariant::fromValue(maxValueString.toDouble(&maxValueConverted));
                        stepSize = QVariant::fromValue(stepSizeString.toDouble(&stepSizeConverted));
                        break;
                }
                if (!minValueConverted)
                    Log::error(QString("Error converting parameter's minimum value to %1: \"%2\"").arg(typeDescription).arg(minValueString), "Parameter::create");
                if (!maxValueConverted)
                    Log::error(QString("Error converting parameter's maximum value to %1: \"%2\"").arg(typeDescription).arg(maxValueString), "Parameter::create");
                if (!stepSizeConverted)
                    Log::error(QString("Error converting parameter's step size value to %1: \"%2\"").arg(typeDescription).arg(stepSizeString), "Parameter::create");

                // configure the number parameter
                NumberParameter *numberParameter = dynamic_cast<NumberParameter *>(parameter);
                numberParameter->setInputMethod(NumberParameter::decodeInputMethod(inputMethodString));
                numberParameter->setMinValue(minValue);
                numberParameter->setMaxValue(maxValue);
                numberParameter->setStepSize(stepSize);
                numberParameter->setUnit(unit);
            }
            break;
        case T_Filename:
            {
                // configure the filename parameter
                FilenameParameter *filenameParameter = dynamic_cast<FilenameParameter *>(parameter);
                filenameParameter->setFilters(filterString.replace(", ", ";;"));
            }
            break;
        case T_Enumeration:
            {
                // build a list of enumeration literals and values
                QStringList literals;
                QStringList values;
                QDomElement childElement = element.firstChildElement();
                while (!childElement.isNull()) {
                    if (childElement.nodeName() == "literal") {
                        literals << childElement.attribute("name");
                        values << childElement.attribute("value");
                    } else if (childElement.nodeName() == "separator") {
                        literals << EnumerationSeparator;
                        values << QString();
                    }

                    // go to next element
                    childElement = childElement.nextSiblingElement();
                }
                defaultValue = QVariant::fromValue(literals.indexOf(defaultValueString));

                // configure the enumeration parameter
                EnumerationParameter *enumerationParameter = dynamic_cast<EnumerationParameter *>(parameter);
                enumerationParameter->setLiterals(literals);
                enumerationParameter->setValues(values);
            }
            break;

		case T_PlugIn:
            {
				// configure plugin parameter
				ParameterPlugin *parameterPlugin = dynamic_cast<ParameterPlugin *>(parameter);
				parameterPlugin->setCall(inputMethodString);
			}
			break;
    }

    // set additional values for the parameter
    parameter->setSize(size);
    parameter->setMultiplicity(multiplicity);
    parameter->setDescription(description);
    parameter->setPinType(pinType);
    parameter->setVisible(visible);
    parameter->setEnabled(enabled);
    parameter->setReadOnly(readOnly);
    parameter->setSelfEvaluating(selfEvaluating);

    // set filename parameters to be read-only for now, as editing a filename in the parameter panel is not supported yet
    if (parameterType == T_Filename)
        parameter->setReadOnly(true);

    return parameter;
}


//!
//! Creates a copy of the given parameter.
//!
//! \param parameter The parameter to clone.
//! \return A new parameter with data corresponding to the given parameter.
//!
Parameter * Parameter::clone ( const Parameter &parameter )
{
    switch (parameter.getType()) {
        case T_Int:
        case T_UnsignedInt:
        case T_Float:
            return new NumberParameter(dynamic_cast<const NumberParameter &>(parameter));
        case T_Filename:
            return new FilenameParameter(dynamic_cast<const FilenameParameter &>(parameter));
        case T_Enumeration:
            return new EnumerationParameter(dynamic_cast<const EnumerationParameter &>(parameter));
        case T_Geometry:
            return new GeometryParameter(dynamic_cast<const GeometryParameter &>(parameter));
        case T_Light:
            return new LightParameter(dynamic_cast<const LightParameter &>(parameter));
        case T_Camera:
            return new CameraParameter(dynamic_cast<const CameraParameter &>(parameter));
		case T_PlugIn:
            return new ParameterPlugin(dynamic_cast<const ParameterPlugin &>(parameter));
        default:
            return new Parameter(parameter);
    }
}


//!
//! Extracts the name of first group found in the given parameter name.
//!
//! \param name The parameter name to extract the first group name from.
//! \return The name of the first group found in the given name, or an empty string if the name contains no group name.
//!
QString Parameter::extractFirstGroupName ( QString *name )
{
    if (!name)
        return "";
    QString groupName = "";

    // check if the given parameter name contains a path separator
    int posPathSeparator = name->indexOf(Parameter::PathSeparator);
    QString restOfPath = *name;
    if (posPathSeparator > -1) {
        // split the path into the first group name and the rest of the path
        groupName = name->mid(0, posPathSeparator);
        *name = name->mid(posPathSeparator + Parameter::PathSeparator.length());
    }

    return groupName;
}


//!
//! Creates an image parameter with the given name.
//!
//! \param name The name to use for the parameter.
//! \param texturePointer The texture pointer to use as the parameter's value.
//! \return A new image parameter with the given name.
//!
Parameter * Parameter::createImageParameter ( const QString &name, Ogre::TexturePtr texturePointer /* = Ogre::TexturePtr(0) */ )
{
    return new Parameter(name, Parameter::T_Image, QVariant::fromValue<Ogre::TexturePtr>(texturePointer));
}


//!
//! Creates a group parameter with the given name.
//!
//! \param name The name to use for the parameter.
//! \param parameterGroup The parameter group to use as the parameter's value.
//! \return A new group parameter with the given name.
//!
Parameter * Parameter::createGroupParameter ( const QString &name, ParameterGroup *parameterGroup /* = 0 */ )
{
    return new Parameter(name, Parameter::T_Group, QVariant::fromValue<ParameterGroup *>(parameterGroup));
}


///
/// Public Static Constants
///


//!
//! The string that identifies enumeration separators.
//!
const QString Parameter::EnumerationSeparator = "---";

//!
//! The string that separates parts in parameter paths.
//!
const QString Parameter::PathSeparator = " > ";


///
/// Constructors and Destructors
///


//!
//! Constructor of the Parameter class.
//!
//! \param name The name of the parameter.
//! \param type The type of the parameter's value(s).
//! \param value The parameter's value.
//!
Parameter::Parameter ( const QString &name, Parameter::Type type, const QVariant &value ) :
    AbstractParameter(name),
    m_mutex(QMutex::Recursive),
    m_type(type),
    m_size((value.type() == QVariant::List) ? value.toList().size() : value.canConvert<Ogre::Vector3>() ? 3 : 1),
    m_multiplicity(1),
    m_defaultValue(value),
    m_value(value),
    m_description(""),
    m_pinType(PT_None),
    m_dirty(false),
    m_auxDirty(false),
    m_visible(true),
    m_readOnly(false),
    m_selfEvaluating(false)
{

    INC_INSTANCE_COUNTER
}


//!
//! Copy constructor of the Parameter class.
//!
//! \param parameter The parameter to copy.
//!
Parameter::Parameter ( const Parameter &parameter ) :
    AbstractParameter(parameter),
    m_mutex(QMutex::Recursive),
    m_type(parameter.m_type),
    m_size(parameter.m_size),
    m_multiplicity(parameter.m_multiplicity),
    m_defaultValue(parameter.m_defaultValue),
    m_value(parameter.m_value),
    m_description(parameter.m_description),
    m_pinType(parameter.m_pinType),
    m_affectedParameters(parameter.m_affectedParameters),
    m_dirty(parameter.m_dirty),
    m_auxDirty(parameter.m_auxDirty),
    m_visible(parameter.m_visible),
    m_readOnly(parameter.m_readOnly),
    m_selfEvaluating(parameter.m_selfEvaluating)
{
    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the Parameter class.
//!
Parameter::~Parameter ()
{
    // remove the parameter from all registered connections
    QList<Connection *> connections = m_connectionMap.values();
    for (int i = 0; i < connections.size(); ++i) {
        Connection *connection = connections.at(i);
        if (connection->getTargetParameter() == this)
            connection->setTargetParameter(0);
        else
            connection->setSourceParameter(0);
		getNode()->deleteConnection(connection);
    }

    foreach(AbstractParameter *abstractParameter, m_affectedParameters) {
        Parameter *parameter = dynamic_cast<Parameter*>(abstractParameter);
        if (parameter) {
            parameter->removeAffectingParameter(this);
        }
    }

    foreach(AbstractParameter *abstractParameter, m_affectingParameters) {
        Parameter *parameter = dynamic_cast<Parameter*>(abstractParameter);
        if (parameter) {
            parameter->removeAffectedParameter(this);
        }
    }

    DEC_INSTANCE_COUNTER
}


///
/// Public Functions
///


//!
//! Creates an exact copy of the parameter.
//!
//! \return An exact copy of the parameter.
//!
AbstractParameter * Parameter::clone ()
{
    return new Parameter(*this);
}


//!
//! Returns whether the parameter object derived from this class is a
//! parameter group.
//!
//! \return True if the parameter object is a parameter group, otherwise False.
//!
bool Parameter::isGroup () const
{
    return false;
}


//!
//! Sets whether the controls for editing the parameter or parameter group
//! in the UI should be enabled.
//!
//! \param enabled The value for the parameter's enabled flag.
//!
void Parameter::setEnabled ( bool enabled )
{
	if (m_enabled != enabled) {
		m_enabled = enabled;
		emit valueChanged();
	}
}


//!
//! Returns the parameter's type.
//!
//! \return The parameter's type.
//!
Parameter::Type Parameter::getType () const
{
    return m_type;
}


//!
//! Returns the parameter's size specifying how many values of the given
//! type it stores.
//!
//! \return The parameter's size value.
//!
Parameter::Size Parameter::getSize () const
{
    return m_size;
}


//!
//! Sets the parameter's size specifying how many values of the given type
//! it stores.
//!
//! \param size The parameter's size value.
//!
void Parameter::setSize ( Parameter::Size size )
{
    if (size == m_size)
        return;

    m_size = size;
}


//!
//! Returns the parameter's multiplicity specifying how many parameters of
//! the given type can be connected to it.
//!
//! \return The parameter's multiplicity value.
//!
int Parameter::getMultiplicity () const
{
    return m_multiplicity;
}


//!
//! Sets the parameter's multiplicity specifying how many parameters of the
//! given type can be connected to it.
//!
//! \param multiplicity The parameter's multiplicity value.
//!
void Parameter::setMultiplicity ( int multiplicity )
{
    if (multiplicity == m_multiplicity)
        return;

    if (multiplicity <= M_Invalid) {
        m_multiplicity = M_Invalid;
        Log::error(QString("Invalid multiplicity value given: %1").arg(multiplicity), "Parameter::setMultiplicity");
        return;
    }

    if (multiplicity == 1)
        m_multiplicity = multiplicity;
    else if (ParameterMultiplicityTypes.contains(m_type))
        m_multiplicity = multiplicity;
    else
        Log::error(QString("A multiplicity value of more than 1 is not supported for parameters of type %1.").arg(ParameterTypeNames[m_type]), "Parameter::setMultiplicity");
}

//!
//! Returns the parameter's number of connections.
//!
//! \return The parameter's number of connections.
//!
int Parameter::getNumberOfConnections () const
{
    return m_connectionMap.size();
}

//!
//! Returns the parameter's value while optionally triggering the
//! evaluation chain.
//!
//!    \param Trigger evaluation.
//! \param triggerEvaluation Flag to control whether to trigger the evaluation chain.
//! \return The parameter's value.
//!
QVariant Parameter::getValue ( bool triggerEvaluation /* = false */ )
{
    //QMutexLocker locker(&m_mutex);
    while (!m_mutex.tryLock()) {
        int x = 5;
    }
    // optionally trigger the evaluation chain
    if (triggerEvaluation) {
        Parameter::PinType pinType = getPinType();
        if (pinType == Parameter::PT_Output || pinType == Parameter::PT_Input)
            propagateEvaluation(0);
    }
    QVariant value(m_value);
    m_mutex.unlock();
    return m_value;
}

//!
//! Returns the parameter's value list. Which contains the values of all connected
//! parameters.
//!
//! \return The parameter's value list.
//!
const QVariantList & Parameter::getValueList () const
{
    return m_valueList;
}

//!
//! Sets the parameter's value to the given value.
//!
//! \param value The new value for the parameter.
//!
void Parameter::setValue ( const QVariant &value )
{
    //QMutexLocker locker(&m_mutex);
    while (!m_mutex.tryLock()) {
        int x = 5;
    }
    // NILZ: TODO: Replace toDouble conversion to toFloat globally!?
    // check if the type of the given value is different from the type of the stored value
    //if (m_value.type() != value.type())
    //    if (m_node)
    //        Log::warning(QString("Node \"%1\": Type of value of parameter \"%2\" has changed from <%3> to <%4>.").arg(m_node->getName()).arg(m_name).arg(m_value.typeName()).arg(value.typeName()), "Parameter::setValue");
    //    else
    //        Log::warning(QString("Type of value of parameter \"%1\" has changed from <%2> to <%3>.").arg(m_name).arg(m_value.typeName()).arg(value.typeName()), "Parameter::setValue");

    // check if the value has actually changed
    if (m_value != value) {
        m_value = value;

        // notify connected objects that the parameter's value has changed
        emit valueChanged();
    }
    m_mutex.unlock();
}


//!
//! Sets the parameter's value with the given index to the given value.
//!
//! Convenience function that works on a list of values.
//!
//! \param index The index of the value to set.
//! \param value The new value for the parameter.
//!
void Parameter::setValue ( int index, const QVariant &value )
{
    //QMutexLocker locker(&m_mutex);
    while (!m_mutex.tryLock()) {
        int x = 5;
    }
    if (m_value.type() != QVariant::List && !m_value.canConvert<Ogre::Vector3>()) {
        Log::error(QString("Parameter \"%1\" does not contain a list of values.").arg(m_name), "Parameter::setValue");
        return;
    }

    if (m_value.canConvert<Ogre::Vector3>()) {
        // make sure the index into the list is valid
        if (index < 0 || index >= 3) {
            Log::error(QString("List index %1 for vector parameter \"%1\" is out of bounds.").arg(index).arg(m_name), "Parameter::setValue");
            return;
        }

        // convert the variant value to an OGRE vector
        Ogre::Vector3 v = m_value.value<Ogre::Vector3>();
        double doubleValue = value.toDouble();
        if (doubleValue != v[index]) {
            v[index] = doubleValue;
            m_value = QVariant::fromValue<Ogre::Vector3>(v);

            // notify connected objects that the parameter's value has changed
            emit valueChanged(index);
        }
    } else {
        QVariantList values = m_value.toList();

        // make sure the index into the list is valid
        if (index < 0 || index > values.size()) {
            Log::error(QString("List index %1 for parameter \"%1\" is out of bounds.").arg(index).arg(m_name), "Parameter::setValue");
            return;
        }

        // check if the value has actually changed
        if (values[index] != value) {
            values[index] = value;
            m_value = values;

            // notify connected objects that the parameter's value has changed
            emit valueChanged(index);
        }
    }
    m_mutex.unlock();
}


//!
//! Returns whether the parameter's current value is the default value.
//!
//! \return True if the parameter's current value is the default value, otherwise False.
//!
bool Parameter::hasDefaultValue () const
{
    return m_value == m_defaultValue;
}


//!
//! Sets the parameter's default value to the given value.
//!
//! \param defaultValue The default value to use for the parameter.
//!
void Parameter::setDefaultValue ( const QVariant &defaultValue )
{
    m_defaultValue = defaultValue;
}


//!
//! Resets the parameter to its default value.
//!
void Parameter::reset ()
{
    setValue(m_defaultValue);
    propagateDirty(0);
    //setDirty(true);
}


//!
//! Returns the list of parameters that are affected when the value of this
//! parameter is changed.
//!
//! \return The list of parameters affected by this parameter.
//!
AbstractParameter::List Parameter::getAffectedParameters () const
{
    return m_affectedParameters;
}


//!
//! Adds a parameter to the list of parameters that are affected when the value of this
//! parameter is changed.
//!
//! \param affectedParameter Parameter that is affected when the value of this
//! parameter is changed.
//!
void Parameter::addAffectedParameter ( Parameter *affectedParameter )
{
    if (!affectedParameter) {
        Log::warning("No affected parameter given.", "Parameter::addAffectedParameter");
        return;
    }

    if (!m_affectedParameters.contains(affectedParameter)) {
        m_affectedParameters.append(affectedParameter);
        affectedParameter->addAffectingParameter(this);
    }
}


//!
//! Removes a parameter from the list of parameters that are affected when the value of this
//! parameter is changed.
//!
//! \param affectedParameter Parameter that is affected when the value of this
//! parameter is changed.
//!
void Parameter::removeAffectedParameter ( Parameter *affectedParameter )
{
    if (!affectedParameter) {
        Log::warning("No affected parameter given.", "Parameter::removeAffectedParameter");
        return;
    }

    if (m_affectedParameters.contains(affectedParameter)) {
        m_affectedParameters.removeAt(m_affectedParameters.indexOf(affectedParameter));
        affectedParameter->removeAffectingParameter(this);
    }
}


//!
//! Returns the list of parameters that affect this parameter when their value
//! is changed.
//!
//! \return List of parameters that affect this parameter when their value
//! is changed.
//!
AbstractParameter::List Parameter::getAffectingParameters () const
{
    return m_affectingParameters;
}


//!
//! Adds a parameter to the list of parameters that are affected when the value of this
//! parameter is changed.
//!
//! \param affectingParameter Parameter that affects this parameter when its value is changed.
//!
void Parameter::addAffectingParameter ( Parameter *affectingParameter )
{
    if (!affectingParameter) {
        Log::warning("No affecting parameter given.", "Parameter::addAffectingParameter");
        return;
    }

    if (!m_affectingParameters.contains(affectingParameter)) {
        m_affectingParameters.append(affectingParameter);
        affectingParameter->addAffectedParameter(this);
    }
}


//!
//! Removes a parameter from the list of parameters that are affected when the value of this
//! parameter is changed.
//!
//! \param affectingParameter Parameter that affects this parameter when its value is changed.
//!
void Parameter::removeAffectingParameter ( Parameter *affectingParameter )
{
    if (!affectingParameter) {
        Log::warning("No affecting parameter given.", "Parameter::removeAffectingParameter");
        return;
    }

    if (m_affectingParameters.contains(affectingParameter)) {
        m_affectingParameters.removeAt(m_affectingParameters.indexOf(affectingParameter));
        affectingParameter->removeAffectedParameter(this);
    }
}


//!
//! Returns the description of the parameter.
//!
//! \return The description of the parameter.
//!
QString Parameter::getDescription () const
{
    return m_description;
}


//!
//! Sets the description for the parameter.
//!
//! \param description The description for the parameter.
//!
void Parameter::setDescription ( const QString &description )
{
    m_description = description;
}


//!
//! Returns the type of pin that should be created to represent the
//! parameter.
//!
//! \return the type of pin that should be created to represent the parameter.
//!
Parameter::PinType Parameter::getPinType () const
{
    return m_pinType;
}


//!
//! Sets the type of pin that should be created to represent the parameter.
//!
//! \param pinType The type of pin that should be created to represent the parameter.
//!
void Parameter::setPinType ( PinType pinType )
{
    m_pinType = pinType;
}


//!
//! Returns all connections currently connected to this parameter.
//!
//! \return All connections currently connected to this parameter.
//!
Connection::Map Parameter::getConnectionMap () const
{
    return m_connectionMap;
}


//!
//! Adds the given connection to the list of connections.
//!
//! \param connection The connection to add to the list of connections.
//!
void Parameter::addConnection ( Connection *connection )
{
    m_connectionMap.insert(connection->getId(), connection);

    if (m_node)
        m_node->evaluateConnection(connection);

    Parameter *parameter = connection->getSourceParameter();
    if (parameter)
        parameter->propagateDirty(0);
}


//!
//! Removes the given connection from the list of connections.
//!
//! \param id The ID of the connection to remove from the list of connections.
//!
void Parameter::removeConnection ( Connection::ID id )
{
    if (m_connectionMap.contains(id))
        m_connectionMap.remove(id);
}


//!
//! Returns whether the parameter is connected to at least one other
//! parameter.
//!
//! \return True if the parameter is connected to another parameter, otherwise False.
//!
bool Parameter::isConnected () const
{
    return !m_connectionMap.isEmpty();
}


//!
//! Returns the first parameter connected to this parameter.
//!
//! \return The first parameter connected to this parameter or Null if no parameter is connected to this parameter.
//!
Parameter * Parameter::getConnectedParameter () const
{
    Parameter *result = 0;
    if (isConnected()) {
        QList<Connection *> connections = m_connectionMap.values();
        Connection *firstConnection = connections.at(0);
        if (m_pinType == PT_Input)
            result = firstConnection->getSourceParameter();
        else
            result = firstConnection->getTargetParameter();
    }
    return result;
}


//!
//! Returns whether the parameter's value has changed.
//!
//! \return True if the parameter's value has changed, otherwise False.
//!
bool Parameter::isDirty ()
{
    QMutexLocker locker(&m_mutex);
    return m_dirty;
}


//!
//! Sets whether the parameter's value has changed.
//!
//! \param dirty The new value for the parameter's dirty flag.
//!
void Parameter::setDirty ( bool dirty )
{
    QMutexLocker locker(&m_mutex);
    m_dirty = dirty;
}

//!
//! Returns the auxiliary dirty flag.
//!
//! \return The auxiliary dirty flag.
//!
bool Parameter::isAuxDirty () {
    QMutexLocker locker(&m_mutex);
    return m_auxDirty;
}

//!
//! Sets the auxiliary dirty flag.
//!
//! \param dirty The new value for the parameter auxiliary dirty flag.
//!
void Parameter::setAuxDirty ( bool dirty ) {
    QMutexLocker locker(&m_mutex);
    m_auxDirty = dirty;
	//std::cout << m_name.toStdString() << "Dirty" << (int) m_auxDirty << std::endl;
}

//!
//! Returns the visibility for this parameter
//!
//! \return The visibility of th parameter.
//!
bool Parameter::isVisible () const
{
    return m_visible;
}


//!
//! Sets the visibility for this parameter
//!
//! \param visible The visibility of th parameter.
//!
void Parameter::setVisible ( bool visible)
{
    m_visible = visible;
}


//!
//! Returns whether the parameter can not be modified using a control in
//! the UI.
//!
//! \return True if the parameter is read-only, otherwise False.
//!
bool Parameter::isReadOnly () const
{
    return m_readOnly;
}


//!
//! Sets whether the parameter can not be modified using a control in
//! the UI.
//!
//! \param readOnly The value for the parameter's read-only flag.
//!
void Parameter::setReadOnly ( bool readOnly )
{
    m_readOnly = readOnly;
}


//!
//! Sets whether the parameter is self-evaluating.
//!
//! \param selfEvaluating True if the parameter is self-evaluating.
//!
void Parameter::setSelfEvaluating ( bool selfEvaluating )
{
    m_selfEvaluating = selfEvaluating;
}


//!
//! Returns wether the parameter is self-evaluating.
//!
//! \return True if the parameter is self-evaluating.
//!
bool Parameter::isSelfEvaluating () const
{
    return m_selfEvaluating;
}


//!
//! Sets the dirty flag for all parameters that are affected by this
//! parameter.
//!
//! \param callingNode The node calling this function.
//!
void Parameter::propagateDirty ( Node *callingNode )
{
    //while (!m_mutex.tryLock()) {
    //    int x = 45;
    //}
    if (getPinType() == PT_Input && isSelfEvaluating()) {
        setDirty(true);
        propagateEvaluation(0);
        //m_mutex.unlock();
        //return;
    }

    // mark the parameter as dirty
    setDirty(true);

    // check if the parameter is still dirty and if so notify connected objects that it has been dirtied
    if (isDirty())
        emit dirtied();

    if (m_pinType == PT_Output) {
        QList<Connection *> connections = getConnectionMap().values();
        for (int i = 0; i < connections.size(); ++i) {
            Parameter *targetParameter = connections.at(i)->getTargetParameter();
            if (targetParameter /* && !targetParameter->isDirty() */)
                // recursively propagate the dirtying
                targetParameter->propagateDirty(callingNode);
        }
    } else {
        // iterate over the list of parameters affected by this parameter
        for (int i = 0; i < m_affectedParameters.size(); ++i) {
            Parameter *parameter = dynamic_cast<Parameter *>(m_affectedParameters.at(i));
            // check if the parameter is not dirty yet
            if (parameter /* && !parameter->isDirty() */)
                // set the dirty flag for the parameter itself and all parameters affected by the parameter
                parameter->propagateDirty(callingNode);
        }
    }


    //m_mutex.unlock();
}

//!
//! Sets the aux dirty flag for all parameters that are affecting
//! this parameter.
//!
//! \param callingNode The node calling this function.
//!
void Parameter::propagateAuxDirty ()
{
	Parameter *parameter;

    // mark the parameter as dirty
    setAuxDirty(true);

    if (m_pinType == PT_Input) {
        QList<Connection *> connections = getConnectionMap().values();
        for (int i = 0; i < connections.size(); ++i) {
            parameter = connections.at(i)->getSourceParameter();
            if (parameter /* && !targetParameter->isDirty() */)
                // recursively propagate the dirtying
                parameter->propagateAuxDirty();
        }
    } 
    else if(m_pinType == PT_Output){
        // iterate over the list of parameters affecting this parameter
        for (int i = 0; i < m_affectingParameters.size(); ++i) {
            // check if the parameter is not dirty yet
            if (parameter = dynamic_cast<Parameter *> (m_affectingParameters.at(i)))
                // set the dirty flag for the parameter itself and all parameters affected by the parameter
                parameter->propagateAuxDirty();
        }
    }
}


//!
//! Propagates the evaluation of nodes.
//!
//! \param callingNode The node calling this function.
//!
void Parameter::propagateEvaluation ( Node *callingNode )
{
    //while (!m_mutex.tryLock()) {
    //    int x = 45;
    //}
    // do not evaluate if the parameter is clean
    if (!isAuxDirty() && !isDirty() && !isSelfEvaluating()) {
        //m_mutex.unlock();
        return;
    }
    if (m_pinType == PT_Input) {
        // propagate evaluation over node borders
        QVariant value;
        m_valueList.clear();
        // iterate over all source parameters connected to this parameter
        QList<Connection *> connections = getConnectionMap().values();
        for (int i = 0; i < connections.size(); ++i) {
            Parameter *sourceParameter = connections.at(i)->getSourceParameter();
            if (sourceParameter /*&& sourceParameter->isDirty()*/) {
                // recursively propagate the evaluation
                sourceParameter->propagateEvaluation(callingNode);
                // after evaluation of the preceeding node, retrieve the new calculated value
                if (isDirty()) {
                    value = sourceParameter->getValue();
                    m_valueList.append(QVariant(value));
                    if (i == 0)
                        setValue(value);
                }
            }
        }
    } else {
        // propagate evaluation within node
        // iterate over the list of parameters affecting this parameter
        for (int i = 0; i < m_affectingParameters.size(); ++i) {
            Parameter *parameter = dynamic_cast<Parameter *>(m_affectingParameters.at(i));
            // check if the parameter is dirty
            if (parameter && (parameter->isDirty() || parameter->isAuxDirty()))
                // process the affecting parameter
                parameter->propagateEvaluation(callingNode);
        }
    }

    // prevent looping when calling a parameter value getter function from within a Node, generate the value for this parameter
    //if (getPinType() == Parameter::PT_Output)
        if (isDirty())
            emit processingRequested();
		if (isAuxDirty()) {
			//std::cout << " Node: " << m_node->getName().toStdString() << "Parameter: " << this->getName().toStdString() << " Dirty: " << (int) m_auxDirty  << std::endl;
			emit auxProcessingRequested();
		}
        if (m_node)
            m_node->process(m_name);

    //// HACK: Don't undirty the current frame/time parameter
    //if (m_name != "Frame")
        // undirty this parameter
        setDirty(false);
		setAuxDirty(false);
        //m_mutex.unlock();
}


//!
//! Sets the function to call when the parameter's value changes.
//!
//! Should only be called after the parameter has been added to a parameter
//! group of a node so that the m_node member is already set.
//!
//! \param changeFunction The name of the function to call when the parameter's value changes.
//!
void Parameter::setChangeFunction ( const char *changeFunction )
{
    if (!m_node) {
        Log::warning(QString("The change function for parameter \"%1\" could not be set, because the parameter has not been added to a node yet.").arg(m_name), "Parameter::setChangeFunction");
        return;
    }

    connect(this, SIGNAL(valueChanged()), m_node, changeFunction);
    connect(this, SIGNAL(valueChanged(int)), m_node, changeFunction);
}


//!
//! Sets the function to use for computing the parameter's value.
//!
//! Should only be called after the parameter has been added to a parameter
//! group of a node so that the m_node member is already set.
//!
//! \param processingFunction The function to use for computing the parameter's value.
//!
void Parameter::setProcessingFunction ( const char *processingFunction )
{
    if (!m_node) {
        Log::warning(QString("The processing function for parameter \"%1\" could not be set, because the parameter has not been added to a node yet.").arg(m_name), "Parameter::setChangeFunction");
        return;
    }

    connect(this, SIGNAL(processingRequested()), m_node, processingFunction);
}

    //!
    //! Sets the function to use for computing auxiliary value dependant things.
    //!
    //! Should only be called after the parameter has been added to a parameter
    //! group of a node so that the m_node member is already set.
    //!
    //! \param processingFunction The name of the function to use for computing the parameter's value.
    //!
void Parameter::setAuxProcessingFunction ( const char *processingFunction )
{
    if (!m_node) {
        Log::warning(QString("The auxiliary processing function for parameter \"%1\" could not be set, because the parameter has not been added to a node yet.").arg(m_name), "Parameter::setChangeFunction");
        return;
    }

    connect(this, SIGNAL(auxProcessingRequested()), m_node, processingFunction);
}


//!
//! Sets the function to use for executing the command represented by the
//! parameter.
//!
//! Should only be called after the parameter has been added to a parameter
//! group of a node so that the m_node member is already set.
//!
//! \param commandFunction The function to use for executing the command represented by the parameter.
//!
void Parameter::setCommandFunction ( const char *commandFunction )
{
    if (!m_node) {
        Log::warning(QString("The command function for parameter \"%1\" could not be set, because the parameter has not been added to a node yet.").arg(m_name), "Parameter::setChangeFunction");
        return;
    }

    connect(this, SIGNAL(commandExecutionRequested()), m_node, commandFunction);
}


//!
//! Emits the commandExecutionRequested signal to notify connected objects
//! that the command represented by the parameter should be executed.
//!
void Parameter::executeCommand ()
{
    emit commandExecutionRequested();
}


//!
//! Returns the parameter's value as a character string.
//!
//! \return The parameter's value as a character string.
//!
QString Parameter::getValueString ()
{
    // check if the parameter's type is string-based
    if (m_type == T_String || m_type == T_TextInfo || m_type == T_Filename)
        // perform a simple string conversion
        return m_value.toString();
    else {
        // perform a string conversion depending on the parameter's type
        switch (m_type) {
            case T_Float:
				if (m_size == 1)
                    return m_value.toString();
                else if (m_value.canConvert<Ogre::Vector3>()) {
                    Ogre::Vector3 v = m_value.value<Ogre::Vector3>();
                    return QString("%1 %2 %3").arg(v.x).arg(v.y).arg(v.z);
                } else {
                    QString result = "";
                    QVariantList values = m_value.toList();
                    for (int i = 0; i < values.size(); ++i)
                        result += QString("%1 ").arg(values[i].toString());
                    return result.trimmed();
                }
                break;
        }
    }
    return m_value.toString();
}

