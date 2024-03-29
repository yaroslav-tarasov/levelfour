#ifndef VTKTABLENODE_H
#define VTKTABLENODE_H

#include "vtkTable.h"
#include "Node.h"

Q_DECLARE_METATYPE(vtkTable *)

//!
//! Base class representing nodes that provide a vtkTable output.
//!
class FRAPPER_CORE_EXPORT VTKTableNode : public Node
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the VTKTableNode class.
    //!
    //! \param name The name to give the new mesh node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    VTKTableNode ( const QString &name, ParameterGroup *parameterRoot, const QString &outputVTKTableName /* = "VTKTable" */ );

    //!
    //! Destructor of the VTKTableNode class.
    //!
    ~VTKTableNode ();

    //!
    //! Return the current vtk table associated with this node.
    //!
	vtkTable * getTable() const;

signals:
    //!
    //! Signal that is emitted when the vtk table is changed.
    //!
	void tableChanged(vtkTable * table);
	void destroyed();

private slots: //

protected: // functions
	//!
    //! Update the vtk table associated with the text delimited source.
    //!
    //! \return void.
    //!
	virtual void updateTable() { emit tableChanged(m_table); }

protected: // data

	vtkTable * m_table;

    //!
    //! The name of the vtk table output parameter.
    //!
	QString m_outputVTKTableName; 

};


#endif
