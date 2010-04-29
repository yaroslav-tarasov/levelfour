//!
//! \file "SingleCycleLayouterNode.h"
//! \brief Header file for SingleCycleLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef SingleCycleLayouterNODE_H
#define SingleCycleLayouterNODE_H

#include "VTKGraphLayoutNode.h"

//!
//! Class for a SingleCycleLayouter
//!
class SingleCycleLayouterNode : public VTKGraphLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the SingleCycleLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    SingleCycleLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the SingleCycleLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~SingleCycleLayouterNode ();

private slots: //

    //!
    //! Set the single layout angular radius property
    //!
    void setAngularRadius ();

    //!
    //! Set the single layout edge weight field property
    //!
    void setEdgeWeightField ();

private: // data

    //!
    //! Angular radius
    //!
	double m_angularRadius;

    //!
    //! Edge field
    //!
	QString m_edgeWeightField;
};


#endif
