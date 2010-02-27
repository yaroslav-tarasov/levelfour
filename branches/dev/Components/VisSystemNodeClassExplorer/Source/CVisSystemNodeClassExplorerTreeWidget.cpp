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

#include "CVisSystemNodeClassExplorerTreeWidget.h"
#include "CVisSystemNodeClassExplorerTreeWidget.h"
#include <QDrag>
#include <QMimeData>
#include <QHeaderView>
#include <QMouseEvent>
#include <QApplication>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionButton>

struct CVisSystemNodeClassExplorerTreeWidgetData
{
    CVisSystemNodeClassExplorerTreeWidgetData() : itemDelegate(0) { }

    CVisSystemNodeClassExplorerTreeWidgetDelegate* itemDelegate;
    QPoint dragStartPosition;
};

CVisSystemNodeClassExplorerTreeWidget::CVisSystemNodeClassExplorerTreeWidget(QWidget* widget)
: QTreeWidget(widget)
{
    d = new CVisSystemNodeClassExplorerTreeWidgetData;
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(on_itemClicked(QTreeWidgetItem*, int)));

    d->itemDelegate = new CVisSystemNodeClassExplorerTreeWidgetDelegate(
                            itemDelegate(), // Current item delegate
                            this, this);
    setItemDelegate(d->itemDelegate);
    setIndentation(0);
    setUniformRowHeights(false);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
}


CVisSystemNodeClassExplorerTreeWidget::~CVisSystemNodeClassExplorerTreeWidget()
{
    delete d;
}

void CVisSystemNodeClassExplorerTreeWidget::mousePressEvent(QMouseEvent* me)
{
    // Let the parent class handle the mouse press event first.
    QTreeWidget::mousePressEvent(me);

    // Check if the left mouse button was pressed.
    if((me->button()&Qt::RightButton))
    {
        QTreeWidgetItem* item = itemAt(me->pos());
        if(item)
            on_itemRightClicked(item);
        return;
    }

    // Store the mouse press position as the drag start position.
    d->dragStartPosition = me->pos();

    // Preload the device pixmap so that there is less of a
    // GUI drag while actually dragging the image.
    QTreeWidgetItem* item = itemAt(d->dragStartPosition);
    if ( ! item ) return;
    // make sure that it is leaf item.
    if ( item->childCount() != 0 ) 
        return;

    //Check whether the leaf item exists or not.
    if ( item->text(1).isEmpty() ) 
        return;
}

void CVisSystemNodeClassExplorerTreeWidget::mouseMoveEvent(QMouseEvent* me)
{
    // Check if the left mouse button is being moved.
    if (!(me->buttons() & Qt::LeftButton))
        return;

    // Check if the drag distance is at least as much as required.
    if ((me->pos() - d->dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    // Fetch the item at which the mouse was originally pressed.
    QTreeWidgetItem* item = itemAt(d->dragStartPosition);
    if(!(item->flags() & Qt::ItemIsDragEnabled))
        return;

    // Find out if the item could be dragged.
    if(item && !item->text(0).isEmpty())
    {
        item->setSelected(true);

        QDrag* dragObject = new QDrag(parentWidget());
        QMimeData* mimeData = new QMimeData;
        mimeData->setText(item->text(0));
        dragObject->setMimeData(mimeData);
        dragObject->start(Qt::CopyAction);
    }
}

void CVisSystemNodeClassExplorerTreeWidget::drawBranches(QPainter* paint, const QRect & rect, const QModelIndex & index) const
{
    Q_UNUSED(paint);
    Q_UNUSED(rect);
    Q_UNUSED(index);
}

void CVisSystemNodeClassExplorerTreeWidget::on_itemClicked(QTreeWidgetItem *item, int col)
{
    if(item->parent())
        return;

    if(item->isExpanded())
        setItemExpanded(item, false);
    else
        setItemExpanded(item, true);

    Q_UNUSED(col);
}

void CVisSystemNodeClassExplorerTreeWidget::on_itemRightClicked(QTreeWidgetItem* item)
{
    Q_UNUSED(item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

CVisSystemNodeClassExplorerTreeWidgetDelegate::CVisSystemNodeClassExplorerTreeWidgetDelegate(QAbstractItemDelegate* dd, QTreeWidget* tw, QObject* parent)
:QItemDelegate(parent), defaultDelegate(dd), treeWidget(tw)
{

}

QSize CVisSystemNodeClassExplorerTreeWidgetDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    // Step 1: Fetch a pointer to the tree widget item
    // referenced by index.
    void *ip = index.internalPointer();
    if(!index.isValid() || !ip)
        return defaultDelegate->sizeHint(opt, index);
    QTreeWidgetItem* twItem = static_cast<QTreeWidgetItem*>(ip);

    // Step 2: If twItem is not a toplevel item then we dont need
    // to have customized drawing done.
    if(twItem->parent())
        return defaultDelegate->sizeHint(opt, index);

    // Step 3: Return customized size hint for the item in question.
    return QItemDelegate::sizeHint(opt, index) + QSize(4,4);
}


void CVisSystemNodeClassExplorerTreeWidgetDelegate::paint(QPainter* paint, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    // ### hardcoded in qcommonstyle.cpp
    static const int indRectConst = 9;

    // Step 1: Use the default delegates paint method if the item
    // doesnt need custom drawing.
    void *ip = index.internalPointer();
    if(!index.isValid() || !ip)
    {
        defaultDelegate->paint(paint, opt, index);
        return;
    }
    QTreeWidgetItem* twItem = static_cast<QTreeWidgetItem*>(ip);
    if(twItem->parent())
    {
        defaultDelegate->paint(paint, opt, index);
        return;
    }

	// treeWidget->setStyleSheet("border-image: url(:/MainWindow/images/branch-more.png) 0");
	
    // Step 2: We need custom drawing. So draw it here.
    // We make use of the QStyle API to ensure that the drawing
    // is done using the current style. This important since we
    // do not want our widget to appear out of place even on
    // operating systems like Linux or Mac OSX for example.
    QStyleOptionButton hopt;
    hopt.rect = opt.rect;
    QColor color = QColor(200, 200, 200);
	QColor topColor = QColor(129, 129, 129);
	QColor bottomColor = QColor(43, 43, 43);
	hopt.palette.setBrush(QPalette::Button, color);
    hopt.state = QStyle::State_Active|QStyle::State_Enabled|QStyle::State_Horizontal|QStyle::State_Enabled|QStyle::State_Raised;

    // Draw custom header.
	// hopt.icon(QIcon(":/OsgCore/Camera.png"));
	QColor darkColor = QColor(85,85,85);
	QColor lightColor = QColor(86,86,86);
	
	QLinearGradient grad(opt.rect.topLeft(), opt.rect.bottomLeft());
	grad.setColorAt(0, lightColor);
	// grad.setColorAt(0.8, midColor);
	grad.setColorAt(1, darkColor);

	paint->setBrush(lightColor);
	
	paint->setPen(Qt::NoPen);
	paint->drawRect(opt.rect);
	
	if (treeWidget->isExpanded(index))
	{
	QPixmap pixmap(":/MainWindow/images/branch-open.png");
	QRectF iconRect( hopt.rect.left()+10, hopt.rect.top()+7, pixmap.width(), pixmap.height() );
	iconRect.moveTop( hopt.rect.center().y() - pixmap.height()/2 );
	paint->drawPixmap( iconRect, pixmap, QRectF(0,0,pixmap.width(),pixmap.height()) );
	}
	else
	{
	QPixmap pixmap(":/MainWindow/images/branch-closed.png");
	QRectF iconRect( hopt.rect.left()+10, hopt.rect.top()+7, pixmap.width(), pixmap.height() );
	iconRect.moveTop( hopt.rect.center().y() - pixmap.height()/2 );
	paint->drawPixmap( iconRect, pixmap, QRectF(0,0,pixmap.width(),pixmap.height()) );
	}

	paint->setPen(topColor);
	paint->drawLine(opt.rect.topLeft(), opt.rect.topRight());
	paint->setPen(bottomColor);
	paint->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());
	
	QFont titleFont( "Tahoma" );
    titleFont.setPointSize( 8 );
	titleFont.setWeight( QFont::Normal );
	paint->setFont(titleFont);
	paint->setPen(color);
    QString text = index.model()->data(index).toString();
    QRect tempRect = opt.rect;
    tempRect.setLeft(opt.rect.left()+35);
    paint->drawText(tempRect, Qt::AlignLeft|Qt::AlignVCenter, text);

    // Draw custom branch indicators.
    QStyleOption branchOption;
    QRect r = opt.rect;
    branchOption.rect = QRect(r.left() + indRectConst/2, r.top() + (r.height() - indRectConst)/2,
        indRectConst, indRectConst);
    branchOption.palette = opt.palette;
    branchOption.state = QStyle::State_Children;
    if (treeWidget->isExpanded(index))
        branchOption.state |= QStyle::State_Open;
    treeWidget->style()->drawPrimitive(QStyle::PE_IndicatorBranch, &branchOption, paint, treeWidget);
}


