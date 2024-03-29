/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "NodeModel.h"
//! \brief Header file for NodeModel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       02.06.2009 (last updated)
//!

#ifndef NODEMODEL_H
#define NODEMODEL_H

#include "FrapperPrerequisites.h"
#include <QtGui/QStandardItemModel>
#include <QtGui/QGraphicsItem>
#include <QtGui/QItemSelection>
#include <QtCore/QMutex>
#include "Node.h"


//!
//! Class for the model containing nodes.
//!
//! <div align="center"><b>Inheritance Diagram</b></div>
//!
//! \dot
//!   digraph NodeModel {
//!     node [fontname="FreeSans",fontsize="10",shape="box",height=0.2,width=0.4,fontname="FreeSans",color="black",style="filled",fillcolor="white"];
//!     edge [fontname="FreeSans",fontsize="10",labelfontname="FreeSans",labelfontsize="10",dir=back,style="solid",color="midnightblue"];
//!
//!     NodeModel [label="NodeModel",fillcolor="grey75"];
//!     QStandardItemModel -> NodeModel;
//!     QStandardItemModel [label="QStandardItemModel",fontcolor="white",fillcolor="#66b036",URL="http://doc.qtsoftware.com/4.5/qstandarditemModel.html"];
//!     QAbstractItemModel -> QStandardItemModel;
//!     QAbstractItemModel [label="QAbstractItemModel",fontcolor="white",fillcolor="#66b036",URL="http://doc.qtsoftware.com/4.5/qabstractitemmodel.html"];
//!     QObject -> QAbstractItemModel;
//!     QObject [label="QObject",fontcolor="white",fillcolor="#66b036",URL="http://doc.qtsoftware.com/4.5/qobject.html"];
//!   }
//! \enddot
//! <div><center>[<a href="graph_legend.html">legend</a>]</center></div>
//!
class FRAPPER_CORE_EXPORT NodeModel : public QStandardItemModel
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the NodeModel class.
    //!
    NodeModel ();

    //!
    //! Destructor of the NodeModel class.
    //!
    ~NodeModel ();

public: // functions

    //!
    //! Sets an updating flag for the node model to prevent emitting redundant
    //! updated() signals.
    //!
    //! Should be called to initiate a substantial update of the model.
    //!
    //! \see endUpdate()
    //!
    void beginUpdate ();

    //!
    //! Resets the updating flag for the node model and emits the updated()
    //! signal.
    //!
    //! Should be called when a substantial update of the model that was initiated
    //! by calling beginUpdate() has finished.
    //!
    //! \see beginUpdate()
    //!
    void endUpdate ();

    //!
    //! Sets the m_selectingAll flag for the node model to prevent emitting
    //! redundant nodeSelected() signals.
    //!
    //! Should be called before selecting all nodes in the node model.
    //!
    //! \see endSelectingAll()
    //!
    void beginSelectingAll ();

    //!
    //! Resets the m_selectingAll flag for the node model and optionally emits
    //! the nodeSelected() signal for the last selected node.
    //!
    //! Should be called after selecting all nodes in the node model.
    //!
    //! \param emitSignal Flag to control whether to emit the nodeSelected() signal.
    //! \see beginSelectingAll()
    //!
    void endSelectingAll ( bool emitSignal = true);

    //!
    //! Creates a node of the given type with the given name in the node model.
    //!
    //! \param typeName The name of the type of node to add to the model.
    //! \param name The name to give to the node to add to the model.
    //! \param visible Flag that controls whether the object represented by the node should appear in views.
    //! \return The new node or 0 if the node could not be created.
    //!
    Node * createNode ( const QString &typeName, const QString &name = "", bool visible = true );

    //!
    //! Creates a connection.
    //!
    //! \param sourceParameter The source parameter of the connection.
    //! \param targetParameter The target parameter of the connection.
    //! \return The new connection or 0 if the connection could not be created.
    //!
    Connection * createConnection ( Parameter *sourceParameter, Parameter *targetParameter );


    //!
    //! Returns the node with the given name or 0 if no node with the given
    //! name can be found.
    //!
    //! \param name The name of the node to return.
    //! \return The node with the given name or 0.
    //!
    Node * getNode ( const QString &name ) const;

    //!
    //! Returns a list of nodes of the given type name contained in the node
    //! model.
    //!
    //! \param typeName The name of the type of nodes to return, or an empty string to return all nodes.
    //! \return A list of nodes of the given type name.
    //!
    QList<Node *> getNodes ( const QString &typeName = "" ) const;

    //!
    //! Returns a list of connections of the given type name contained in the node
    //! model.
    //!
    //! \return A list of connections.
    //!
    QList<Connection *> getConnections () const;

    //!
    //! Returns the list of names of nodes of the given type name contained in
    //! the node model.
    //!
    //! \param typeName The name of the type of node names to return, or an empty string to return all node names.
    //! \return A list of names of nodes of the given type name.
    //!
    QStringList getNodeNames ( const QString &typeName = "" ) const;

    //!
    //! Returns the standard item node with the given name or 0 if no item with the
    //! given name can be found.
    //!
    //! \param name The name of the standard item to return.
    //! \return The item with the given name or 0.
    //!
    QStandardItem * getStandardItemNode ( const QString &name ) const;

    //!
    //! Returns the standard item (connection) with the given name or 0 if no item with the
    //! given name can be found.
    //!
    //! \param name The name of the standard item to return.
    //! \return The item with the given name or 0.
    //!
    QStandardItem * getStandardItemConnection ( const QString &name ) const;

    //!
    //! Sets the selection state of the items in the given selection to the given
    //! value.
    //!
    //! \param selection The items to set the selection state for.
    //! \param selected The selection state to set for the items.
    //!
    void setSelected ( const QItemSelection &selection, bool selected );

    //!
    //! Deletes the node with the given name from the node model.
    //!
    //! \param name The name of the node to delete from the node model.
    //!
    void deleteNode ( const QString &name );

    //!
    //! Deletes the connection with the given name from the node model.
    //!
    //! \param name The name of the connection to delete from the node model.
    //!
    void deleteConnection ( const QString &name );

public slots:

    //!
    //! Selects or deselects the object with the given name.
    //!
    //! \param name The name of the object to select or deselect.
    //! \param selected The new selected state for the object.
    //!
    void setSelected ( const QString &name, bool selected );

    //!
    //! Gives the node with the given old name the given new name.
    //!
    //! \param oldName The old name of the scene object.
    //! \param newName The new name of the scene object.
    //!
    void renameNode ( const QString &oldName, const QString &newName );

    //!
    //! Changes the value of the parameter with the given name of the node
    //! with the given name to the given value.
    //!
    //! \param name The name of the node for which to change the parameter.
    //! \param parameterName The name of the parameter to change.
    //! \param value The new value for the parameter.
    //!
    void changeParameter ( const QString &name, const QString &parameterName, const QVariant &value );

signals: //

    //!
    //! Signal that is emitted when the internal state of the node model has
    //! changed.
    //!
    void modified ();

    //!
    //! Signal that is emitted when the selection of nodes should be cleared.
    //!
    void clearSelectionRequested ();

    //!
    //! Signal that is emitted when the given node has been selected.
    //!
    //! \param node The node that was selected.
    //!
    void nodeSelected ( Node *node );

    //!
    //! Signal that is emitted when the node with the given name has been
    //! deleted.
    //!
    //! \param name The name of the node that was deleted.
    //!
    void nodeDeleted ( const QString &name );

private: // functions

    //!
    //! Returns the category item that stands for nodes of the given node type.
    //!
    //! \param nodeCategory The category name of nodes that the category item stands for.
    //! \return The category item that stands for nodes of the given node type.
    //!
    QStandardItem * getCategoryItem ( const QString &nodeCategory );

private: // data

    //!
    //! Flag that states whether a bigger update of the node model is taking
    //! place.
    //!
    //! \see beginUpdate(), endUpdate()
    //!
    bool m_updating;

    //!
    //! Flag that states whether all nodes are currently being selected.
    //!
    //! \see beginSelectingAll(), endSelectingAll(), setSelected()
    //!
    bool m_selectingAll;

    //!
    //! The mutex to protect the node model data structure.
    //!
    QMutex m_mutex;

    //!
    //! A map for referencing nodes by name.
    //!
    QMap<QString, Node *> m_nodeMap;

    //!
    //! A map for referencing connections by name.
    //!
    QMap<QString, Connection *> m_connectionMap;

    //!
    //! A map for referencing standard items by name.
    //!
    QMap<QString, QStandardItem *> m_standardItemNodeMap;

    //! A map for referencing standard items by name.
    //!
    QMap<QString, QStandardItem *> m_standardItemConnectionMap;

    //!
    //! The list of category items that appear as children of the model's root
    //! item.
    //!
    QList<QStandardItem *> m_categoryItems;

};


#endif
