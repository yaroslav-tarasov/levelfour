/*
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/

//!
//! \file "SepiaNode.h"
//! \brief Header file for SepiaNode class.
//!

#ifndef SepiaNODE_H
#define SepiaNODE_H

#include "CompositorNode.h"


//!
//! Base class for all render nodes.
//!
class SepiaNode : public CompositorNode
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the SepiaNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    SepiaNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the SepiaNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~SepiaNode ();

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
