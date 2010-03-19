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
//! \file "CurveEditorGraphicsView.h"
//! \brief Header file for CurveEditorGraphicsView class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       28.07.2009 (last updated)
//!

#ifndef CURVEEDITORGRAPHICSVIEW_H
#define CURVEEDITORGRAPHICSVIEW_H

#include "FrapperPrerequisites.h"
#include "BaseGraphicsView.h"
#include "ParameterGroup.h"
#include "NumberParameter.h"
#include "TimelineGraphicsItem.h"
#include <QtGui/QGraphicsView>
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QtGui/QpainterPath>
#include <QtCore/QPointF>
#include <QtGui/QTreeWidget>


//!
//! Class representing a graphics view for displaying and editing animated
//! number parameters.
//!
//! <div align="center"><b>Inheritance Diagram</b></div>
//!
//! \dot
//!   digraph CurveEditorGraphicsView {
//!     node [fontname="FreeSans",fontsize="10",shape="box",height=0.2,width=0.4,fontname="FreeSans",color="black",style="filled",fillcolor="white"];
//!     edge [fontname="FreeSans",fontsize="10",labelfontname="FreeSans",labelfontsize="10",dir=back,style="solid",color="midnightblue"];
//!
//!     CurveEditorGraphicsView [label="CurveEditorGraphicsView",fillcolor="grey75"];
//!     QGraphicsView -> CurveEditorGraphicsView;
//!     QGraphicsView [label="QGraphicsView",fontcolor="white",fillcolor="#66b036",URL="http://doc.qtsoftware.com/4.5/qgraphicsview.html"];
//!   }
//! \enddot
//! <div><center>[<a href="graph_legend.html">legend</a>]</center></div>
//!

class KeyGraphicsItem;

class FRAPPER_GUI_EXPORT CurveEditorGraphicsView : public BaseGraphicsView
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the CurveEditorGraphicsView class.
    //!
    //! \param parent The parent widget the created instance will be a child of.
    //! \param flags Extra widget options.
    //!
    CurveEditorGraphicsView ( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

    //!
    //! Destructor of the CurveEditorGraphicsView class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~CurveEditorGraphicsView ();

public: // functions

    //!
    //! Displays the curves of the selected items in the curve editor
    //! widget.
    //!
    //! \param selectedItems The item list whose animated parameters to display in the curve editor widget.
    //!
    void showCurves ( QList<QTreeWidgetItem *> *selectedItems );

    //!
    //! Builds the scene containing the segment ande key widgets
    //!
    virtual void buildScene ();

    //!
    //! Sets the index of the current frame in the scene's time.
    //!
    //! \param index The new index of the current frame in the scene's time.
    //!
    void setCurrentFrame ( const int pos );


public slots: //

	//!
    //! Resets the network graphics view's matrix.
    //!
    void homeView ();

    //!
    //! Changes the viewing transformation so that all items are visible at maximum
    //! zoom level.
    //!
    void frameAll ();

    //!
    //! Changes the viewing transformation so that the selected items are visible
    //! at maximum zoom level.
    //!
    void frameSelected ();

	//!
    //! Toggle to show only enabled parameters
    //!
    //! \param visible Flag to control whether to show or hide enabled parameters.
    //!
	void toggleShowEnabledOnly( bool enabled );

signals:
	//!
	//! Signal that is emitted when a drag event is emited
	//!
	void drag();

protected: // event handlers

    //virtual void paintEvent (QPaintEvent *event);

    //!
    //! Displays the curves of the given parameter group in the curve editor
    //! widget.
    //!
    //! \param parameterGroup The parameter group whose animated parameters to display in the curve editor widget.
    //! \param selectedItems The selected datatree items.
    //!
    inline void drawCurves ( AbstractParameter *parameter );


	//!
    //! The overwritten the event handler for the drag move event.
    //!
    //! \param event The description of the drag move event.
    //!
	virtual void mouseMoveEvent ( QMouseEvent * event );


    //!
    //! The overwritten the event handler for resize events.
    //! Adds scene resizing and redrawing.
    //!
    //! \param event The description of the key event.
    //!
    virtual void resizeEvent ( QResizeEvent *event );

    //!
    //! The overwritten the event handler for background drawing events.
    //! Adds coordinate system to the background.
    //!
    //! \param painter The QT painter object.
    //! \param rect The drawing region painter object.
    //!
    virtual void drawBackground (QPainter *painter, const QRectF &rect);

protected: // functions
	//!
	//! Event handler for mouse wheel events.
	//!
	//! \param event The description of the mouse wheel event.
	//!
	virtual void wheelEvent ( QWheelEvent *event );

private: // functions
	//!
	//! Scales the graphics items in the scene by given value.
	//!
	//! \param value The value for scaling the scene items.
	//!
	void scaleSceneItems ( const float &value );

private: // data

    //!
    //! The pen for painting the control points on a curve.
    //!
    QPen m_pointPen;

    //!
    //! The pen for painting the path between points on a curve.
    //!
    QPen m_connectionPen;

    //!
    //! Brush to fill the control points on a curve.
    //!
    QBrush m_pointBrush;

    //!
    //! Size of the points on a curve
    //!
    QSizeF m_pointSize;

    //!
    //! A list of NumberParameters which should be drawn.
    //!
    QList<NumberParameter *> m_numberParametersToDraw;

    //!
    //! The timeleine widget.
    //!
    TimelineGraphicsItem *m_timeline;

    //!
    //! The timeleine widgets position.
    //!
    int m_timelinePos;

    //!
    //! The max number of keys in numberparameters to draw.
    //!
    int m_maxLength;

    //!
    //! The max value of keys in numberparameters to draw.
    //!
    float m_maxValue;

	//!
    //! The flag for showing only enabled parameters.
    //!
    bool m_showEnabledCurves;

	qreal m_scaleFactor;
};

#endif