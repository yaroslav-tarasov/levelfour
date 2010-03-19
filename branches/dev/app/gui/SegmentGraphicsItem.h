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
//! \brief Header file for SegmentGraphicsItem class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       24.07.2009 (last updated)
//!

#ifndef SEGMENTGRAPHICSITEM_H
#define SEGMENTGRAPHICSITEM_H

#include "FrapperPrerequisites.h"
#include <QtGui/QGraphicsItem>


//!
//! Forward declaration for the KeyGraphicsItem class.
//!
class KeyGraphicsItem;


//!
//! Class for graphics items representing curve segments in the Curve Editor
//! panel.
//!
class FRAPPER_GUI_EXPORT SegmentGraphicsItem : public QGraphicsItem
{

public: // constructors and destructors

    //!
    //! Constructor of the SegmentGraphicsItem class.
    //!
    //! \param startKeyItem The key item where the curve segment starts.
    //! \param endKeyItem The key item where the curve segment ends.
    //! \param color The color to use for the curve segment.
    //!
    SegmentGraphicsItem ( KeyGraphicsItem *startKeyItem, KeyGraphicsItem *endKeyItem, const QColor &color = QColor(Qt::darkGray) );

    //!
    //! Destructor of the SegmentGraphicsItem class.
    //!
    ~SegmentGraphicsItem();

public: // functions

    //!
    //! Returns the key item where the curve segment starts.
    //!
    //! \return The key item where the curve segment starts.
    //!
    KeyGraphicsItem * getStartKeyItem () const;

    //!
    //! Sets the key item where the curve segment should start.
    //!
    //! \param startKeyItem The key item where the curve segment should start.
    //!
    inline void setStartKeyItem ( KeyGraphicsItem *startKeyItem );

    //!
    //! Returns the key item where the curve segment ends.
    //!
    //! \return The key item where the curve segment ends.
    //!
    KeyGraphicsItem * getEndKeyItem () const;

    //!
    //! Sets the key item where the curve segment should end.
    //!
    //! \param endKeyItem The key item where the curve segment should end.
    //!
    inline void setEndKeyItem ( KeyGraphicsItem *endKeyItem );

    //!
    //! Calculates the start and end points for painting the curve segment.
    //!
    inline void adjust ();

protected: // functions

    //!
    //! Returns the bounding rectangle of the graphics item.
    //!
    //! \return The bounding rectangle of the graphics item.
    //!
    virtual QRectF boundingRect () const;

    //!
    //! Paints the graphics item into a graphics view.
    //!
    //! \param painter The object to use for painting.
    //! \param option Style options for painting the graphics item.
    //! \param widget The widget into which to paint the graphics item.
    //!
    virtual void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );

private: // data

    //!
    //! The key item where the curve segment starts.
    //!
    KeyGraphicsItem *m_startKeyItem;

    //!
    //! The key item where the curve segment ends.
    //!
    KeyGraphicsItem *m_endKeyItem;

    //!
    //! The color to use for painting the curve segment.
    //!
    QColor m_color;

    //!
    //! The start point to use for painting the curve segment.
    //!
    QPointF m_startPoint;

    //!
    //! The end point to use for painting the curve segment.
    //!
    QPointF m_endPoint;

};

#endif
