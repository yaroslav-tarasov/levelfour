//!
//! \file "TreeLayouterNode.h"
//! \brief Header file for VTKGraphLayoutNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef TreeLayouterNode_H
#define TreeLayouterNode_H

#include "VTKTreeLayoutNode.h"
#include "vtkTreeLayoutStrategy.h"

//!
//! Class for a TreeLayouterNode
//!
class TreeLayouterNode : public VTKTreeLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER
	LAYOUT_TYPE(vtkTreeLayoutStrategy)

public: // constructors and destructors

    //!
    //! Constructor of the VTKGraphLayoutNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    TreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the VTKGraphLayoutNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~TreeLayouterNode ();

protected slots: //
	virtual void processParameters();

protected: // functions

    //!
    //! Apply the radial parameter
    //!
    void setRadial();

    //!
    //! Apply the angle parameter
    //!
    void setAngle();

private: // data

    //!
    //! The name of the radial parameter.
    //!
    QString m_radialParameterName;
	bool m_radial;

    //!
    //! The name of the angle parameter.
    //!
    QString m_angleParameterName;
	double m_angle;
};


#endif
