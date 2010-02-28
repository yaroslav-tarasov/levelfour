/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVisSystemCanvasNodeItem.h"
#include "CVisSystemCanvas.h"
#include "CVisSystemCanvasConnectionItem.h"
#include "CVisSystemCanvasCmds.h"

#include "IConfigurable.h"
#include "IPropertyEditor.h"
#include "IVisSystemNode.h"
#include "IVisSystemNodeDesc.h"
#include "IVisSystemNodeConnectionPath.h"

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QPainter>
#include <QLinearGradient>
#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QStyle>
#include <QStyleOptionButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

struct CVisSystemCanvasNodeItemData
{
    CVisSystemCanvasNodeItemData() : node(0), canvas(0), configurable(0),
        updateConnectionItemsScheduled(false) { }

    IVisSystemNode* node;
    CVisSystemCanvas* canvas;
    IConfigurable* configurable;
    QMap<IVisSystemNodeConnectionPath*, QRectF> pathRectMap;
    QList<CVisSystemCanvasConnectionItem*> connectionList;
    QPointF oldPos, newPos;
    bool updateConnectionItemsScheduled;
};

CVisSystemCanvasNodeItem::CVisSystemCanvasNodeItem(IVisSystemNode* node, CVisSystemCanvas* canvas)
:QGraphicsRectItem(0, canvas->scene())
{
    d = new CVisSystemCanvasNodeItemData;
    d->node = node;
    d->canvas = canvas;
    if(d->node)
    {
        QObject* object = d->node->containerObject();
        d->configurable = qobject_cast<IConfigurable*>(object);
    }

    setZValue(1.0f);
    updateNode();
    QObject* object = d->node->containerObject();
    if(object)
        connect(object, SIGNAL(nodeNameChanged()), this, SLOT(updateNode()));

    setFlags(ItemIsSelectable|ItemIsMovable|ItemIsFocusable);

}

CVisSystemCanvasNodeItem::~CVisSystemCanvasNodeItem()
{
    delete d;
}

IVisSystemNode* CVisSystemCanvasNodeItem::visSystemNode() const
{
    return d->node;
}

void CVisSystemCanvasNodeItem::registerConnectionItem(CVisSystemCanvasConnectionItem* item)
{
    if(!item)
        return;

    if(item->senderNode() == d->node || item->receiverNode() == d->node)
        d->connectionList.append(item);
}

void CVisSystemCanvasNodeItem::unregisterConnectionItem(CVisSystemCanvasConnectionItem* item)
{
    d->connectionList.removeAll(item);
}

int CVisSystemCanvasNodeItem::connectionItemCount() const
{
    return d->connectionList.count();
}

CVisSystemCanvasConnectionItem* CVisSystemCanvasNodeItem::connectionItem(int index) const
{
    if(index < 0 || index >= d->connectionList.count())
        return 0;
    return d->connectionList[index];
}

QRectF CVisSystemCanvasNodeItem::pathRect(int pathIndex) const
{
    if(!d->node)
        return QRectF();

    IVisSystemNodeConnectionPath* path = d->node->nodeDesc()->connectionPath(pathIndex);
    if(!path)
        return QRectF();

    if(d->pathRectMap.contains(path))
        return d->pathRectMap[path];

    return d->node->pathRect(path);
}

QRectF CVisSystemCanvasNodeItem::pathRect(IVisSystemNodeConnectionPath* path) const
{
    if(!d->node)
        return QRectF();

    if(path)
        d->node->pathRect(path);

    if(d->pathRectMap.contains(path))
        return d->pathRectMap[path];

    return d->node->pathRect(path);
}

int CVisSystemCanvasNodeItem::pathIndexAt(const QPointF pos) const
{
    QList<QRectF> pathRects = d->pathRectMap.values();
    for(int i=0; i<pathRects.count(); i++)
    {
        QRectF rect = pathRects[i];
        if(rect.contains(pos))
        {
            IVisSystemNodeConnectionPath* path = d->pathRectMap.key(rect);
            return path->pathIndex();
        }
    }

    return -1;
}

IVisSystemNodeConnectionPath* CVisSystemCanvasNodeItem::pathAt(const QPointF pos) const
{
    QList<QRectF> pathRects = d->pathRectMap.values();
    for(int i=0; i<pathRects.count(); i++)
    {
        QRectF rect = pathRects[i];
        if(rect.contains(pos))
        {
            IVisSystemNodeConnectionPath* path = d->pathRectMap.key(rect);
            return path;
        }
    }

    return 0;
}

void CVisSystemCanvasNodeItem::updateNode()
{
    if(!d->node)
        return;

    QRectF r = d->node->nodeRect();

#ifndef USE_SYSTEM_STYLE
    r = r.adjusted(0, 0, 5, 5);
#endif

    setRect(r);
    d->pathRectMap.clear();
    for(int i=0; i<d->node->nodeDesc()->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = d->node->nodeDesc()->connectionPath(i);
        if(!path)
            continue;
        QRectF pathRect = d->node->pathRect(path);
        d->pathRectMap[path] = pathRect;
    }
    update();

    updateConnectionItems();
}

void CVisSystemCanvasNodeItem::updateConnectionItems()
{
    for(int i=0; i<d->connectionList.count(); i++)
        d->connectionList[i]->updateConnection();
    d->updateConnectionItemsScheduled = false;
}

#define MEAN_COLOR(c1, c2) QColor( 88,88,88 );

void CVisSystemCanvasNodeItem::paint(QPainter *p, const QStyleOptionGraphicsItem* opt, QWidget* widget)
{
    Q_UNUSED(widget);

    if(!d->node)
        return;

    QRectF r = this->rect();
    QPen pen = p->pen();

	// Isometric rotations - this is just a test. The logic should be tied to events on selection of projection toolbutton
	if(d->node->nodeDesc()->nodeClassCategory()== "Primitives")
		{
			// Right perspective
			p->shear(0,-0.5); // this works as sy = 0.5 (sine(30 deg)) for right orientation
		}
	else
		{
			// Left perspective
			p->shear(0,0.5);
		}

	// We want the nodes to be smooth and clear
	p->setRenderHint(QPainter::Antialiasing, true);
	p->setRenderHint(QPainter::TextAntialiasing, true);
	
#ifdef USE_SYSTEM_STYLE
    
    p->setPen(pen);
    p->drawText(r2, Qt::AlignCenter, d->node->nodeName());

#else
    
	// This sets the size of the visible node - x, y, w, h
	QRectF r2 = r.adjusted(0, 0, -5, -15);

    // Draw the node shadow - this should be reprojected
	if(opt->levelOfDetail >= 0.75)
    {
        QColor color1 = QColor(10,10,10);
        color1.setAlphaF(this->isSelected() ? 0.4 : 0.3);

        int shadowSize = this->isSelected() ? 5 : 3;
        QPainterPath path;
        path.addRoundedRect(r2.adjusted(shadowSize,shadowSize,shadowSize,shadowSize), 0, 0);
        p->fillPath(path, color1);

    }

    // Draw the node rectangle.
    double alpha = 0.90;
    
	if(this->isSelected())
        alpha = 1.0;

    if(this->isSelected())
	{   
		// Stroke for highlight on select
		// Actors: Green
		// Mappers: Purple
		// Filters: Yellow
		// Renderers: Blue
		// Scene: Orange
		// Effects: Red
		// Particle: Red
		// Shadow: Grey
		// Text: Grey
		// Source: Pink
		// Primitives: Silver
		// Strategies: Turquise
		// Default: White
		if(d->node->nodeDesc()->nodeClassCategory()== "Primitives")
		{
			// silver
			QColor primitivesColor = QColor(200,200,200);
			p->setPen( QPen(primitivesColor, 3) );
		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Actors")
		{
			// green
			QColor actorsColor = QColor(102,204,0);
			p->setPen( QPen(actorsColor, 3) );
		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Sources")
		{
			// pink
			QColor sourcesColor = QColor(144,68,116);
			p->setPen( QPen(sourcesColor, 3) );

		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Mappers")
		{
			// purple
			QColor mappersColor = QColor(90,85,148);
			p->setPen( QPen(mappersColor, 3) );

		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Renderers")
		{
			// blue
			QColor renderersColor = QColor(73,98,158);
			p->setPen( QPen(renderersColor, 3) );
		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Scene")
		{
			// orange
			QColor sceneColor = QColor(173,97,32);
			p->setPen( QPen(sceneColor, 3) );

		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Effects")
		{
			// red
			QColor effectsColor = QColor(125,39,90);
			p->setPen( QPen(effectsColor, 3) );

		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Particle")
		{
			// red
			QColor particleColor = QColor(152,63,80);
			p->setPen( QPen(particleColor, 3) );

		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Text")
		{
			// gray
			QColor textColor = QColor(192,177,153);
			p->setPen( QPen(textColor, 3) );

		}
		else if(d->node->nodeDesc()->nodeClassCategory()== "Shadow")
		{
			// blue
			QColor shadowColor = QColor(70,80,106);
			p->setPen( QPen(shadowColor, 3) );

		}
		else  if(d->node->nodeDesc()->nodeClassCategory()== "Filters")
		{
			// yellow	
			QColor filtersColor = QColor(242,182,0);
			p->setPen( QPen(filtersColor, 2) );
		}
		else
		{
			QColor elseColor = QColor(190,190,190);
			p->setPen( QPen(elseColor, 3) );
		}
  
		// QColor penSelectColor = QColor(0,0,0);
		// p->setPen( QPen(penSelectColor, 2) );
	}

	else
    {
		QColor penLightColor = QColor(10,10,10);
		penLightColor.setAlphaF(0.85);
        p->setPen( QPen(penLightColor, 2) );
    }
	// End stroke styling

	// Node color
	if(opt->levelOfDetail >= 0.75)
    {
		QColor topColor = QColor(154,154,154);
        QColor bottomColor = QColor(204,204,204);

        topColor.setAlphaF(alpha);
        bottomColor.setAlphaF(alpha);

        QLinearGradient grad(r2.topLeft(), r2.bottomLeft());
        grad.setColorAt(0, topColor);
        grad.setColorAt(1, bottomColor);
		
		QPainterPath path;
        path.addRoundRect(r2.adjusted(1,1,-1,-1), 0, 0);
		p->fillPath(path, grad);
		p->drawPath(path);

	}
	else
	{
		QColor topColor = QColor(164,164,164);
        QColor bottomColor = QColor(214,214,214);

        topColor.setAlphaF(alpha);
        bottomColor.setAlphaF(alpha);

        QLinearGradient grad(r2.topLeft(), r2.bottomLeft());
        grad.setColorAt(0, topColor);
        grad.setColorAt(1, bottomColor);
		
		QPainterPath path;
        path.addRoundRect(r2.adjusted(1,1,-1,-1), 0, 0);
		p->fillPath(path, grad);
		p->drawPath(path);

	}
	// End node color logic

    // Draw the node icon
	QRectF textRect;
	if(opt->levelOfDetail >= 0.75)
	{
		QPixmap nodePm = d->node->nodeDesc()->nodeIcon().pixmap(30, 30);
		QRectF iconRect( r2.left()+10, r2.top()+7, nodePm.width(), nodePm.height() );
		iconRect.moveTop( r2.center().y() - nodePm.height()/2 );
		p->drawPixmap( iconRect, nodePm, QRectF(0,0,nodePm.width(),nodePm.height()) );
	
		textRect = QRectF ( iconRect.right()+2, r2.top()+10, r2.width(), r2.height()-20 );
		textRect.setRight( r2.right() );
	}
	else
		textRect = r2;

    // Draw the node text
    p->setPen(Qt::black);
	if(opt->levelOfDetail >= 0.75)
	{
		// First draw the node name
		textRect.moveTop( r2.center().y()- textRect.height()/2 );
		textRect.moveLeft( r2.center().y()- textRect.width()/2 );
		p->drawText(textRect, Qt::AlignCenter, d->node->nodeName());
	    
		// Now draw the node class name in a smaller font
		// We don't need this because the icon is an identifier
		/*
		QFont font = p->font();
		QFont newFont = font;
		newFont.setPointSize( font.pointSize()-1 );
		p->setFont( newFont );
		textRect.moveTop( r2.center().y()+1 );
		p->drawText(textRect, Qt::AlignCenter, d->node->nodeDesc()->nodeClassName());
		p->setFont(font);
		*/
	}
	else
		p->drawText(textRect, Qt::AlignCenter, d->node->nodeDesc()->nodeClassName());

#endif

	// Draw connection boxes
	// Need to set conditions of color and add tooltip to provide hint for connections
	if(opt->levelOfDetail >= 0.75)
		d->node->paintNode(p, r, *opt);

    QBrush brush = opt->palette.mid();
    QColor color = QColor(185, 134, 32);
    color.setAlphaF(0.75f);
    brush.setColor(color);
    // color = opt->palette.shadow().color();
    color.setAlphaF(0.75f);

    QMap<IVisSystemNodeConnectionPath*, QRectF>::iterator it = d->pathRectMap.begin();
    QMap<IVisSystemNodeConnectionPath*, QRectF>::iterator end = d->pathRectMap.end();
    while(it != end)
    {
        p->setPen(Qt::black);
        p->setBrush(brush);
        // p->drawRoundRect(it.value(), 25, 25);
		p->drawEllipse(it.value());
        p->setPen(pen);

		d->node->paintConnectionPath(it.key(), p, it.value(), *opt);
        ++it;
    }
}

QVariant CVisSystemCanvasNodeItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if(change == ItemPositionChange)
    {
        if(!d->updateConnectionItemsScheduled)
        {
            d->updateConnectionItemsScheduled = true;
            QMetaObject::invokeMethod(this, "updateConnectionItems", Qt::QueuedConnection);
        }
    }
    else if(change == ItemSelectedChange)
    {
        emit itemSelected(value.toBool());
        if(value.toBool())
            setFocus();
        else
            clearFocus();

        if(value.toBool())
            emit d->canvas->nodeSelected(d->node);
        else
            emit d->canvas->nodeUnselected(d->node);
    }
    return value;
}

void CVisSystemCanvasNodeItem::mousePressEvent(QGraphicsSceneMouseEvent* me)
{
    d->oldPos = this->pos();
    QGraphicsRectItem::mousePressEvent(me);
}

void CVisSystemCanvasNodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* me)
{
    QGraphicsRectItem::mouseReleaseEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
        emit d->canvas->nodeClickedEvent(d->node, me->scenePos().toPoint(), me->button(), me->modifiers());

    d->newPos = this->pos();
    if(d->oldPos != d->newPos)
    {
        CNodeMoveCmd* cmd = new CNodeMoveCmd(this, d->oldPos, d->newPos);
        d->canvas->undoRedoStack()->push(cmd);
        d->oldPos = QPointF();
        d->newPos = QPointF();
   }
}

void CVisSystemCanvasNodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* me)
{
    QGraphicsRectItem::mouseDoubleClickEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
    {
        bool success = false;
        if(d->configurable)
            success = d->configurable->showDefaultConfigurationDialog();
        
        if(!success)
        {
            if( d->canvas->isDoubleClickSignalConnected() )
            {
                emit d->canvas->nodeDoubleClickedEvent(d->node, me->scenePos().toPoint(), me->button(), me->modifiers());
                return;
            }

            IPropertyEditor* propEditor = d->canvas->getNewPropertyEditorInstance();
            if(!propEditor)
                return;

            QDialog dialog( d->canvas );
            QWidget* propEditorWidget = (QWidget*)(propEditor->containerObject());
            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
            QVBoxLayout* layout = new QVBoxLayout(&dialog);
            
            layout->addWidget(propEditorWidget);
            layout->addWidget(buttonBox);

            propEditor->setObject(d->node->containerObject());

            dialog.setWindowTitle( QString("%1 properties").arg(d->node->nodeName()) );

            connect(buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
            dialog.resize(320, 400);
            dialog.exec();
        }
    }
}

void CVisSystemCanvasNodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* me)
{
    QGraphicsRectItem::contextMenuEvent(me);
    if(me->widget() == d->canvas || me->widget() == d->canvas->viewport())
    {
        // Show the context sensitive menu
        QMenu menu;

        // First show configurable options.
        QList<QAction*> configActions;
        if(d->configurable)
        {
            QStringList configOptions = d->configurable->configurationOptions();
            for(int i=0; i<configOptions.count(); i++)
            {
                QString option = configOptions[i];
                QAction* action = new QAction(option, &menu);
                menu.addAction(action);
                configActions.append(action);
            }
            if(configOptions.count())
                menu.addSeparator();
        }

        // Now show command actions.
        QList<QAction*> commandActions;
        QStringList commandNames = d->node->nodeDesc()->commandNames();
        for(int i=0; i<commandNames.count(); i++)
        {
            QString command = commandNames[i];
            QAction* action = new QAction(command, &menu);
            menu.addAction(action);
            commandActions.append(action);
        }
        if(commandNames.count())
            menu.addSeparator();

        // Now show default options
        QAction* disconnectNode = new QAction("Disconnect Node", &menu);
        disconnectNode->setEnabled( d->canvas->hasConnections(d->node) );
        menu.addAction(disconnectNode);
        QAction* delNode = new QAction("Remove node", &menu);
        menu.addAction(delNode);

        // Allow others to insert their items into the menu.
        emit d->canvas->nodeContextMenuEvent(d->node, &menu, me->modifiers());

        QAction* result = menu.exec(QCursor::pos());
        if(result == delNode)
        {
            IVisNetwork* visNetwork = d->canvas->visNetwork();
            if(visNetwork)
                visNetwork->removeNode(d->node);
        }
        else if(result == disconnectNode)
        {
            IVisNetwork* visNetwork = d->canvas->visNetwork();
            if(visNetwork)
                visNetwork->disconnectNode(d->node);
        }
        else if(configActions.contains(result))
        {
            QString configOption = result->text();
            d->configurable->showConfigurationDialog(configOption);
        }
        else if(commandActions.contains(result))
        {
            QString command = result->text();
            QString result;
            d->node->executeCommand(command, result);
            qDebug(qPrintable(result));
        }
        me->accept();
    }    
}

void CVisSystemCanvasNodeItem::keyPressEvent(QKeyEvent* ke)
{
    if(ke->key() == Qt::Key_F2)
    {
        QRectF nodeRect = mapToScene(boundingRect()).boundingRect();
        QRect nodeRect2 = d->canvas->mapFromScene(nodeRect).boundingRect();
        nodeRect2 = nodeRect2.adjusted(10, 10, -10, -10);
        CNodeNameLineEdit* nameLineEdit = new CNodeNameLineEdit(d->canvas->viewport(), d->node, this);
        nameLineEdit->setGeometry(nodeRect2);
        nameLineEdit->show();
        nameLineEdit->setFocus();
        ke->accept();
        return;
    }
    
    if(ke->key() == Qt::Key_Up && ke->modifiers()&Qt::AltModifier)
    {
        d->canvas->selectPrevious(this);
        return;
    }

    if(ke->key() == Qt::Key_Down && ke->modifiers()&Qt::AltModifier)
    {
        d->canvas->selectNext(this);
        return;
    }

    QGraphicsRectItem::keyPressEvent(ke);

    emit d->canvas->nodeKeyEvent(d->node, ke->key(), ke->text(), ke->modifiers());
}

QString CVisSystemCanvasNodeItem::toolTipAt(QPointF itemPos, QRectF& rect)
{
    IVisSystemNodeConnectionPath* path = pathAt(itemPos);
    if(!path)
    {
        IVisSystemNode* node = d->node;
        rect = node->pathRect(path);
        return QString("%1: %2").arg(node->nodeDesc()->nodeClassName()).arg(node->nodeDesc()->nodeClassDescription());
    }

    rect = boundingRect();
    return QString("%1 (%2)").arg(path->pathName()).arg(path->pathIndex());
}

int CVisSystemCanvasNodeItem::type() const
{
    return CVisSystemCanvasNodeItem_Type;
}

// CNodeNameLineEdit

CNodeNameLineEdit::CNodeNameLineEdit(QWidget* parent, IVisSystemNode* node, CVisSystemCanvasNodeItem* nodeItem)
:QLineEdit(parent), mNode(node), mNodeItem(nodeItem), mEscapePressed(false)
{
    connect(this, SIGNAL(editingFinished()), this, SLOT(close()));
    setText(node->nodeName());
    mOldName = node->nodeName();
    setFocus();
    grabKeyboard();
    setFrame(false);
    setAlignment(Qt::AlignCenter);
    setAutoFillBackground(false);
    selectAll();
    setMinimumHeight(sizeHint().height());
}

CNodeNameLineEdit::~CNodeNameLineEdit()
{

}

void CNodeNameLineEdit::keyPressEvent(QKeyEvent* ke)
{
    mEscapePressed = false;
    if(ke->key() == Qt::Key_Escape)
        mEscapePressed = true;
    if(mEscapePressed)
        close();
    else
        QLineEdit::keyPressEvent(ke);
}

void CNodeNameLineEdit::closeEvent(QCloseEvent* ce)
{
    releaseKeyboard();
    mNodeItem->setFocus();
    parentWidget()->setFocus();
    if(!mEscapePressed)
        mNode->setNodeName(this->text());
    else
        mNode->setNodeName(mOldName);

    CNodeNameChangeCmd* cmd = new CNodeNameChangeCmd(mNode, mOldName, this->text());
    mNodeItem->d->canvas->undoRedoStack()->push(cmd);

    deleteLater();
    Q_UNUSED(ce);
}


