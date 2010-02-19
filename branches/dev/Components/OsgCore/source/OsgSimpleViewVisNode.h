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

#ifndef OSGSIMPLEVIEWVISNODE_VIS_NODE_H
#define OSGSIMPLEVIEWVISNODE_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CGenericVisNodeBase.h"
#include <QTabWidget>
#include <QTimer>

#include "QOSGWidget.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct OsgSimpleViewVisNodeData;
class OsgSimpleViewVisNode : public CGenericVisNodeBase
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

public:
    OsgSimpleViewVisNode();
    ~OsgSimpleViewVisNode();

	void toggleXYGrid(bool enable);
	void addNode(osg::Node * node);
	void removeNode(osg::Node * node);
	bool containsNode(osg::Node * node);

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

    Q_INVOKABLE void render();
    Q_INVOKABLE void EarthManipulator();
    Q_INVOKABLE void saveOSG();

protected:

protected slots:
	void command_Render();
	void command_EarthManipulator();
	void command_SaveOSG();

private:
    OsgSimpleViewVisNodeData* d;

	QTimer timer;
};

#endif

