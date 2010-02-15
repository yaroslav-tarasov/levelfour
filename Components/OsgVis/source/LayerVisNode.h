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

#ifndef LAYERVISNODE_VIS_NODE_H
#define LAYERVISNODE_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CGenericVisNodeBase.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct LayerVisNodeData;
class LayerVisNode : public CGenericVisNodeBase
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

	Q_PROPERTY(bool Visible READ getVisibility WRITE setVisibility)
    Q_PROPERTY(QString URL READ source WRITE setSource)
    Q_PROPERTY(QString Driver READ driver WRITE setDriver)
    Q_PROPERTY(QString Type READ type WRITE setType)
    Q_PROPERTY(QString Profile READ profile WRITE setProfile)
    Q_PROPERTY(QString Layers READ layers WRITE setLayers)
    Q_PROPERTY(QString Format READ format WRITE setFormat)
    Q_PROPERTY(QString Tile_Width READ tileWidth WRITE setTileWidth)
    Q_PROPERTY(QString Tile_Height READ tileHeight WRITE setTileHeight)
    Q_PROPERTY(QString Tile_Size READ tileSize WRITE setTileSize)
    Q_PROPERTY(QString TMS_type READ tmsType WRITE setTmsType)
    Q_PROPERTY(QString SRS READ SRS WRITE setSRS)
    Q_PROPERTY(QString DataSet READ dataSet WRITE setDataSet)
    Q_PROPERTY(QString Style READ style WRITE setStyle)
    Q_PROPERTY(QString ElevationUnit READ elevationUnit WRITE setElevationUnit)

public:
    LayerVisNode();
    ~LayerVisNode();
	void setVisibility(bool visible);
	bool getVisibility() const;

	QString source() const;
	void setSource(QString source);

	QString driver() const;
	void setDriver(QString driver);

	QString type() const;
	void setType(QString type);

	QString profile() const;
	void setProfile(QString profile);

	QString layers() const;
	void setLayers(QString layers);

	QString format() const;
	void setFormat(QString format);

	QString tileWidth() const;
	void setTileWidth(QString tileWidth);

	QString tileHeight() const;
	void setTileHeight(QString tileHeight);

	QString tileSize() const;
	void setTileSize(QString tileSize);

	QString tmsType() const;
	void setTmsType(QString tmsType);

	QString SRS() const;
	void setSRS(QString srs);

	QString dataSet() const;
	void setDataSet(QString dataSet);

	QString style() const;
	void setStyle(QString style);

	QString elevationUnit() const;
	void setElevationUnit(QString elevationUnit);

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

    Q_INVOKABLE void compile();

protected:

protected slots:
	void command_Compile();


private:
    LayerVisNodeData* d;
	bool _visible;
	QString _source;
	QString _driver;
	QString _type;
	QString _profile;
	QString _layers;
	QString _format;
	QString _tileWidth;
	QString _tileHeight;
	QString _tileSize;
	QString _tmsType;
	QString _srs;
	QString _dataSet;
	QString _elevationUnit;
	QString _style;
};

#endif

