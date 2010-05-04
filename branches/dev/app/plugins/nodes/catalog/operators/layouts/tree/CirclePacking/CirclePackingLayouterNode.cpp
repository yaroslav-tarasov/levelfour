//!
//! \file "CirclePackingLayouterNode.cpp"
//! \brief Implementation file for CirclePackingLayouterNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#include "CirclePackingLayouterNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the CirclePackingLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CirclePackingLayouterNode::CirclePackingLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the CirclePackingLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CirclePackingLayouterNode::~CirclePackingLayouterNode ()
{
}


