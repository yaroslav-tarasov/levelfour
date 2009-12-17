/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef C_VTK_PROPERTY_H
#define C_VTK_PROPERTY_H

#include "CVtkObject.h"
#include "vtkOpenGLProperty.h"
#include "CVtkPropertyData.h"

class CVtkProperty : public CVtkObject
{
    Q_OBJECT
    Q_ENUMS(Interpolation)
    Q_ENUMS(Representation)
    Q_PROPERTY(Interpolation Interpolation READ interpolation WRITE setInterpolation)
    Q_PROPERTY(Representation Representation READ representation WRITE setRepresentation)
    Q_PROPERTY(QColor Color READ color WRITE setColor)
    Q_PROPERTY(double Ambient READ ambient WRITE setAmbient)
    Q_PROPERTY(double Diffuse READ diffuse WRITE setDiffuse)
    Q_PROPERTY(double Specular READ specular WRITE setSpecular)
    Q_PROPERTY(double SpecularPower READ specularPower WRITE setSpecularPower)
    Q_PROPERTY(double Opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QColor AmbientColor READ ambientColor WRITE setAmbientColor)
    Q_PROPERTY(QColor DiffuseColor READ diffuseColor WRITE setDiffuseColor)
    Q_PROPERTY(QColor SpecularColor READ specularColor WRITE setSpecularColor)
    Q_PROPERTY(bool EdgeVisibility READ isEdgeVisibility WRITE setEdgeVisibility)
    Q_PROPERTY(QColor EdgeColor READ edgeColor WRITE setEdgeColor)
    Q_PROPERTY(double LineWidth READ lineWidth WRITE setLineWidth)
    Q_PROPERTY(int LineStipplePattern READ lineStipplePattern WRITE setLineStipplePattern)
    Q_PROPERTY(int LineStippleRepeatFactor READ lineStippleRepeatFactor WRITE setLineStippleRepeatFactor)
    Q_PROPERTY(double PointSize READ pointSize WRITE setPointSize)
    Q_PROPERTY(bool BackfaceCulling READ isBackfaceCulling WRITE setBackfaceCulling)
    Q_PROPERTY(bool FrontfaceCulling READ isFrontfaceCulling WRITE setFrontfaceCulling)
    DECLARE_VTK_OBJECT

public:
    enum Interpolation
    {
        Flat=VTK_FLAT,
        Gouraud=VTK_GOURAUD,
        Phong=VTK_PHONG
    };

    enum Representation
    {
        Points=VTK_POINTS,
        Wireframe=VTK_WIREFRAME,
        Surface=VTK_SURFACE
    };

    CVtkProperty();
    ~CVtkProperty();

    Interpolation interpolation() const;
    void setInterpolation(Interpolation val);

    Representation representation() const;
    void setRepresentation(Representation val);

    QColor color() const;
    void setColor(QColor val);

    double ambient() const;
    void setAmbient(double val);

    double diffuse() const;
    void setDiffuse(double val);

    double specular() const;
    void setSpecular(double val);

    double specularPower() const;
    void setSpecularPower(double val);

    double opacity() const;
    void setOpacity(double val);

    QColor ambientColor() const;
    void setAmbientColor(QColor val);

    QColor diffuseColor() const;
    void setDiffuseColor(QColor val);

    QColor specularColor() const;
    void setSpecularColor(QColor val);

    bool isEdgeVisibility() const;
    void setEdgeVisibility(bool val);

    QColor edgeColor() const;
    void setEdgeColor(QColor val);

    double lineWidth() const;
    void setLineWidth(double val);

    int lineStipplePattern() const;
    void setLineStipplePattern(int val);

    int lineStippleRepeatFactor() const;
    void setLineStippleRepeatFactor(int val);

    double pointSize() const;
    void setPointSize(double val);

    bool isBackfaceCulling() const;
    void setBackfaceCulling(bool val);

    bool isFrontfaceCulling() const;
    void setFrontfaceCulling(bool val);

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    vtkProperty* m_vtkProperty;
    CVtkPropertyData m_vtkPropertyData;
};

class vtkProperty2 : public vtkOpenGLProperty
{
public:
    static vtkProperty2* New() {
        return new vtkProperty2();
    }

    void SetSpecularPower2(double val) {
        if(val > 128)
            this->SpecularPower = 128;
        else if(val < 0)
            this->SpecularPower = 0;
        else
            this->SpecularPower = val;
        this->Modified();
    }

protected:
    vtkProperty2() { }
    ~vtkProperty2() { }
};

#endif
