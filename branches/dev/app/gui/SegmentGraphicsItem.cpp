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
//! \file "SegmentGraphicsItem.h"
//! \brief Implementation file for SegmentGraphicsItem class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       27.07.2009 (last updated)
//!

#include "SegmentGraphicsItem.h"
#include "KeyGraphicsItem.h"
#include <QtGui/QPainter>
#include <math.h>


///
/// Constructors and Destructors
///


//!
//! Constructor of the SegmentGraphicsItem class.
//!
//! \param startKeyItem The key item where the curve segment starts.
//! \param endKeyItem The key item where the curve segment ends.
//! \param color The color to use for the curve segment.
//!
SegmentGraphicsItem::SegmentGraphicsItem ( KeyGraphicsItem *startKeyItem, KeyGraphicsItem *endKeyItem, const QColor &color /* = QColor(Qt::darkGray) */ ) :
    m_startKeyItem(startKeyItem),
    m_endKeyItem(endKeyItem),
    m_color(color),
    m_startPoint(0.0, 0.0),
    m_endPoint(0.0, 0.0)
{
	setCacheMode(DeviceCoordinateCache);
    //setCacheMode(NoCache);
    setAcceptedMouseButtons(0);
	setFlag(ItemClipsToShape);
	setHandlesChildEvents(false);

    if (m_startKeyItem)
        m_startKeyItem->addSegment(this);
    if (m_endKeyItem)
        m_endKeyItem->addSegment(this);

    adjust();
}


//!
//! Destructor of the SegmentGraphicsItem class.
//!
SegmentGraphicsItem::~SegmentGraphicsItem ()
{
}


///
/// Public Functions
///


//!
//! Returns the key item where the curve segment starts.
//!
//! \return The key item where the curve segment starts.
//!
KeyGraphicsItem * SegmentGraphicsItem::getStartKeyItem () const
{
    return m_startKeyItem;
}


//!
//! Sets the key item where the curve segment should start.
//!
//! \param startKeyItem The key item where the curve segment should start.
//!
inline void SegmentGraphicsItem::setStartKeyItem ( KeyGraphicsItem *startKeyItem )
{
    m_startKeyItem = startKeyItem;
    adjust();
}


//!
//! Sets the key item where the curve segment should end.
//!
//! \param endKeyItem The key item where the curve segment should end.
//!
inline void SegmentGraphicsItem::setEndKeyItem ( KeyGraphicsItem *endKeyItem )
{
    m_endKeyItem = endKeyItem;
    adjust();
}


//!
//! Returns the key item where the curve segment ends.
//!
//! \return The key item where the curve segment ends.
//!
KeyGraphicsItem * SegmentGraphicsItem::getEndKeyItem () const
{
    return m_endKeyItem;
}


//!
//! Calculates the start and end points for painting the curve segment.
//!
inline void SegmentGraphicsItem::adjust ()
{
    if (!m_startKeyItem || !m_endKeyItem)
        return;

    QLineF line (mapFromItem(m_startKeyItem, 0, 0), mapFromItem(m_endKeyItem, 0, 0));
    qreal length = line.length();
    QPointF edgeOffset(0.0, 0.0);
    if (!length == 0.0)
        QPointF edgeOffset ((line.dx()) / length, (line.dy()) / length);

    prepareGeometryChange();
    m_startPoint = line.p1() + edgeOffset;
    m_endPoint = line.p2() - edgeOffset;
}


///
/// Protected Functions
///


//!
//! Returns the bounding rectangle of the graphics item.
//!
//! \return The bounding rectangle of the graphics item.
//!
QRectF SegmentGraphicsItem::boundingRect () const
{
    if (!m_startKeyItem || !m_endKeyItem)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = penWidth / 2.0;

    return QRectF(m_startPoint, QSizeF(m_endPoint.x() - m_startPoint.x(), m_endPoint.y() - m_startPoint.y())).normalized().adjusted(-extra, -extra, extra, extra);
}


//!
//! Paints the graphics item into a graphics view.
//!
//! \param painter The object to use for painting.
//! \param option Style options for painting the graphics item.
//! \param widget The widget into which to paint the graphics item.
//!
void SegmentGraphicsItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    if (!m_startKeyItem || !m_endKeyItem)
        return;

    // draw the line itself
    QLineF line (m_startPoint, m_endPoint);
    painter->setPen(QPen(m_color, 0.5, Qt::SolidLine));
    painter->drawLine(line);
}
