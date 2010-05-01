#ifndef TextDelimiterSourceNODE_H
#define TextDelimiterSourceNODE_H

#include "vtkDelimitedTextReader.h"
#include "VTKTableNode.h"
#include <QString>
//!
//! Class representing nodes that can load delimited text files into a vtk table.
//!
class TextDelimiterSourceNode : public VTKTableNode
{

    Q_OBJECT
	ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the TextDelimiterSourceNode class.
    //!
    //! \param name The name to give the new mesh node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    TextDelimiterSourceNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the TextDelimiterSourceNode class.
    //!
    ~TextDelimiterSourceNode ();

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
	void updateTable();

private slots: //

    //!
    //! Change function for the Source File parameter.
    //!
    void sourceFileChanged ();

    //!
    //! Change function for the fieldDelimiter parameter.
    //!
    void fieldDelimiterChanged ();

    //!
    //! Change function for the String Delimiter parameter.
    //!
    void stringDelimiterChanged ();

    //!
    //! Change function for the Use String Delimiter parameter.
    //!
    void useStringDelimiterChanged ();

    //!
    //! Change function for the Have Header parameter.
    //!
    void haveHeaderChanged ();

    //!
    //! Change function for the Merge Consecutive Delimiters parameter.
    //!
    void mergeConsecutiveDelimitersChanged ();

    //!
    //! Change function for the MaxRecords parameter.
    //!
    void maxRecordsChanged ();

    //!
    //! Change function for the Detect Numeric Columns parameter.
    //!
    void detectNumericColumnsChanged ();

private: // data

	vtkDelimitedTextReader * m_dtxt;
};


#endif
