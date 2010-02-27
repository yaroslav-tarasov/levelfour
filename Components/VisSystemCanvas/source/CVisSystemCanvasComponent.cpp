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

#include "CVisSystemCanvasComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>
#include "IPropertyEditor.h"
#include "IQObjectLoadSave.h"
#include "CVisSystemCanvas.h"
#include "CVisSystemCanvasThumbnailView.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IXmlUtils.h"
#include "IScriptEngineManager.h"
#include "IVisSystemNode.h"
#include "IScriptableObjectExplorer.h"
#include "IProjectExt.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QStackedWidget>
#include <QComboBox>


GCF_DEFINE_COMPONENT(CVisSystemCanvasComponent)

struct CVisSystemCanvasComponentData
{
    CVisSystemCanvasComponentData() : canvasThumbView(0), nodeClassRegistryComp(0),
        scriptEngineManager(0), scriptEngineComp(0), soExplorer(0),
        soExplorerComp(0) { }

    QVBoxLayout* visSystemCanvasLayout;
	QToolBar* visSystemCanvasToolBar;
	QStackedWidget* pipelineStack;

	// Controls
	QComboBox* pipelineSelection;
	QComboBox* pipelineViewports;
	
	// Actions for toolbar
	QAction* copyAction;
	QAction* cutAction;
	QAction* pasteAction;
	QAction* undoAction;
	QAction* redoAction;
	QAction* addnoteAction;
	QAction* layoutnodesAction;
	QAction* bringtocenterAction;
	QAction* zoominAction;
	QAction* zoomoutAction;
	QAction* zoomoneAction;
	QAction* zoomfitAction;
	QAction* leftProjectionAction;
	QAction* rightProjectionAction;
	
	CVisSystemCanvas* visSystemCanvas;
    CVisSystemCanvasThumbnailView* canvasThumbView;

    GCF::AbstractComponent* nodeClassRegistryComp;

    IScriptEngineManager* scriptEngineManager;
    GCF::AbstractComponent* scriptEngineComp;

    IScriptableObjectExplorer* soExplorer;
    GCF::AbstractComponent* soExplorerComp;
};

CVisSystemCanvasComponent & CVisSystemCanvasComponent::instance()
{
    static CVisSystemCanvasComponent* theInstance = GCF_CREATE_COMPONENT(CVisSystemCanvasComponent);
    return *theInstance;
}

CVisSystemCanvasComponent::CVisSystemCanvasComponent()
{
    d = new CVisSystemCanvasComponentData;
	
	d->pipelineStack = new QStackedWidget;
	d->visSystemCanvas = new CVisSystemCanvas;
	// d->pipelineStack->addWidget(d->visSystemCanvas);

	d->visSystemCanvasLayout  = new QVBoxLayout;
	d->visSystemCanvasToolBar = new QToolBar;

	d->pipelineSelection = new QComboBox;
	d->pipelineViewports = new QComboBox;
	

	// Viewport actions (these are provided by the combo box)
	d->pipelineViewports->addItem(QIcon(":/MainWindow/single.png"), tr("Single"));
	d->pipelineViewports->addItem(QIcon(":/MainWindow/overunder.png"), tr("Over Under"));
	d->pipelineViewports->addItem(QIcon(":/MainWindow/sideside.png"), tr("Side by Side"));
	d->pipelineViewports->addItem(QIcon(":/MainWindow/2u1d.png"), tr("2 Up 1 Down"));
	d->pipelineViewports->addItem(QIcon(":/MainWindow/1u2d.png"), tr("1 Up 2 Down"));
	d->pipelineViewports->addItem(QIcon(":/MainWindow/quad.png"), tr("Quad"));

	// Toolbar actions
	d->visSystemCanvasToolBar->addWidget(d->pipelineSelection);

	d->copyAction = new QAction(this);
	d->copyAction = new QAction(QIcon(":/MainWindow/copy.png"), tr("&Copy"), this);
	d->copyAction->setStatusTip(tr("Copy a node"));
	d->visSystemCanvasToolBar->addAction(d->copyAction);

	d->cutAction = new QAction(this);
	d->cutAction = new QAction(QIcon(":/MainWindow/cut.png"), tr("&Cut"), this);
	d->cutAction->setStatusTip(tr("Cut a node"));
	d->visSystemCanvasToolBar->addAction(d->cutAction);

	d->pasteAction = new QAction(this);
	d->pasteAction = new QAction(QIcon(":/MainWindow/paste.png"), tr("&Cut"), this);
	d->pasteAction->setStatusTip(tr("Paste a node"));
	d->visSystemCanvasToolBar->addAction(d->pasteAction);

	d->undoAction = new QAction(this);
	d->undoAction = new QAction(QIcon(":/MainWindow/undo.png"), tr("&Undo"), this);
	d->undoAction->setStatusTip(tr("Undo an action"));
	d->visSystemCanvasToolBar->addAction(d->undoAction);

	d->redoAction = new QAction(this);
	d->redoAction = new QAction(QIcon(":/MainWindow/redo.png"), tr("&Redo"), this);
	d->redoAction->setStatusTip(tr("Redo an action"));
	d->visSystemCanvasToolBar->addAction(d->redoAction);

	d->addnoteAction = new QAction(this);
	d->addnoteAction = new QAction(QIcon(":/VisSystemCanvas/note.png"), tr("&Add note"), this);
	d->addnoteAction->setStatusTip(tr("Add a note"));
	d->visSystemCanvasToolBar->addAction(d->addnoteAction);

	d->layoutnodesAction = new QAction(this);
	d->layoutnodesAction = new QAction(QIcon(":/VisSystemCanvas/layoutnodes.png"), tr("&Layout nodes"), this);
	d->layoutnodesAction->setStatusTip(tr("Layout nodes"));
	d->visSystemCanvasToolBar->addAction(d->layoutnodesAction);

	d->bringtocenterAction = new QAction(this);
	d->bringtocenterAction = new QAction(QIcon(":/VisSystemCanvas/bringtocenter.png"), tr("&Bring nodes to center"), this);
	d->bringtocenterAction->setStatusTip(tr("Bring nodes to center"));
	d->visSystemCanvasToolBar->addAction(d->bringtocenterAction);

	d->zoominAction = new QAction(this);
	d->zoominAction = new QAction(QIcon(":/VisSystemCanvas/zoomin.png"), tr("&ZoomIn"), this);
	d->zoominAction->setStatusTip(tr("Zoom in to canvas space"));
	d->visSystemCanvasToolBar->addAction(d->zoominAction);

	d->zoomoutAction = new QAction(this);
	d->zoomoutAction = new QAction(QIcon(":/VisSystemCanvas/zoomout.png"), tr("&Zoom out"), this);
	d->zoomoutAction->setStatusTip(tr("Zoom out"));
	d->visSystemCanvasToolBar->addAction(d->zoomoutAction);

	d->zoomoneAction = new QAction(this);
	d->zoomoneAction = new QAction(QIcon(":/VisSystemCanvas/zoomone.png"), tr("&Zoom one"), this);
	d->zoomoneAction->setStatusTip(tr("Zoom one"));
	d->visSystemCanvasToolBar->addAction(d->zoomoneAction);

	d->zoomfitAction = new QAction(this);
	d->zoomfitAction = new QAction(QIcon(":/VisSystemCanvas/zoomfit.png"), tr("&Zoom fit"), this);
	d->zoomfitAction->setStatusTip(tr("Zoom fit"));
	d->visSystemCanvasToolBar->addAction(d->zoomfitAction);

	d->leftProjectionAction = new QAction(this);
	d->leftProjectionAction = new QAction(QIcon(":/MainWindow/images/left_arrow.png"), tr("&Left Projection"), this);
	d->leftProjectionAction->setStatusTip(tr("Set left isometric projection"));
	d->visSystemCanvasToolBar->addAction(d->leftProjectionAction);

	d->rightProjectionAction = new QAction(this);
	d->rightProjectionAction = new QAction(QIcon(":/MainWindow/images/right_arrow.png"), tr("&Right Projection"), this);
	d->rightProjectionAction->setStatusTip(tr("Set right isometric projection"));
	d->visSystemCanvasToolBar->addAction(d->rightProjectionAction);

	// combo box for setting viewport
	d->visSystemCanvasToolBar->addWidget(d->pipelineViewports);

	// Toolbar Triggers
	connect(d->zoominAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(zoomIn()));
	connect(d->copyAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(copy()));
	connect(d->cutAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(cut()));
	connect(d->pasteAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(paste()));
	connect(d->undoAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(undo()));
	connect(d->redoAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(redo()));
	connect(d->addnoteAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(addNote()));
	connect(d->layoutnodesAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(addNote()));
	connect(d->bringtocenterAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(bringToCenter()));
	connect(d->zoomoutAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(zoomOut()));
	connect(d->zoomoneAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(zoomOne()));
	connect(d->zoomfitAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(zoomFit()));
	
	connect(d->leftProjectionAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(leftProjection()));
	connect(d->rightProjectionAction, SIGNAL(triggered()), d->visSystemCanvas, SLOT(rightProjection()));

	// Canvas Triggers
    connect(d->visSystemCanvas, SIGNAL(nodeAdded(IVisSystemNode*)), this, SLOT(on_nodeAdded(IVisSystemNode*)));
    connect(d->visSystemCanvas, SIGNAL(nodeRemoved(IVisSystemNode*)), this, SLOT(on_nodeRemoved(IVisSystemNode*)));
    connect(d->visSystemCanvas, SIGNAL(nodeClickedEvent(IVisSystemNode*,QPoint,Qt::MouseButton,Qt::KeyboardModifiers)),
            this, SLOT(on_nodeClickedEvent(IVisSystemNode*,QPoint,Qt::MouseButton,Qt::KeyboardModifiers)));
    connect(d->visSystemCanvas, SIGNAL(canvasClickedEvent(IVisNetworkCanvas*,QPoint,Qt::MouseButton,Qt::KeyboardModifiers)),
            this, SLOT(on_canvasClickedEvent(IVisNetworkCanvas*,QPoint,Qt::MouseButton,Qt::KeyboardModifiers)));

	// This should be the stack
	d->visSystemCanvas->setLayout(d->visSystemCanvasLayout);
	// Assemble component
	d->visSystemCanvasLayout->addWidget(d->visSystemCanvasToolBar);
	d->visSystemCanvasLayout->setContentsMargins(0,0,0,0);
	d->visSystemCanvasLayout->addStretch(1);
}

CVisSystemCanvasComponent::~CVisSystemCanvasComponent()
{
    delete d->visSystemCanvas;
    delete d;
}

CVisSystemCanvas* CVisSystemCanvasComponent::canvasObject()
{
    return d->visSystemCanvas;
}

QObject* CVisSystemCanvasComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    
    CVisSystemCanvasComponent* that = const_cast<CVisSystemCanvasComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* CVisSystemCanvasComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "visSystemCanvas")
        return d->visSystemCanvas;

    if(comps.last() == "canvasThumbView")
    {
        if(d->canvasThumbView)
            return d->canvasThumbView;

        d->canvasThumbView = new CVisSystemCanvasThumbnailView;
        d->canvasThumbView->setScene( d->visSystemCanvas->scene() );

        QString name;
        QObject* pExtObj = findObject("IProjectExt", name);
        if(pExtObj)
            connect(pExtObj, SIGNAL(projectLoadEnd()), d->canvasThumbView, SLOT(zoomFit()));

        return d->canvasThumbView;
    }

    return 0;
}

void CVisSystemCanvasComponent::addChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    if(parent != d->visSystemCanvas)
        return;

    // Lets support adding of children widgets into the graphics view
    QRectF rect;
    QPointF pos;
    QStringList hintComps = hint.split('.');
    if(hintComps.count() != 6)
    {
        rect = QRectF(-40, -40, 80, 80);
        pos = QPointF(0,0);
    }
    else
    {
        rect = QRectF(hintComps[0].toDouble(), hintComps[1].toDouble(), hintComps[2].toDouble(), hintComps[3].toDouble());
        pos = QPointF(hintComps[4].toDouble(), hintComps[5].toDouble());
    }

    d->visSystemCanvas->addWidget(child, rect, pos);
}

void CVisSystemCanvasComponent::removeChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    if(parent != d->visSystemCanvas)
        return;

    d->visSystemCanvas->removeWidget(child);
}

void CVisSystemCanvasComponent::componentCreated(GCF::AbstractComponent* comp)
{
    if( !d->scriptEngineComp || !d->scriptEngineManager )
    {
        QString ifaceName = QString("%1.IScriptEngineManager").arg(comp->componentName());
        QString completeName;
        QObject* obj = findObject(ifaceName, completeName, &(d->scriptEngineComp));
        if(obj)
            d->scriptEngineManager = qobject_cast<IScriptEngineManager*>(obj);
    }

    if( !d->soExplorer || !d->soExplorerComp )
    {
        QString ifaceName = QString("%1.IScriptableObjectExplorer").arg(comp->componentName());
        QString completeName;
        QObject* obj = findObject(ifaceName, completeName, &(d->soExplorerComp));
        if( obj )
            d->soExplorer = qobject_cast<IScriptableObjectExplorer*>(obj);
    }
}

void CVisSystemCanvasComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if( d->nodeClassRegistryComp == comp )
        d->nodeClassRegistryComp = 0;

    if( d->scriptEngineComp == comp )
    {
        d->scriptEngineComp = 0;
        d->scriptEngineManager = 0;
    }

    if( d->soExplorerComp == comp )
    {
        d->soExplorer = 0;
        d->soExplorerComp = 0;
    }
}

void CVisSystemCanvasComponent::creationAnnounced()
{
    GCF::AbstractComponent* compPtr = 0;
    QString completeName;
    QObject* nodeFacReg = findObject("IVisSystemNodeFactoryRegistry", completeName, &compPtr);
    if(nodeFacReg)
    {
        d->visSystemCanvas->setNodeFactoryRegistry(qobject_cast<IVisSystemNodeFactoryRegistry*>(nodeFacReg));
        GCF::AbstractComponent::addDependency(this, compPtr);
    }

    GCF::AbstractComponent* propEditorComp = 0;
    IPropertyEditor* propertyEditor = 0;
    QObject* propEditorObject = findObject("IPropertyEditor", completeName, &propEditorComp);
    if(propEditorObject)
        propertyEditor = qobject_cast<IPropertyEditor*>(propEditorObject);
    if(propertyEditor)
    {
        d->visSystemCanvas->setPropertyEditor(propertyEditor);
        GCF::AbstractComponent::addDependency(propEditorComp, this);
    }

    GCF::AbstractComponent* xmlUtilsComp = 0;
    QObject* xmlUtilsObj = findObject("IXmlUtils", completeName, &xmlUtilsComp);
    if(xmlUtilsObj)
    {
        IXmlUtils* xmlUtils = qobject_cast<IXmlUtils*>(xmlUtilsObj);
        d->visSystemCanvas->setXmlUtils(xmlUtils);
    }

    GCF::AbstractComponent* qlsComp = 0;
    QObject* qlsObject = findObject("IQObjectLoadSave", completeName, &qlsComp);
    if(qlsComp)
    {
        IQObjectLoadSave* qls = qobject_cast<IQObjectLoadSave*>(qlsObject);
        d->visSystemCanvas->setQObjectLoadSave(qls);
    }

    d->nodeClassRegistryComp = 0;
    findObject("IVisSystemNodeFactoryRegistryExplorer", completeName, &(d->nodeClassRegistryComp));

    {
        // Connect to main window to find out when to make this component
        // active and inactive.
        QWidget* widget = GCF::AbstractComponent::componentHostWidget();
        if(widget)
            connect(widget, SIGNAL(workspaceWidgetActivated(QWidget*)),
                    this, SLOT(on_workspaceWidgetActivated(QWidget*)));
        setActive(false);
    }

    d->scriptEngineManager = 0;
    QObject* sem = findObject("IScriptEngineManager", completeName, &(d->scriptEngineComp));
    if( sem )
        d->scriptEngineManager = qobject_cast<IScriptEngineManager*>(sem);

    d->soExplorer = 0;
    QObject* soe = findObject("IScriptableObjectExplorer", completeName, &(d->soExplorerComp));
    if( soe )
        d->soExplorer = qobject_cast<IScriptableObjectExplorer*>(soe);
}

void CVisSystemCanvasComponent::initializeComponent()
{

}

void CVisSystemCanvasComponent::finalizeComponent()
{
    d->visSystemCanvas->newCanvas();
}

IPropertyEditor* CVisSystemCanvasComponent::getNewPropertyEditorInstance() const
{
    GCF::AbstractComponent* comp = 0;
    QString peName;
    QObject* peObj = findObject("IPropertyEditor", peName, &comp);
    if(!peObj)
        return 0;
    
    QWidget* widget = comp->createWidget(peName);
    if(widget)
        return qobject_cast<IPropertyEditor*>(widget);

    return 0;
}

QObject* CVisSystemCanvasComponent::containerObject()
{
    return this;
}

QString CVisSystemCanvasComponent::productName() const
{
    return "Visualization System Canvas";
}

QString CVisSystemCanvasComponent::organization() const
{
    return "VCreate Logic (P) Ltd";
}

QImage CVisSystemCanvasComponent::programLogo() const
{
    return QImage();
}

QString CVisSystemCanvasComponent::version() const
{
    return "2.0";
}

QString CVisSystemCanvasComponent::shortDescription() const
{
    return "Canvas for creating visualization networks";
}

QString CVisSystemCanvasComponent::homepage() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner";
}

QString CVisSystemCanvasComponent::bugAddress() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner/feedback/bugs";
}

const QList<GCF::AboutPerson> CVisSystemCanvasComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
    {
        retList << GCF::AboutPerson("Prashanth N Udupa", "prashanth@vcreatelogic.com", "http://www.vcreatelogic.com", "Founder, Vision, Design, Development");
        retList << GCF::AboutPerson("Lohith D S", "NA", "http://www.vcreatelogic.com", "VTK System Development");
    }
    return retList;
}

const QList<GCF::AboutPerson> CVisSystemCanvasComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString CVisSystemCanvasComponent::license() const
{
    return "Dual License: GPL Version 2 for FOSS development, Commercial for others.";
}

QString CVisSystemCanvasComponent::copyrightStatement() const
{
    return QString();
}

void CVisSystemCanvasComponent::on_workspaceWidgetActivated(QWidget* widget)
{
    setActive((widget == d->visSystemCanvas));
    if(d->nodeClassRegistryComp && this->isActive() )
    {
        d->nodeClassRegistryComp->setActive( false );
        d->nodeClassRegistryComp->setActive( true );
    }
}

void CVisSystemCanvasComponent::on_nodeAdded(IVisSystemNode* node)
{
    if( !d->scriptEngineManager || !node )
        return;

    d->scriptEngineManager->addScriptableObject(node->containerObject(), node->nodeName());
    connect(node->containerObject(), SIGNAL(nodeNameChanged()), this, SLOT(on_nodeNameChanged()));
}

void CVisSystemCanvasComponent::on_nodeRemoved(IVisSystemNode* node)
{
    if( !d->scriptEngineManager || !node )
        return;

    d->scriptEngineManager->removeScriptableObject(node->containerObject());
}

void CVisSystemCanvasComponent::on_nodeNameChanged()
{
    if( !d->scriptEngineManager )
        return;

    IVisSystemNode* node = qobject_cast<IVisSystemNode*>(sender());
    if( !node )
        return;

    d->scriptEngineManager->changeScriptableObjectName( sender(), node->nodeName() );
}

void CVisSystemCanvasComponent::on_nodeClickedEvent(IVisSystemNode* node, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
{
    if( !d->soExplorer )
        return;

    if( !node )
        return;

    if( button != Qt::LeftButton )
        return;

    if( modifiers != Qt::NoModifier )
        return;

    d->soExplorer->highlightScriptableObject( node->containerObject() );

    Q_UNUSED(pos);
}

void CVisSystemCanvasComponent::on_canvasClickedEvent(IVisNetworkCanvas* canvas, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
{
    if( !d->soExplorer )
        return;

    d->soExplorer->highlightScriptableObject(0);

    Q_UNUSED(canvas);
    Q_UNUSED(pos);
    Q_UNUSED(button);
    Q_UNUSED(modifiers);
}

GCF_EXPORT_COMPONENT_PLUGIN(CVisSystemCanvasComponent)
