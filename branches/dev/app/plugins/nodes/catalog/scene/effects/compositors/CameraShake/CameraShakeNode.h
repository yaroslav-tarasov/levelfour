/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "CameraShakeNode.h"
//! \brief Header file for CameraShakeNode class.
//!

#ifndef CameraShakeNODE_H
#define CameraShakeNODE_H

#include "CompositorNode.h"


//!
//! Base class for all render nodes.
//!
class CameraShakeNode : public CompositorNode
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the CameraShakeNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    CameraShakeNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the CameraShakeNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~CameraShakeNode ();

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
