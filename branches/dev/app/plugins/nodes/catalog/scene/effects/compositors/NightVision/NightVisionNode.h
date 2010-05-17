/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "NightVisionNode.h"
//! \brief Header file for NightVisionNode class.
//!


#ifndef NightVisionNODE_H
#define NightVisionNODE_H

#include "CompositorNode.h"


//!
//! Base class for all render nodes.
//!
class NightVisionNode : public CompositorNode
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the NightVisionNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    NightVisionNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the NightVisionNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~NightVisionNode ();

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

    //!
    //! Marks the cache as being invalid so that it is cleared the next time
    //! the output image is processed.
    //!
    void invalidateCache ();

};


#endif
