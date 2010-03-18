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
//! \file "TimelineGraphicsItem.h"
//! \brief Implementation file for TimelineGraphicsItem class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       21.07.2009 (last updated)
//!

#include "TimelineGraphicsItem.h"
#include <QtGui/QPainter>



///
/// Constructors and Destructors
///


//!
//! Constructor of the TimelineGraphicsItem class.
//!
//! \param curveEditorGraphicsView The graphics view in which the key graphics item is contained.
//!
TimelineGraphicsItem::TimelineGraphicsItem ( const int height ) :
	m_height(height)
{
    setCacheMode(QGraphicsItem::NoCache);
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
QRectF TimelineGraphicsItem::boundingRect () const
{
	return QRectF(-1.0, 0.0, 2.0, m_height);
}

//!
//! The paint function of the node.
//!
void TimelineGraphicsItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
    QLineF line(QPointF(0.0, 0.0), QPointF(0.0, m_height));
	painter->setPen(QPen(Qt::green, 1.0, Qt::SolidLine));
    painter->drawLine(line);
}

