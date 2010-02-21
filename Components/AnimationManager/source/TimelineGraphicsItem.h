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
//! \brief Header file for TimelineGraphicsItem class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       03.07.2009 (last updated)
//!

#ifndef TIMELINEGRAPHICSITEM_H
#define TIMELINEGRAPHICSITEM_H

// #include "FrapperPrerequisites.h"
#include <QtGui/QGraphicsItem>


//!
//! Class for graphics items representing keys in the Curve Editor panel.
//!
class TimelineGraphicsItem : public QGraphicsItem
{

public: // constructors and destructors

    //!
    //! Constructor of the TimelineGraphicsItem class.
    //!
    //! \param curveEditorGraphicsView The graphics view in which the key graphics item is contained.
    //!
    TimelineGraphicsItem ( const int height );


public: // functions

	//!
	//! Returns a rectangle describing the bounding region of the key item.
	//!
	//! \return A rectangle describing the bounding region of the key item.
	//!
	QRectF boundingRect () const;

    //!
    //! The paint function of the node.
    //!
    virtual void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );

private: // data

	//!
	//! the height of the timeline
	//!
	int m_height;
};

#endif
