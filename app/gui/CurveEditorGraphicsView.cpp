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
//! \file "CurveEditorGraphicsView.cpp"
//! \brief Implementation file for CurveEditorGraphicsView class.
//!
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       28.07.2009 (last updated)
//!

#include "CurveEditorGraphicsView.h"
#include "SegmentGraphicsItem.h"
#include "KeyGraphicsItem.h"
#include "NumberParameter.h"
#include <QtGui/QPainter>
#include <QtGui/QScrollBar>
#include <QtGui/QGraphicsScene>
#include <QtGui/QWheelEvent>
#include <QPointF>

#include <math.h>
#define scenescale 5 

Q_DECLARE_METATYPE(AbstractParameter *)
Q_DECLARE_METATYPE(Key *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the CurveEditorGraphicsView class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
CurveEditorGraphicsView::CurveEditorGraphicsView ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    BaseGraphicsView(parent),
    m_timeline(new TimelineGraphicsItem(height()*scenescale)),
    m_maxLength(1),
	m_scaleFactor(1.0),
    m_maxValue(-HUGE),
    m_timelinePos(0),
	m_showEnabledCurves(false)
{
    QGraphicsScene *graphicsScene = new QGraphicsScene(this);
    graphicsScene->setSceneRect(0, 0, width()*scenescale, height()*scenescale);

    setScene(graphicsScene);

	centerOn(0,scene()->sceneRect().height()/2);

    //setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorViewCenter);
    setResizeAnchor(AnchorViewCenter);
}


//!
//! Destructor of the CurveEditorGraphicsView class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CurveEditorGraphicsView::~CurveEditorGraphicsView ()
{
}


///
/// Public Functions
///


//!
//! Displays the curves of the given parameter group in the curve editor
//! widget.
//!
//! \param parameterGroup The parameter group whose animated parameters to display in the curve editor widget.
//!
void CurveEditorGraphicsView::showCurves ( QList<QTreeWidgetItem *> *selectedItems )
{
    AbstractParameter *currentParameter;
    m_numberParametersToDraw.clear();
    m_maxValue = -HUGE;
    m_maxLength = 1;

    foreach (QTreeWidgetItem *item, *selectedItems) {
        currentParameter = item->data(1,0).value<AbstractParameter *>();
        drawCurves(currentParameter);
    }
}

//!
//! Displays the curves of the given parameter group in the curve editor
//! widget.
//!
//! \param parameterGroup The parameter group whose animated parameters to display in the curve editor widget.
//!
inline void CurveEditorGraphicsView::drawCurves ( AbstractParameter *parameter )
{
    // iterate over the list of parameters contained in the given parameter group
    NumberParameter *numberParameter;
    float length;

    if (parameter->isGroup()) {
        const AbstractParameter::List *parameters = static_cast<ParameterGroup *>(parameter)->getParameterList();
		foreach (AbstractParameter *parameter, *parameters) {
            // recursively parse nested parameter groups and parameters
			if (m_showEnabledCurves && !parameter->isEnabled())
				continue;
            drawCurves(parameter);
		}
	}
    else {
		if (m_showEnabledCurves && !parameter->isEnabled())
			return;
        numberParameter = dynamic_cast<NumberParameter *>(parameter);
        if (numberParameter && numberParameter->isAnimated()) {
            length = numberParameter->getKeysSize();
            if (length > m_maxLength) 
				m_maxLength = length;
			m_numberParametersToDraw.append(numberParameter);
        }
    }
}


//!
//! The overwritten the event handler for the drag move event.
//!
//! \param event The description of the drag move event.
//!
void CurveEditorGraphicsView::mouseMoveEvent ( QMouseEvent * event )
{
	Qt::MouseButton debug = event->button();
	if (event->buttons() & Qt::LeftButton)
		emit drag();
	BaseGraphicsView::mouseMoveEvent(event);
}


//!
//! The overwritten the event handler for resize events.
//! Adds scene resizing and redrawing.
//!
//! \param event The description of the key event.
//!
void CurveEditorGraphicsView::resizeEvent ( QResizeEvent *event )
{
	//const QPointF oldPos = this->sceneRect().center();
	scene()->setSceneRect(0, 0, width()*scenescale, height()*scenescale);
    buildScene();
	centerOn(0,scene()->sceneRect().height()/2);
    QGraphicsView::resizeEvent(event);
}

//!
//! The overwritten the event handler for background drawing events.
//! Adds coordinate system to the background.
//!
//! \param painter The QT painter object.
//! \param rect The drawing region painter object.
//!
void CurveEditorGraphicsView::drawBackground (QPainter *painter, const QRectF &rect)
{
    static const unsigned int collumns = 40;
    unsigned int yPos;
    QVector<QLineF> lines;
	painter->setBrush(Qt::gray);
	painter->drawRect(sceneRect());

	painter->setPen(QPen(Qt::lightGray, 1.0, Qt::SolidLine));

    for (unsigned int i = 0; i < collumns; ++i) {
		yPos = i * sceneRect().height() / collumns;
        lines.push_back(QLineF( QPointF(0, yPos), QPointF(sceneRect().width(), yPos) ));
    }

    painter->drawLines(lines);
    lines.clear();

    lines.push_back(QLineF( QPointF(0, sceneRect().height() / 2.0), QPointF(sceneRect().width(), sceneRect().height() / 2.0)));
    lines.push_back(QLineF( QPointF(0, 0), QPointF(0, sceneRect().height())));
    painter->setPen(QPen(Qt::black, 1.0, Qt::SolidLine));
    painter->drawLines(lines);
}


///
/// Public Slots
///


//!
//! Resets the network graphics view's matrix.
//!
void CurveEditorGraphicsView::homeView ()
{
    setScale(1);
	QRectF boundingRect = scene()->itemsBoundingRect();
    centerOn(boundingRect.left() + boundingRect.width() / 2, boundingRect.top() + boundingRect.height() / 2);

	scaleSceneItems(1.0/matrix().m11());
}


//!
//! Changes the viewing transformation so that all items are visible at maximum
//! zoom level.
//!
void CurveEditorGraphicsView::frameAll ()
{
    frame(scene()->itemsBoundingRect());
	KeyGraphicsItem *keyItem;
	
	scaleSceneItems(1.0/matrix().m11());
}


//!
//! Changes the viewing transformation so that the selected items are visible
//! at maximum zoom level.
//!
void CurveEditorGraphicsView::frameSelected ()
{
    // make sure there is a selection
    QList<QGraphicsItem *> selectedItems = scene()->selectedItems();
    if (selectedItems.size() == 0)
        return;

    // obtain the bounding rectangle that encompasses all selected items
    QRectF boundingRect;
    foreach (QGraphicsItem *item, selectedItems)
        boundingRect = boundingRect.united(item->sceneBoundingRect());

    frame(boundingRect);

	scaleSceneItems(1.0/matrix().m11());
}


//!
//! Toggle to show only enabled parameters
//!
//! \param visible Flag to control whether to show or hide enabled parameters.
//!
void CurveEditorGraphicsView::toggleShowEnabledOnly( bool enabled )
{
	m_showEnabledCurves = enabled;
}


//!
//! Builds the scene containing the segment ande key widgets
//!
void CurveEditorGraphicsView::buildScene ()
{
    // local variable definitions
    float keyValue, xScale;
    unsigned int index, i;
    QColor curveColor;
    KeyGraphicsItem *node;
    const QList<Key *> *keys;
    KeyGraphicsItem *prenode;
    QVariant variant0;

    // local variable initialization
    i = index = 0;
    prenode = NULL;
    const float yOffset = scene()->height() / 2.0f;
	const float yScale = 1.0f / height() * 2.0f;
    // remove all items from scene
    scene()->clear();

    // iterate through axes
    foreach (NumberParameter *numberParameter, m_numberParametersToDraw) {

        switch (index) {
            case 0:
                curveColor = QColor(Qt::red);
                break;
            case 1:
                curveColor = QColor(Qt::green);
                break;
            case 2:
                curveColor = QColor(Qt::blue);
                break;
            default:
                curveColor = QColor(Qt::darkGray);
        }

        keys    = numberParameter->getKeys();
        i       = 0;
        prenode = NULL;
        xScale  = ((float) width()) / keys->size();

        // iterate through keys
        foreach (Key *key, *keys) {
            keyValue = key->getValue().toDouble(); 
            node     = new KeyGraphicsItem(this, yScale, i * xScale);

            scene()->addItem(node);
            node->setPos( i * xScale, (-keyValue / yScale) + yOffset);
			node->setCurveScale(yScale);
			node->setScale(1.0/matrix().m11());

            variant0.setValue<Key *>(key);
            
			node->setData(0, variant0);

            if (!prenode) {
                prenode = new KeyGraphicsItem(this, yScale);
                prenode->setPos(0.0f, (-keyValue / yScale) + yOffset);
				prenode->setScale(1.0/matrix().m11());
			}

            scene()->addItem(new SegmentGraphicsItem(prenode, node, curveColor));

            prenode = node;
            ++i;
        }
        ++index;
    }

    // adding the timeline widget
    m_timeline = new TimelineGraphicsItem(height()*scenescale);
    m_timeline->setPos(m_timelinePos/m_maxLength, 0);
    scene()->addItem(m_timeline);
}

//!
//! Sets the index of the current frame in the scene's time.
//!
//! \param index The new index of the current frame in the scene's time.
//!
void CurveEditorGraphicsView::setCurrentFrame ( const int pos )
{
    m_timelinePos = pos * width();
    m_timeline->setPos(m_timelinePos/m_maxLength, 0);
    update();
}

//!
//! Event handler for mouse wheel events.
//!
//! \param event The description of the mouse wheel event.
//!
void CurveEditorGraphicsView::wheelEvent ( QWheelEvent *event )
{
	BaseGraphicsView::wheelEvent(event);
	if (event->delta() != 0 && !scene()->items().empty())
		scaleSceneItems(1.0/matrix().m11());
}

//private functions

//!
//! Scales the graphics items in the scene by given value.
//!
//! \param value The value for scaling the scene items.
//!
void CurveEditorGraphicsView::scaleSceneItems ( const float &value )
{
	KeyGraphicsItem *keyItem;
	const QList<QGraphicsItem *> &itemList = scene()->items();
	foreach(QGraphicsItem *item, itemList) {
		if (item->isVisible() && dynamic_cast<KeyGraphicsItem *>(item))
				item->setScale(value);
	}
}
