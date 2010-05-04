//!
//! \file "ConeTreeLayouterNode.h"
//! \brief Header file for ConeTreeLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef ConeTreeLayouterNODE_H
#define ConeTreeLayouterNODE_H

#include "VTKTreeLayoutNode.h"
#include "vtkConeLayoutStrategy.h"

//!
//! Class for a ConeTreeLayouter
//!
class ConeTreeLayouterNode : public VTKTreeLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER
	LAYOUT_TYPE(vtkConeLayoutStrategy)

public: // constructors and destructors

    //!
    //! Constructor of the ConeTreeLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ConeTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ConeTreeLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ConeTreeLayouterNode ();

private slots: //

    void setCompactness ();
    void setCompression ();
	void setSpacing ();

private: // data

	float m_Compactness;
	int m_Compression;
	float m_Spacing;
};


#endif
