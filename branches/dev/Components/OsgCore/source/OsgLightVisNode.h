/****************************************************************************
**
** Copyright (C) UNO
**
** Use of this file is limited according to the terms specified by
** UNO.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef OSGLIGHTVISNODE_VIS_NODE_H
#define OSGLIGHTVISNODE_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CGenericVisNodeBase.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct OsgLightVisNodeData;
class OsgLightVisNode : public CGenericVisNodeBase
#ifdef ENABLE_ADVANCED_PROPERTIES
                       , virtual public IEditableProperties
                       , virtual public IEditablePropertiesExt
#endif
{
    Q_OBJECT
#ifdef ENABLE_ADVANCED_PROPERTIES
    Q_INTERFACES(IEditableProperties IEditablePropertiesExt)
#endif
    DECLARE_VIS_NODE

	Q_PROPERTY(int LightNum READ lightNum WRITE setLightNum)
	Q_PROPERTY(double PosX READ posX WRITE setPosX)
	Q_PROPERTY(double PosY READ posY WRITE setPosY)
	Q_PROPERTY(double PosZ READ posZ WRITE setPosZ)
	Q_PROPERTY(double PosW READ posW WRITE setPosW)
	Q_PROPERTY(double DirectionX READ directionX WRITE setDirectionX)
	Q_PROPERTY(double DirectionY READ directionY WRITE setDirectionY)
	Q_PROPERTY(double DirectionZ READ directionZ WRITE setDirectionZ)

public:
    OsgLightVisNode();
    ~OsgLightVisNode();

	int lightNum() const;
	void setLightNum(int l);
	
	double posX() const;
	void setPosX(double l);

	double posY() const;
	void setPosY(double l);

	double posZ() const;
	void setPosZ(double l);

	double posW() const;
	void setPosW(double l);

	double directionX() const;
	void setDirectionX(double l);

	double directionY() const;
	void setDirectionY(double l);

	double directionZ() const;
	void setDirectionZ(double l);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

#ifdef ENABLE_ADVANCED_PROPERTIES
public:
    // IEditableProperties implementation
    int propertyCount();
    QString propertyName(int index);
    QString propertyGroup(int index);
    QVariant propertyValue(int index);
    void setPropertyValue(int index, QVariant value);
    bool hasEditor(int index);
    QWidget* createEditor(int index);
    void setEditorValue(int index, QWidget* widget, QVariant value);
    QVariant editorValue(int index, QWidget* widget);
    void connectEditor(int index, QWidget* widget, QObject* receiver, const char* member);
    QString propertyValueString(int index);
    QVariant propertyValueFromString(int index, QString valueStr);

signals:
    void propertyConfigurationChanged();

public:
    // IEditablePropertiesExt implementation
    bool canLoadSaveProperty(int index);
#endif

protected:
    

private:
    OsgLightVisNodeData* d;
};

#endif

