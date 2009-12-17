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

#include "CVtkProperty.h"

DEFINE_VTK_OBJECT(CVtkProperty, CVtkObject, vtkProperty)
{
    pDesc->setNodeClassName("vtkProperty");
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassDescription("Rendering attributes for an actor");

    pDesc->setPropertyInfo("Interpolation", "SetInterpolation", "GetInterpolation", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Representation", "SetRepresentation", "GetRepresentation", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Color", "SetColor", "GetColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Ambient", "SetAmbient", "GetAmbient", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Diffuse", "SetDiffuse", "GetDiffuse", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Specular", "SetSpecular", "GetSpecular", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("SpecularPower", "SetSpecularPower", "GetSpecularPower", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Opacity", "SetOpacity", "GetOpacity", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("AmbientColor", "SetAmbientColor", "GetAmbientColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("DiffuseColor", "SetDiffuseColor", "GetDiffuseColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("SpecularColor", "SetSpecularColor", "GetSpecularColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("EdgeVisibility", "SetEdgeVisibility", "GetEdgeVisibility", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("EdgeColor", "SetEdgeColor", "GetEdgeColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("LineWidth", "SetLineWidth", "GetLineWidth", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("LineStipplePattern", "SetLineStipplePattern", "GetLineStipplePattern", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("LineStippleRepeatFactor", "SetLineStippleRepeatFactor", "GetLineStippleRepeatFactor", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("PointSize", "SetPointSize", "GetPointSize", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("BackfaceCulling", "SetBackfaceCulling", "GetBackfaceCulling", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("FrontfaceCulling", "SetFrontfaceCulling", "GetFrontfaceCulling", QStringList() << "int" , QStringList(), "void", "int");

    // Property Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Property", IVisSystemNodeConnectionPath::OutputPath, "vtkProperty", 0)
        );
}

CVtkProperty::CVtkProperty() : m_vtkProperty(0)
{
    CVtkProperty::InitializeObjectDesc();
    setVtkObject(vtkProperty2::New());
}

CVtkProperty::~CVtkProperty()
{

}

CVtkProperty::Interpolation CVtkProperty::interpolation() const
{
    switch(m_vtkProperty->GetInterpolation())
    {
        case VTK_FLAT:return (Flat);
        case VTK_GOURAUD:return (Gouraud);
        case VTK_PHONG:return (Phong);
        default :return (Flat);
    }
}

void CVtkProperty::setInterpolation(Interpolation val)
{
    switch(val)
    {
        case Flat:
            m_vtkProperty->SetInterpolation(VTK_FLAT);
            break;
        case Gouraud:
            m_vtkProperty->SetInterpolation(VTK_GOURAUD);
            break;
        case Phong:
            m_vtkProperty->SetInterpolation(VTK_PHONG);
            break;
        default :
            m_vtkProperty->SetInterpolation(VTK_FLAT);
            break;
    }
}


CVtkProperty::Representation CVtkProperty::representation() const
{
    switch(m_vtkProperty->GetRepresentation())
    {
        case VTK_POINTS:return (Points);
        case VTK_WIREFRAME:return (Wireframe);
        case VTK_SURFACE:return (Surface);
        default :return (Points);
    }
}

void CVtkProperty::setRepresentation(Representation val)
{
    switch(val)
    {
        case Points:
            m_vtkProperty->SetRepresentation(VTK_POINTS);
            break;
        case Wireframe:
            m_vtkProperty->SetRepresentation(VTK_WIREFRAME);
            break;
        case Surface:
            m_vtkProperty->SetRepresentation(VTK_SURFACE);
            break;
        default :
            m_vtkProperty->SetRepresentation(VTK_POINTS);
            break;
    }
}


QColor CVtkProperty::color() const
{
    QColor retColor;
    double* color = m_vtkProperty->GetColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}


void CVtkProperty::setColor(QColor val)
{
    m_vtkProperty->SetColor(val.redF(), val.greenF(), val.blueF());
}


double CVtkProperty::ambient() const
{
    return m_vtkProperty->GetAmbient();
}


void CVtkProperty::setAmbient(double val)
{
    m_vtkProperty->SetAmbient(val);
}


double CVtkProperty::diffuse() const
{
    return m_vtkProperty->GetDiffuse();
}


void CVtkProperty::setDiffuse(double val)
{
    m_vtkProperty->SetDiffuse(val);
}


double CVtkProperty::specular() const
{
    return m_vtkProperty->GetSpecular();
}


void CVtkProperty::setSpecular(double val)
{
    m_vtkProperty->SetSpecular(val);
}


double CVtkProperty::specularPower() const
{
    return m_vtkProperty->GetSpecularPower();
}


void CVtkProperty::setSpecularPower(double val)
{
    ((vtkProperty2*)(m_vtkProperty))->SetSpecularPower2(val);
}


double CVtkProperty::opacity() const
{
    return m_vtkProperty->GetOpacity();
}


void CVtkProperty::setOpacity(double val)
{
    m_vtkProperty->SetOpacity(val);
}


QColor CVtkProperty::ambientColor() const
{
    QColor retColor;
    double* color = m_vtkProperty->GetAmbientColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}


void CVtkProperty::setAmbientColor(QColor val)
{
    m_vtkProperty->SetAmbientColor(val.redF(), val.greenF(), val.blueF());
}


QColor CVtkProperty::diffuseColor() const
{
    QColor retColor;
    double* color = m_vtkProperty->GetDiffuseColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}


void CVtkProperty::setDiffuseColor(QColor val)
{
    m_vtkProperty->SetDiffuseColor(val.redF(), val.greenF(), val.blueF());
}


QColor CVtkProperty::specularColor() const
{
    QColor retColor;
    double* color = m_vtkProperty->GetSpecularColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}


void CVtkProperty::setSpecularColor(QColor val)
{
    m_vtkProperty->SetSpecularColor(val.redF(), val.greenF(), val.blueF());
}


bool CVtkProperty::isEdgeVisibility() const
{
    return m_vtkProperty->GetEdgeVisibility();
}


void CVtkProperty::setEdgeVisibility(bool val)
{
    m_vtkProperty->SetEdgeVisibility(val);
}


QColor CVtkProperty::edgeColor() const
{
    QColor retColor;
    double* color = m_vtkProperty->GetEdgeColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}


void CVtkProperty::setEdgeColor(QColor val)
{
    m_vtkProperty->SetEdgeColor(val.redF(), val.greenF(), val.blueF());
}


double CVtkProperty::lineWidth() const
{
    return m_vtkProperty->GetLineWidth();
}


void CVtkProperty::setLineWidth(double val)
{
    m_vtkProperty->SetLineWidth(val);
}


int CVtkProperty::lineStipplePattern() const
{
    return m_vtkProperty->GetLineStipplePattern();
}


void CVtkProperty::setLineStipplePattern(int val)
{
    m_vtkProperty->SetLineStipplePattern(val);
}


int CVtkProperty::lineStippleRepeatFactor() const
{
    return m_vtkProperty->GetLineStippleRepeatFactor();
}


void CVtkProperty::setLineStippleRepeatFactor(int val)
{
    m_vtkProperty->SetLineStippleRepeatFactor(val);
}


double CVtkProperty::pointSize() const
{
    return m_vtkProperty->GetPointSize();
}


void CVtkProperty::setPointSize(double val)
{
    m_vtkProperty->SetPointSize(val);
}


bool CVtkProperty::isBackfaceCulling() const
{
    return m_vtkProperty->GetBackfaceCulling();
}


void CVtkProperty::setBackfaceCulling(bool val)
{
    m_vtkProperty->SetBackfaceCulling(val);
}


bool CVtkProperty::isFrontfaceCulling() const
{
    return m_vtkProperty->GetFrontfaceCulling();
}


void CVtkProperty::setFrontfaceCulling(bool val)
{
    m_vtkProperty->SetFrontfaceCulling(val);
}

bool CVtkProperty::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Property")
    {
        m_vtkPropertyData.setProperty(m_vtkProperty);
        *outputData = &m_vtkPropertyData;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkProperty::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return CVtkObject::outputDerefed(path, outputData);
}


