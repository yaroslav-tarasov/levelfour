//!
//! \file "ThreeDCirclesLayouterNode.h"
//! \brief Header file for ThreeDCirclesLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef ThreeDCirclesLayouterNODE_H
#define ThreeDCirclesLayouterNODE_H

#include "VTKGraphLayoutNode.h"

//!
//! Class for a ThreeDCirclesLayouter
//!
class ThreeDCirclesLayouterNode : public VTKGraphLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the ThreeDCirclesLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    ThreeDCirclesLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the ThreeDCirclesLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~ThreeDCirclesLayouterNode ();

private slots: //

    void setMethod ();
    void setRadius ();
    void setHeight ();
    void setMarkedStartVertices ();
    void setMarkedValue ();
    void setForceToUseUniversalStartPointsFinder ();
    void setAutoHeight ();
    void setMinimumRadian ();
    void setMinimumDegree ();
	void setHierarchicalLayers ();

private: // data

    int m_setMethod;
	double m_setRadius;
	double m_setHeight;
	QString m_setMarkedStartVertices;
	int m_setMarkedValue;
	int m_setForceToUseUniversalStartPointsFinder;
	int m_AutoHeight;
	double m_MinimumRadian;
	double m_MinimumDegree;
	QString m_setHierarchicalLayers;


};


#endif
