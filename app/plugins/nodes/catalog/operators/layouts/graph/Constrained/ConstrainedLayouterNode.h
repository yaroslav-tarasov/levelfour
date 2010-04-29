//!
//! \file "ConstrainedLayouterNode.h"
//! \brief Header file for ConstrainedLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef ConstrainedLayouterNODE_H
#define ConstrainedLayouterNODE_H

#include "VTKGraphLayoutNode.h"

//!
//! Class for a ConstrainedLayouter
//!
class ConstrainedLayouterNode : public VTKGraphLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the ConstrainedLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ConstrainedLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ConstrainedLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ConstrainedLayouterNode ();

private slots: //

	//!
    //! Set the single layout angular radius property
    //!
    void setZRange ();

	//!
    //! Set the single layout angular radius property
    //!
    void setInputArrayName ();

	//!
    //! Set the single layout angular radius property
    //!
    void setRandomSeed ();

    //!
    //! Set the single layout edge weight field property
    //!
    void setMaxNumberOfIterations ();

	//!
    //! Set the single layout edge weight field property
    //!
    void setIterationsPerLayout ();

	//!
    //! Set the single layout edge weight field property
    //!
    void setInitialTemperature ();

	//!
    //! Set the single layout edge weight field property
    //!
    void setCoolDownRate ();

	//!
    //! Set the single layout edge weight field property
    //!
    void setRestDistance ();

private: // data

    double m_zRange;
	int m_randomSeed;
	int m_maxIterations;
	int m_layoutIterations;
	float m_initialTemperature;
	double m_coolDownRate;
	float m_restDistance;
	QString m_inputArrayName;


};


#endif