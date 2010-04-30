//!
//! \file "RadialTreeLayouterNode.h"
//! \brief Header file for RadialTreeLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef RadialTreeLayouterNODE_H
#define RadialTreeLayouterNODE_H

#include "VTKTreeLayoutNode.h"

//!
//! Class for a RadialTreeLayouter
//!
class RadialTreeLayouterNode : public VTKTreeLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the RadialTreeLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    RadialTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the RadialTreeLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~RadialTreeLayouterNode ();

private slots: //

    void setZRange ();
    void setAngle ();
	void setLogSpacingValue ();
	void setLeafSpacing ();
	void setDistanceArrayName ();
	void setRotation ();

private: // data

	double m_zRange;
	double m_Angle;
	double m_logSpacingValue;
	double m_leafSpacing;
	QString m_distanceArrayName;
	double m_Rotation;
};


#endif
