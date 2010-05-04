//!
//! \file "ClusteringLayouterNode.h"
//! \brief Header file for ClusteringLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef ClusteringLayouterNODE_H
#define ClusteringLayouterNODE_H

#include "VTKGraphLayoutNode.h"
#include "vtkClustering2DLayoutStrategy.h"

//!
//! Class for a ClusteringLayouter
//!
class ClusteringLayouterNode : public VTKGraphLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER
	LAYOUT_TYPE(vtkClustering2DLayoutStrategy)

public: // constructors and destructors

    //!
    //! Constructor of the ClusteringLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ClusteringLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ClusteringLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ClusteringLayouterNode ();

private slots: //

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

    int m_randomSeed;
	int m_maxIterations;
	int m_layoutIterations;
	float m_initialTemperature;
	double m_coolDownRate;
	float m_restDistance;

};


#endif
