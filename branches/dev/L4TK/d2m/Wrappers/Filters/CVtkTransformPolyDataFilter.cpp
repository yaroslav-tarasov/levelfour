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

#include "CVtkTransformPolyDataFilter.h"
#include "vtkTransform.h"

DEFINE_VTK_OBJECT(CVtkTransformPolyDataFilter, CVtkPolyDataFilter, vtkTransformPolyDataFilter)
{
    pDesc->setNodeClassName("vtkTransformPolyDataFilter");
    pDesc->setNodeClassDescription("Transform PolyData filter");
}

CVtkTransformPolyDataFilter::CVtkTransformPolyDataFilter() : m_vtkTransformPolyDataFilter(0)
{
    CVtkTransformPolyDataFilter::InitializeObjectDesc();
    setVtkObject(vtkTransformPolyDataFilter::New());

    m_vtkTransform = vtkTransform::New();
    m_vtkTransformPolyDataFilter->SetTransform(m_vtkTransform);
    m_vtkTransform->Delete();
}

CVtkTransformPolyDataFilter::~CVtkTransformPolyDataFilter()
{

}

void CVtkTransformPolyDataFilter::translate(Point3D p)
{
    m_vtkTransform->Translate(p.x, p.y, p.z);
}

void CVtkTransformPolyDataFilter::translate(double x, double y, double z)
{
    m_vtkTransform->Translate(x, y, z);
}

void CVtkTransformPolyDataFilter::rotateX(double angle)
{
    m_vtkTransform->RotateX(angle);
}

void CVtkTransformPolyDataFilter::rotateY(double angle)
{
    m_vtkTransform->RotateY(angle);
}

void CVtkTransformPolyDataFilter::rotateZ(double angle)
{
    m_vtkTransform->RotateZ(angle);
}

void CVtkTransformPolyDataFilter::scale(Point3D p)
{
    m_vtkTransform->Scale(p.x, p.y, p.z);
}

void CVtkTransformPolyDataFilter::scale(double x, double y, double z)
{
    m_vtkTransform->Scale(x, y, z);
}

void CVtkTransformPolyDataFilter::preMultiply()
{
    m_vtkTransform->PreMultiply();
}

void CVtkTransformPolyDataFilter::postMultiply()
{
    m_vtkTransform->PostMultiply();
}

void CVtkTransformPolyDataFilter::makeIdentity()
{
    m_vtkTransform->Identity();
}

void CVtkTransformPolyDataFilter::concatenate(QList<double> elements)
{
    QVector<double> eVec = elements.toVector();
    m_vtkTransform->Concatenate(eVec.data());
}

void CVtkTransformPolyDataFilter::setMatrix(QList<double> matrix)
{
    QVector<double> eVec = matrix.toVector();
    m_vtkTransform->SetMatrix( eVec.data() );
}

QList<double> CVtkTransformPolyDataFilter::getMatrix()
{
    QList<double> ret;
    vtkMatrix4x4* matrix = m_vtkTransform->GetMatrix();
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            ret << matrix->Element[i][j];
    return ret;
}



