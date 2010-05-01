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
//! \file "XmlTreeReaderNode.cpp"
//! \brief Implementation file for XmlTreeReaderNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#include "XmlTreeReaderNode.h"
#include "vtkTree.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "SceneNodeParameter.h"
#include "FilenameParameter.h"
#include <QtCore/QFile>
#include <QtCore/QDir>
#include "VTKTreeParameter.h"

INIT_INSTANCE_COUNTER(XmlTreeReaderNode)


///
/// Constructors and Destructors
///


//!
//! Constructor of the XmlTreeReaderNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
XmlTreeReaderNode::XmlTreeReaderNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKTreeNode (name, parameterRoot, "VTKTree"),
		m_xmltree(0)
{
    // set affections and functions
    setChangeFunction("Source File", SLOT(sourceFileChanged()));
    setCommandFunction("Source File", SLOT(sourceFileChanged()));

	connect(this, SIGNAL(frameChanged(int)), SLOT(updateAll()));

	setTypeName("XmlTreeReaderNode");

    INC_INSTANCE_COUNTER
}


//!
//! Destructor of the XmlTreeReaderNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
XmlTreeReaderNode::~XmlTreeReaderNode ()
{
	emit destroyed();
	if (m_xmltree && m_xmltree->GetOutput()) 
	{
		m_xmltree->Delete();
		m_Tree = 0;
	}
	DEC_INSTANCE_COUNTER
    Log::info(QString("XmlTreeReaderNode destroyed."), "XmlTreeReaderNode::~XmlTreeReaderNode");
}

///
/// Private Functions
///

//!
//!
//! \return True, if successful loading of ogre mesh
//!            False, otherwise.
//!
bool XmlTreeReaderNode::loadFile()
{
    QString filename = getStringValue("Source File");
    if (filename == "") {
        Log::debug(QString("Source file has not been set yet. (\"%1\")").arg(m_name), "XmlTreeReaderNode::loadFile");
        return false;
    }

    // check if the file exists
    if (!QFile::exists(filename)) {
        Log::error(QString("Source file \"%1\" not found.").arg(filename), "XmlTreeReaderNode::loadFile");
        return false;
    }

	if (!filename.endsWith(".xml") && !filename.endsWith(".txt")) {
        Log::error("The source file has to be a csv or txt.", "XmlTreeReaderNode::loadFile");
        return false;
    }

	// if any, delete previous file
	if (m_xmltree)
		m_xmltree->Delete();

	// create and load new file
	m_xmltree = vtkXMLTreeReader::New();
	m_xmltree->SetFileName(filename.toLatin1().constData());
	m_xmltree->Update();
	updateTree();

    Log::info(QString("Source file \"%1\" loaded.").arg(filename), "XmlTreeReaderNode::loadFile");
	// Log::info(QString("Number rows \"%1\" loaded: ").arg(m_Tree->GetNumberOfRows()), "XmlTreeReaderNode::loadFile");
    return true;
}

///
/// Private Slots
///

//!
//! Change function for the Source File parameter.
//!
void XmlTreeReaderNode::sourceFileChanged ()
{
    // load new source file
    loadFile();
}

void XmlTreeReaderNode::updateTree()
{
	m_xmltree->Update();
	m_Tree = m_xmltree->GetOutput();

	VTKTreeParameter * outParameter = dynamic_cast<VTKTreeParameter*>(getParameter(m_outputVTKTreeName));
	outParameter->setVTKTree(m_Tree);
	outParameter->propagateDirty(0);
//	setValue(m_outputVTKTableName, QVariant::fromValue<vtkTable *>(m_Tree), true);

	emit treeChanged(m_Tree);
}
