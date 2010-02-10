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

#include "MainWindow.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QWorkspace>
#include <QDockWidget>
#include <QTabWidget>
#include <QStatusBar>
#include <QApplication>
#include <QDialog>
#include <QContextMenuEvent>
#include <QLabel>
#include <QMenuBar>
#include <QStyle>
#include <QIcon>
#include <QSplitter>
#include <QTabBar>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include "MainWindowComponent.h"
#include "AboutBox.h"
#include "IWorkspaceTabBarPaintHelper.h"

#include <GCF/ConfigurationDialogComponent.h>

struct GCF::Components::MainWindowData
{
    MainWindowData() : centralWidget(0), workspace(0), shutdownFlag(false) { }

    QSplitter* centralWidget;
    QTabWidget* workspace;
    QMap<Qt::DockWidgetArea, QDockWidget*> dockWidgetMap;
    bool shutdownFlag;
};

/**
\ingroup mainwindowcomponent
\class GCF::Components::MainWindow GCF::Components::MainWindow.h
\brief This class provides functions to create Main window for your application.

Class provides functions to create main window, and the contents of the main window like central widget,
dock widgets, adding/removing the widgets to the central widgets etc.
*/

/**
Returns the pointer to only instance of GCF::Components::MainWindow.
If there is no instance, one is created and returned.
*/
GCF::Components::MainWindow & GCF::Components::MainWindow::instance()
{
    return *(GCF::Components::MainWindowComponent::instance().mainWindow());
}

GCF::Components::MainWindow::MainWindow()
:QMainWindow(0)
{
    d = new GCF::Components::MainWindowData;
    setAnimated(false);
    setMinimumSize(800, 600);

    d->centralWidget = new QSplitter(Qt::Vertical, this);
    setCentralWidget(d->centralWidget);
	setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);


    d->workspace = new QTabWidget(d->centralWidget);
    d->workspace->setTabPosition(QTabWidget::North);
    d->workspace->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    d->centralWidget->addWidget(d->workspace);
    connect(d->workspace, SIGNAL(currentChanged(int)), this, SLOT(on_workspace_currentChanged(int)));

    // new TabWidgetStyle(d->workspace);
}

/**
Destructor
*/
GCF::Components::MainWindow::~MainWindow()
{
    delete d;
}

/**
Returns pointer to the central widget
*/
QSplitter* GCF::Components::MainWindow::centralWidget() const
{
    return d->centralWidget;
}

/**
Returns pointer to the workspace.
*/
QTabWidget* GCF::Components::MainWindow::workspace()
{
    return d->workspace;
}


/**
Returns the pointer to the dock widget for the given dock area.

If dockwidget corresponding to the dock area is not found, new dockwidget for that
dock area is created.

\param area DockWidgetArea for which corresponding dockwidget is returned.

*/
QDockWidget* GCF::Components::MainWindow::dockWidget(Qt::DockWidgetArea area)
{
    QDockWidget* ret = 0;

    if(d->dockWidgetMap.contains(area))
        ret = d->dockWidgetMap[area];

    if(!ret)
    {
        // create the dock widget
        ret = new QDockWidget(this);
        ret->setAllowedAreas(area);
		ret->setFeatures(QDockWidget::NoDockWidgetFeatures);
		
		// This is to remove the title bar
		QWidget* titleWidget = new QWidget(this);
		ret->setTitleBarWidget( titleWidget );

        // place a tab widget on the dock widget
        QTabWidget* tabWidget = new QTabWidget(ret);
        ret->setWidget(tabWidget);

        // position the tabs in the right place.
        switch(area)
        {
        case Qt::LeftDockWidgetArea:
            tabWidget->setTabPosition(QTabWidget::North);
            break;
        case Qt::RightDockWidgetArea:
            tabWidget->setTabPosition(QTabWidget::North);
            break;
        case Qt::TopDockWidgetArea:
            tabWidget->setTabPosition(QTabWidget::North);
            // tabWidget->setFixedHeight(150);
            // ret->setFixedHeight(150);
            break;
        case Qt::BottomDockWidgetArea:
            tabWidget->setTabPosition(QTabWidget::North);
            // tabWidget->setFixedHeight(150);
            // ret->setFixedHeight(150);
            break;
        default:
            break;
        }
    }
	addDockWidget(area, ret);
    d->dockWidgetMap[area] = ret;
    ret->hide();
	

    return ret;
}

void GCF::Components::MainWindow::addChildWidget(QWidget* parent, QWidget* child)
{
    if(!parent || !child)
        return;

    if(parent == d->workspace)
    {
        d->workspace->addTab(child, child->windowTitle());
        child->installEventFilter(this);
        child->setAutoFillBackground(true);
        emit workspaceWidgetActivated(d->workspace->currentWidget());
    }
    else
    {
        QList<QDockWidget*> dockWidgets = d->dockWidgetMap.values();
        QDockWidget* dw = qobject_cast<QDockWidget*>(parent);
        if(dw && dockWidgets.contains(dw))
        {
            QTabWidget* tw = qobject_cast<QTabWidget*>(dw->widget());
            tw->addTab(child, child->windowTitle());
            child->setAutoFillBackground(true);
            if(tw->count())
                dw->show();
            else
                dw->hide();
        }
    }
}

void GCF::Components::MainWindow::removeChildWidget(QWidget* parent, QWidget* child)
{
    if(!parent || !child)
        return;

    if(parent == d->workspace)
    {
        d->workspace->removeTab(d->workspace->indexOf(child));
        child->removeEventFilter(this);
    }
    else
    {
        QList<QDockWidget*> dockWidgets = d->dockWidgetMap.values();
        QDockWidget* dw = qobject_cast<QDockWidget*>(parent);
        if(dw && dockWidgets.contains(dw))
        {
            QTabWidget* tw = qobject_cast<QTabWidget*>(dw->widget());
            tw->removeTab(tw->indexOf(child));
            if(tw->count())
                dw->show();
            else
                dw->hide();
        }
    }
}

void GCF::Components::MainWindow::showChildWidget(QWidget* parent, QWidget* child)
{
    if(!parent || !child)
        return;

    if(parent == d->workspace)
    {
        d->workspace->setCurrentWidget(child);
    }
    else
    {
        QList<QDockWidget*> dockWidgets = d->dockWidgetMap.values();
        QDockWidget* dw = qobject_cast<QDockWidget*>(parent);
        if(dw && dockWidgets.contains(dw))
        {
            QTabWidget* tw = qobject_cast<QTabWidget*>(dw->widget());
            int index = tw->indexOf(child);
            if(index < 0)
                child->show();
            else
            {
                child->setEnabled(true);
                tw->setCurrentWidget(child);
            }
        }
    }
}

void GCF::Components::MainWindow::hideChildWidget(QWidget* parent, QWidget* child)
{
    if(!parent || !child)
        return;

    if(parent == d->workspace)
    {
        // widgets on the workspace are not hidden or shown
    }
    else
    {
        QList<QDockWidget*> dockWidgets = d->dockWidgetMap.values();
        QDockWidget* dw = qobject_cast<QDockWidget*>(parent);
        if(dw && dockWidgets.contains(dw))
        {
            QTabWidget* tw = qobject_cast<QTabWidget*>(dw->widget());
            int index = tw->indexOf(child);
            if(index < 0)
                child->hide();
            else
                child->setEnabled(false);
        }
    }
}

void GCF::Components::MainWindow::showConfigurationDialog()
{
    GCF::ConfigurationDialogComponent* comp = new GCF::ConfigurationDialogComponent;
    comp->initialize();

    // The dialog box will execute in the meantime.
    // It will also load pages from objects that implement the
    // IConfigurationDialogPageProvider interface

    comp->finalize();
}

void GCF::Components::MainWindow::on_workspace_currentChanged(int index)
{
    QWidget* widget = d->workspace->widget(index);
    emit workspaceWidgetActivated(widget);
}

void GCF::Components::MainWindow::about()
{
    GCF::Components::AboutBox aboutDialog(this);
    aboutDialog.exec();
}

void GCF::Components::MainWindow::closeEvent(QCloseEvent* ce)
{
    if(d->shutdownFlag)
    {
        QMainWindow::closeEvent(ce);
        return;
    }

    QMessageBox::StandardButton ret =
    QMessageBox::information(this, "Quit Confirmation", "Are you sure you want to quit?",
        QMessageBox::Yes|QMessageBox::No, QMessageBox::No);

    if(ret == QMessageBox::No)
        ce->setAccepted(false);
    else
    {
        ce->setAccepted(true);
        d->shutdownFlag = true;
        GCF::Components::MainWindowComponent::instance().shutdown();
    }
}

bool GCF::Components::MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if(event->type() == QEvent::WindowTitleChange)
    {
        QWidget* w = qobject_cast<QWidget*>(obj);
        int index = d->workspace->indexOf(w);
        if(index < 0)
            return false;

        QString title = w->windowTitle();

        d->workspace->setTabText(index, title);
    }

    return false;
}

