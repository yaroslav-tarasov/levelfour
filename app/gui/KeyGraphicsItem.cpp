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
//! \brief Implementation file for KeyGraphicsItem class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.3
//! \date       16.03.2010 (last updated)
//!

#include "KeyGraphicsItem.h"
#include "SegmentGraphicsItem.h"
#include "CurveEditorGraphicsView.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>

Q_DECLARE_METATYPE(Key *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the KeyGraphicsItem class.
//!
//! \param curveEditorGraphicsView The graphics view in which the key graphics item is contained.
//!
KeyGraphicsItem::KeyGraphicsItem ( CurveEditorGraphicsView *curveEditorGraphicsView, const float scale /*= 1.0f*/, const float posX /*= 0.0f*/) :
	m_posX(posX),
	m_sceneScale(scale),
	m_size(1.5)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
	setFlag(ItemClipsToShape);
	setHandlesChildEvents(false);
#if QT_VERSION >= 0x040600
    // Since Qt 4.6 geometry changes are disabled by default. Introduction of new flag.
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
#endif
    setCacheMode(DeviceCoordinateCache);
    //setCacheMode(NoCache);
    setZValue(1);
}


///
/// Public Functions
///


//!
//! Returns a rectangle describing the bounding region of the key item.
//!
//! \return A rectangle describing the bounding region of the key item.
//!
QRectF KeyGraphicsItem::boundingRect () const
{
    return QRectF(-m_size, -m_size, m_size*2, m_size*2);
}

//!
//! The paint function of the node.
//!
void KeyGraphicsItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	painter->setPen(Qt::NoPen);
	if (isSelected())
		painter->setBrush(Qt::yellow);
	else 
		painter->setBrush(Qt::black);
	painter->drawRect(-m_size, -m_size, m_size*2, m_size*2);
}

//!
//! Overwrite the default mouseMoveEvent handler to lock x axis
//!
void KeyGraphicsItem::mouseMoveEvent ( QGraphicsSceneMouseEvent *event )
{
	event->setScenePos(QPointF(m_posX, event->scenePos().y()));

	Key *key;
	KeyGraphicsItem *keyItem;
	QList<QGraphicsItem *> &selectedItems = scene()->selectedItems();
	foreach (QGraphicsItem *item, selectedItems) {
		key = item->data(0).value<Key *>();
		keyItem = static_cast<KeyGraphicsItem *>(item);
		const float newPosY = scene()->height() / 2.0f - keyItem->pos().y();
		key->setValue( (float) (newPosY * m_sceneScale) );
	}

	QGraphicsItem::mouseMoveEvent(event);
}

//!
//! Saves the scene/curve to a member variable.
//!
inline void KeyGraphicsItem::setCurveScale ( const float scale )
{
	m_sceneScale = scale;
}


//!
//! Adds the given segment item to the list of segment items for the key
//! item.
//!
//! \param segmentItem The segment item to add to the list of segment items.
//!
void KeyGraphicsItem::addSegment ( SegmentGraphicsItem *segmentItem )
{
    m_segmentItems << segmentItem;
    segmentItem->adjust();
}


////!
////! Returns the list of segment items connected to the key item.
////!
////! \return The list of segment items connected to the key item.
////!
//inline QList<SegmentGraphicsItem *> KeyGraphicsItem::getSegmentItems () const
//{
//    return m_segmentItems;
//}


///
/// Protected Functions
///


//!
//! The item change handler for updating the node.
//!
QVariant KeyGraphicsItem::itemChange ( GraphicsItemChange change, const QVariant &value )
{
    if (change == ItemPositionHasChanged)
        foreach (SegmentGraphicsItem *segmentItem, m_segmentItems)
            segmentItem->adjust();
	return QGraphicsItem::itemChange(change, value);
}
