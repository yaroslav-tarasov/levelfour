/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "CoronaNode.h"
//! \brief Header file for CoronaNode class.
//!

#ifndef CoronaNODE_H
#define CoronaNODE_H

#include "CompositorNode.h"


//!
//! Base class for all render nodes.
//!
class CoronaNode : public CompositorNode
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the CoronaNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    CoronaNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the CoronaNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~CoronaNode ();

protected: // functions

    //!
    //! Callback when instance of this class is registered as Ogre::CompositorListener.
    //! 
    //! \param pass_id Id to identifiy current compositor pass.
    //! \param mat Material this pass is currently using.
    //!
    virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);

private slots:

    //!
    //! Processes the node's input data to generate the node's output image.
    //!
    void processOutputImage ();

};


#endif
