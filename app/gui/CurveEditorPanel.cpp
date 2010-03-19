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
//! \file "CurveEditorPanel.cpp"
//! \brief Implementation file for CurveEditorPanel class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.0
//! \date       27.07.2009 (last updated)
//!

#include "CurveEditorPanel.h"
#include "KeyGraphicsItem.h"
#include "CurveEditorDataNode.h"
#include <QtGui/QLabel.h>


Q_DECLARE_METATYPE(AbstractParameter *)
Q_DECLARE_METATYPE(Key *)

///
/// Constructors and Destructors
///


//!
//! Constructor of the CurveEditorPanel class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
CurveEditorPanel::CurveEditorPanel ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    ViewPanel(ViewPanel::T_CurveEditor, parent, flags),
	m_curveEditorGraphicsView(new CurveEditorGraphicsView(this)),
	m_valueSpinBox(new QDoubleSpinBox(this)),
	m_scaleSpinBox(new QDoubleSpinBox(this)),
	m_curveCheckBox(new QCheckBox(this))
{
    setupUi(this);
   
    m_hboxLayout = new QHBoxLayout();
    m_hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

    m_muteIcon.addFile(QString::fromUtf8(":/muteAllIcon"), QSize(), QIcon::Normal, QIcon::Off);

    m_dataTree = new QTreeWidget(this);
    m_dataTree->setObjectName(QString::fromUtf8("dataTreeWidget"));
	m_dataTree->setIconSize(QSize(13,13));
	m_dataTree->setColumnCount(2);
	m_dataTree->setColumnWidth(0, 168);
	m_dataTree->setColumnWidth(1, 25);
	QStringList columnNames;
	columnNames << "Data Tree" << " ";
	m_dataTree->setHeaderLabels(columnNames);
    m_dataTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_dataTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_dataTree->setMaximumWidth(230);

	m_curveCheckBox->setObjectName("curveCheckBox");

    m_hboxLayout->addWidget(m_dataTree);
    m_hboxLayout->addWidget(m_curveEditorGraphicsView);

	ui_vboxLayout->addLayout(m_hboxLayout);

	m_valueSpinBox->setMinimum(-1.5);
	m_valueSpinBox->setMaximum( 1.5);
	m_valueSpinBox->setDecimals(2);
	m_valueSpinBox->setSingleStep(0.01);

	m_scaleSpinBox->setValue( 1.0);
	m_scaleSpinBox->setMinimum( 0.0);
	m_scaleSpinBox->setMaximum(10.0);
	m_scaleSpinBox->setDecimals(2);
	m_scaleSpinBox->setSingleStep(0.01);

    QObject::connect(m_dataTree, SIGNAL(itemSelectionChanged()), this, SLOT(showCurves()));
	QObject::connect(m_curveEditorGraphicsView, SIGNAL(drag()), this, SIGNAL(drag()));

	connect(ui_muteAction, SIGNAL(triggered()), SLOT(muteSelectedParameterGroups()));
	connect(ui_muteAllAction, SIGNAL(triggered()), SLOT(muteAllParameterGroups()));
	connect(ui_unmuteAllAction, SIGNAL(triggered()), SLOT(unmuteAllParameterGroups()));

	connect(ui_homeAction, SIGNAL(triggered()), m_curveEditorGraphicsView, SLOT(homeView()));
    connect(ui_frameAllAction, SIGNAL(triggered()), m_curveEditorGraphicsView, SLOT(frameAll()));
    connect(ui_frameSelectedAction, SIGNAL(triggered()), m_curveEditorGraphicsView, SLOT(frameSelected()));
	connect(ui_scrollbarsAction, SIGNAL(toggled(bool)), m_curveEditorGraphicsView, SLOT(toggleScrollbars(bool)));

	connect(m_valueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeKeyValues(double)));
	connect(m_scaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(scaleKeyValues(double)));
	connect(m_curveCheckBox, SIGNAL(toggled(bool)), m_curveEditorGraphicsView, SLOT(toggleShowEnabledOnly(bool)));
}


//!
//! Destructor of the CurveEditorPanel class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CurveEditorPanel::~CurveEditorPanel ()
{
}


///
/// Public Functions
///


//!
//! Fills the given tool bars with actions for the curve editor panel.
//!
//! \param mainToolBar The main tool bar to fill with actions.
//! \param panelToolBar The panel tool bar to fill with actions.
//!
void CurveEditorPanel::fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar )
{
	QLabel *onlyUnmutedLabel = new QLabel(tr("Show unmuted only "), this);
	QLabel *setselectedLabel = new QLabel(tr("Set selected Keys "), this);
	QLabel *scaleselectedLabel = new QLabel(tr("Scale selected Keys "), this);

	mainToolBar->addAction(ui_muteAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(ui_muteAllAction);
	mainToolBar->addAction(ui_unmuteAllAction);
	mainToolBar->addSeparator();
	mainToolBar->addWidget(setselectedLabel);
	mainToolBar->addWidget(m_valueSpinBox);
	mainToolBar->addSeparator();
	mainToolBar->addWidget(scaleselectedLabel);
	mainToolBar->addWidget(m_scaleSpinBox);
	
	panelToolBar->addWidget(onlyUnmutedLabel);
	panelToolBar->addWidget(m_curveCheckBox);
	panelToolBar->addSeparator();
	panelToolBar->addAction(ui_homeAction);
    panelToolBar->addAction(ui_frameSelectedAction);
    panelToolBar->addAction(ui_frameAllAction);
	panelToolBar->addSeparator();
	panelToolBar->addAction(ui_scrollbarsAction);
	panelToolBar->addSeparator();
}


///
/// Public Slots
///

//!
//! Slot that is called when the selection in the scene model has changed.
//!
//! \param objectsSelected Flag that states whether objects in the scene are selected.
//!
void CurveEditorPanel::updateTree ( bool objectsSelected )
{
    if (objectsSelected)
        return;

    // clear and reset the parameter panel
    buildTree(0);
}

//!
//! Initializes the Selection Tree
//!
//! \param node The node whose animated parameters to display in the curve editor.
//!
void CurveEditorPanel::buildTree ( Node *node )
{
	m_dataTree->clear();

	CurveEditorDataNode *curveEditorDataNode = dynamic_cast<CurveEditorDataNode *>(node);
	
	if (!curveEditorDataNode)
        return;

	ParameterGroup* database = curveEditorDataNode->getCurveEditorGoup();
	if (database)
		fillTree(database, m_dataTree->invisibleRootItem());
}


//!
//! Displays the curves of the given node in the curve editor.
//!
//!
void CurveEditorPanel::showCurves ()
{
    m_curveEditorGraphicsView->showCurves(&m_dataTree->selectedItems());
    m_curveEditorGraphicsView->buildScene();
}


//!
//! Sets the index of the current frame in the scene's time.
//!
//! \param index The new index of the current frame in the scene's time.
//!
void CurveEditorPanel::setCurrentFrame ( const int index ) const
{
    m_curveEditorGraphicsView->setCurrentFrame(index);
}


//!
//! Sets the value of the selected Keys.
//!
//! \param value The new value for the selected keys.
//!
void CurveEditorPanel::changeKeyValues (double value)
{
	Key *key;
	KeyGraphicsItem *keyItem;
	QList<QGraphicsItem *> selectedItems = m_curveEditorGraphicsView->scene()->selectedItems();
	
	if (selectedItems.empty())
		return;
	
	const float sceneScale = 1.0f / m_curveEditorGraphicsView->height() * 2.0f;

	foreach (QGraphicsItem *item, selectedItems) {
		key = item->data(0).value<Key *>();
		key->setValue( (float) (value) );
		keyItem = static_cast<KeyGraphicsItem *>(item);
		const float newPosY = m_curveEditorGraphicsView->scene()->height() / 2.0f - value / sceneScale;
		keyItem->setPos(keyItem->x(),newPosY);
	}
	emit drag();
}


//!
//! Scales the value of the selected Keys.
//!
//! \param scale The scale value for the selected keys.
//!
void CurveEditorPanel::scaleKeyValues (double scale)
{
	Key *key;
	KeyGraphicsItem *keyItem;
	QList<QGraphicsItem *> selectedItems = m_curveEditorGraphicsView->scene()->selectedItems();
	
	if (selectedItems.empty())
		return;
	
	const float sceneScale = 1.0f / m_curveEditorGraphicsView->height() * 2.0f;

	foreach (QGraphicsItem *item, selectedItems) {
		key = item->data(0).value<Key *>();
		// TODO: NILZ: Maybe switch to .toFloat() globally. Available since 4.6
        //const float keyValue = key->getValue().toFloat();
		const float keyValue = (float) key->getValue().toDouble();
        key->setValue( (float) (keyValue * scale) );
		keyItem = static_cast<KeyGraphicsItem *>(item);
		const float newPosY = m_curveEditorGraphicsView->scene()->height() / 2.0f - (keyValue * scale) / sceneScale;
		keyItem->setPos(keyItem->x(),newPosY);
	}
	emit drag();
}


//!
//! Mutes/unmutes the in treeView selected Parameters
//!
void CurveEditorPanel::muteSelectedParameterGroups ()
{
	AbstractParameter *currentParameter;
	const QList<QTreeWidgetItem *> &itemList = m_dataTree->selectedItems();

	foreach (QTreeWidgetItem *item, itemList) {
		currentParameter = item->data(1,0).value<AbstractParameter *>();
		if (currentParameter->isEnabled()) {
			item->setIcon(1, m_muteIcon);
			currentParameter->setEnabled(false);
			item->setTextColor(0, QColor::fromRgb(200, 0, 0));
		}
		else {
			item->setIcon(1, QIcon());
			currentParameter->setEnabled(true);
			item->setTextColor(0, QColor::fromRgb(0 , 0, 0));
		}
	}
	emit drag();
	m_dataTree->repaint();
}


//!
//! Mutes all treeView Parameters
//!
void CurveEditorPanel::muteAllParameterGroups ()
{
	AbstractParameter *currentParameter;
	QTreeWidgetItem  *item, *parent;
	const QList<QTreeWidgetItem *> &itemList = m_dataTree->selectedItems();
	
	if (!itemList.isEmpty() && (parent = itemList.at(0)->parent())) {
		for (unsigned int i=0; i<parent->childCount(); ++i) {
			item = parent->child(i);
			currentParameter = item->data(1, 0).value<AbstractParameter *>();
			if (currentParameter->isEnabled()) {
				currentParameter->setEnabled(false);
				item->setIcon(1, m_muteIcon);
				item->setTextColor(0, QColor::fromRgb(200, 0, 0));
			}
			else
				continue;
		}
	}
	else {
		for (unsigned int i=0; i<m_dataTree->topLevelItemCount(); ++i) {
			item = m_dataTree->topLevelItem(i);
			currentParameter = item->data(1, 0).value<AbstractParameter *>();
			if (currentParameter->isEnabled()) {
				currentParameter->setEnabled(false);
				item->setIcon(1, m_muteIcon);
				item->setTextColor(0, QColor::fromRgb(200, 0, 0));
			}
			else
				continue;
		}
	}
	emit drag();
	m_dataTree->repaint();
}


//!
//! Unmutes all treeView Parameters
//!
void CurveEditorPanel::unmuteAllParameterGroups ()
{
	AbstractParameter *currentParameter;
	QTreeWidgetItem  *item, *parent;
	const QList<QTreeWidgetItem *> &itemList = m_dataTree->selectedItems();
	
	if (!itemList.isEmpty() && (parent = itemList.at(0)->parent())) {
		for (unsigned int i=0; i<parent->childCount(); ++i) {
			item = parent->child(i);
			currentParameter = item->data(1, 0).value<AbstractParameter *>();
			if (!currentParameter->isEnabled()) {
				currentParameter->setEnabled(true);
				item->setIcon(1, QIcon());
				item->setTextColor(0, QColor::fromRgb(0, 0, 0));
			}
			else
				continue;
		}
	}
	else {
		for (unsigned int i=0; i<m_dataTree->topLevelItemCount(); ++i) {
			item = m_dataTree->topLevelItem(i);
			currentParameter = item->data(1, 0).value<AbstractParameter *>();
			if (!currentParameter->isEnabled()) {
				currentParameter->setEnabled(true);
				item->setIcon(1, QIcon());
				item->setTextColor(0, QColor::fromRgb(0, 0, 0));
			}
			else
				continue;
		}
	}
	emit drag();
	m_dataTree->repaint();
}



///
/// Privata Functions
///


//!
//! Fills the tree view with node data groups
//!
//! \param rootData the root parameter group containing the data
//! \param rootItem the QTreeWidgetItem to add the data
//!
void CurveEditorPanel::fillTree( ParameterGroup *rootData, QTreeWidgetItem *rootItem )
{
    const AbstractParameter::List *data = rootData->getParameterList();
	NumberParameter *numparameter;
	ParameterGroup *parameterGroup;
    QVariant variant;
    QTreeWidgetItem *newItem;

    foreach (AbstractParameter *parameter, *data) {
		numparameter = dynamic_cast<NumberParameter *>(parameter);
		parameterGroup = dynamic_cast<ParameterGroup *>(parameter);
		if ( (numparameter && numparameter->isAnimated()) || 
			 (parameterGroup && ( parameterGroup->contains(Parameter::Type::T_Float) || parameterGroup->contains(Parameter::Type::T_Group) )) ) {
			newItem = new QTreeWidgetItem(rootItem, QStringList(parameter->getName()));
			if (!parameter->isEnabled()) {
				newItem->setIcon(1, m_muteIcon);
				newItem->setTextColor(0, QColor::fromRgb(200, 0, 0));
			}
			variant.setValue<AbstractParameter *>(parameter);
			newItem->setData(1, 0, variant);
			rootItem->addChild(newItem);
			if (parameterGroup)
				fillTree(parameterGroup, newItem);
		}
    }
}


///
/// Protected Events
///


//!
//! Handles key press events for the widget.
//!
//! \param event The description of the key event.
//!
void CurveEditorPanel::keyPressEvent ( QKeyEvent *event )
{
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }

    switch (event->key()) {
        case Qt::Key_A:
            ui_frameAllAction->trigger();
            break;
        case Qt::Key_F:
            ui_frameSelectedAction->trigger();
            break;
        case Qt::Key_H:
        case Qt::Key_Home:
            ui_homeAction->trigger();
            break;
        case Qt::Key_T:
            ui_scrollbarsAction->toggle();
            break;
        default:
            ViewPanel::keyPressEvent(event);
    }
}


