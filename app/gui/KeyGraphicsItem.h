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
//! \file "KeyGraphicsItem.h"
//! \brief Header file for KeyGraphicsItem class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.3
//! \date       16.03.2010 (last updated)
//!

#ifndef KEYGRAPHICSITEM_H
#define KEYGRAPHICSITEM_H

#include "FrapperPrerequisites.h"
#include <QtGui/QGraphicsItem>
#include <QtCore/QList>


//!
//! Forward declaration for the edge class.
//!
class SegmentGraphicsItem;

//!
//! Forward declaration for the curve editor class
//!
class CurveEditorGraphicsView;

//!
//! Forward declaration for the mouse event handler.
//!
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


//!
//! Class for graphics items representing keys in the Curve Editor panel.
//!
class FRAPPER_GUI_EXPORT KeyGraphicsItem : public QGraphicsItem
{

public: // constructors and destructors

    //!
    //! Constructor of the KeyGraphicsItem class.
    //!
    //! \param curveEditorGraphicsView The graphics view in which the key graphics item is contained.
    //!
    KeyGraphicsItem ( CurveEditorGraphicsView *curveEditorGraphicsView, const float scale = 1.0f, const float posX = 0.0f );

public: // functions

	//!
    //! Saves the scene/curve scale to a member variable.
    //!
	inline void setCurveScale ( const float scale );

    //!
    //! Adds the given segment item to the list of segment items for the key
    //! item.
    //!
    //! \param segmentItem The segment item to add to the list of segment items.
    //!
    void addSegment ( SegmentGraphicsItem *segmentItem );

    ////!
    ////! Returns the list of segment items connected to the key item.
    ////!
    ////! \return The list of segment items connected to the key item.
    ////!
    //inline QList<SegmentGraphicsItem *> getSegmentItems () const;

protected: // functions

    //!
    //! The item change handler for updating the node.
    //!
    QVariant itemChange ( GraphicsItemChange change, const QVariant &value );

	//!
    //! Returns a rectangle describing the bounding region of the key item.
    //!
    //! \return A rectangle describing the bounding region of the key item.
    //!
    virtual QRectF boundingRect () const;

    //!
    //! The paint function of the node.
    //!
    virtual void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );

	//!
	//! Overwrite the default mouseMoveEvent handler to lock x axis
	//!
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent *event );


private: // data

    //!
    //! The list containing all curve segment items connected to the key item.
    //!
    QList<SegmentGraphicsItem *> m_segmentItems;

	//!
    //! The old x position of the key
    //!
	qreal m_posX;

	//!
    //! The curve/scene scale
    //!
	qreal m_sceneScale;

	//!
    //! The size of the item
    //!
	qreal m_size;
};

#endif
