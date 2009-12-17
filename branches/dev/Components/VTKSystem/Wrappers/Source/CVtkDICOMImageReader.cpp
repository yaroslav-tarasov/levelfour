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
#include "CVtkDICOMImageReader.h"

DEFINE_VTK_OBJECT(CVtkDICOMImageReader,CVtkImageReader2,vtkDICOMImageReader)
{
    pDesc->setNodeClassName("vtkDICOMImageReader");
    pDesc->setNodeClassDescription("Reads some DICOM images");
    pDesc->setNodeClassCategory("Image Readers");

    pDesc->setPropertyInfo("DirectoryName", "SetDirectoryName", "GetDirectoryName", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkDICOMImageReader::CVtkDICOMImageReader() : m_vtkDICOMImageReader(0)
{
    CVtkDICOMImageReader::InitializeObjectDesc();
    setVtkObject(vtkDICOMImageReader::New());
}

CVtkDICOMImageReader::~CVtkDICOMImageReader()
{

}

QString CVtkDICOMImageReader::directoryName() const
{
    return m_vtkDICOMImageReader->GetDirectoryName();
}


void CVtkDICOMImageReader::setDirectoryName(QString val)
{
    m_vtkDICOMImageReader->SetDirectoryName(qPrintable(val));
}

