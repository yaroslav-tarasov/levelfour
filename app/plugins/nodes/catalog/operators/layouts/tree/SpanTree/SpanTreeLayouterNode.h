//!
//! \file "SpanTreeLayouterNode.h"
//! \brief Header file for SpanTreeLayouterNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef SpanTreeLayouterNODE_H
#define SpanTreeLayouterNODE_H

#include "VTKTreeLayoutNode.h"
#include "vtkSpanTreeLayoutStrategy.h"

//!
//! Class for a SpanTreeLayouter
//!
class SpanTreeLayouterNode : public VTKTreeLayoutNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER
	LAYOUT_TYPE(vtkSpanTreeLayoutStrategy)

public: // constructors and destructors

    //!
    //! Constructor of the SpanTreeLayouterNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    SpanTreeLayouterNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the SpanTreeLayouterNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~SpanTreeLayouterNode ();

private slots: //

    void setDepthFirstSpanningTree ();

private: // data

	bool m_depthFirst;
};


#endif
