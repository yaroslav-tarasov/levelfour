//!
//! \file "ForceDirectedLayouterNode.h"
//! \brief Header file for ForceDirectedLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef ForceDirectedLayouterNODE_H
#define ForceDirectedLayouterNODE_H

#include "VTKGraphLayoutNode.h"
#include "vtkForceDirectedLayoutStrategy.h"

//!
//! Class for a ForceDirectedLayouter
//!
class ForceDirectedLayouterNode : public VTKGraphLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER
	LAYOUT_TYPE(vtkForceDirectedLayoutStrategy)

public: // constructors and destructors

    //!
    //! Constructor of the ForceDirectedLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ForceDirectedLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ForceDirectedLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ForceDirectedLayouterNode ();

protected slots: //
	virtual void processParameters();

private slots: //

	//!
    //! Set the single layout angular radius property
    //!
    void setThreeDimensionalLayout ();

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
    void setRandomInitialPoints ();

private: // data

    int m_threeD;
	int m_randomSeed;
	int m_maxIterations;
	int m_layoutIterations;
	float m_initialTemperature;
	double m_coolDownRate;
	int m_randomPoints;



};


#endif
