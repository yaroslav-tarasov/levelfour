//!
//! \file "CompositorTemplateNode.h"
//! \brief Header file for CompositorTemplateNode class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       16.10.2009 (last updated)
//!

#ifndef COMPOSITORTEMPLATENODE_H
#define COMPOSITORTEMPLATENODE_H

#include "CompositorNode.h"
#include "InstanceCounterMacros.h"


//!
//! Class implementing Screen Space Ambient Occlusion operations.
//!
class CompositorTemplateNode : public CompositorNode
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the CompositorTemplateNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    CompositorTemplateNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the CompositorTemplateNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~CompositorTemplateNode ();

protected: // functions

    //!
    //! Callback when instance of this class is registered as Ogre::CompositorListener.
    //! 
    //! \param pass_id Id to identifiy current compositor pass.
    //! \param mat Material this pass is currently using.
    //!
    virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);

public slots:
	//!
    //! Redraw of ogre scene has been triggered.
    //!
	virtual void redrawTriggered ();

private slots:

    //!
    //! Processes the node's input data to generate the node's output image.
    //!
    void processOutputImage ();

};

#endif
