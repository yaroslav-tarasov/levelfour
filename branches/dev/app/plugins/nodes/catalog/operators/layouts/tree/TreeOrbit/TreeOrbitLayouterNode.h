//!
//! \file "TreeOrbitLayouterNode.h"
//! \brief Header file for TreeOrbitLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef TreeOrbitLayouterNODE_H
#define TreeOrbitLayouterNODE_H

#include "VTKTreeLayoutNode.h"

//!
//! Class for a TreeOrbitLayouter
//!
class TreeOrbitLayouterNode : public VTKTreeLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the TreeOrbitLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    TreeOrbitLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the TreeOrbitLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~TreeOrbitLayouterNode ();

private slots: //

    void setLogSpacingValue ();
    void setLeafSpacing ();
	void setChildRadiusFactor ();

private: // data

	double m_logSpacingValue;
	double m_leafSpacing;
	double m_childRadiusFactor;
};


#endif
