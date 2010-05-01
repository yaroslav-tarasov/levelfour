#ifndef XmlTreeReaderNODE_H
#define XmlTreeReaderNODE_H

#include "vtkXMLTreeReader.h"
#include "VTKTreeNode.h"
#include <QString>
//!
//! Class for a node that is used for debugging purposes.
//!
class XmlTreeReaderNode : public VTKTreeNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the XmlTreeReaderNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    XmlTreeReaderNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the XmlTreeReaderNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~XmlTreeReaderNode ();

public slots: //

private: // functions

	//!
    //! Loads the source file.
    //!
    //! \return True if the file was successfully loaded, otherwise False.
    //!
    bool loadFile ();

	//!
    //! Update the vtk table associated with the text delimited source.
    //!
    //! \return void.
    //!
	void updateTree();

private slots: //

    //!
    //! Change function for the Source File parameter.
    //!
    void sourceFileChanged ();

private: // data

	vtkXMLTreeReader * m_xmltree;
};

#endif