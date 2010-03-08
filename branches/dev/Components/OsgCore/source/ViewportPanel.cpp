#include "ViewportPanel.h"
#include "QViewWidget.h"

#define SCENE 0
#define TREE 1

ViewportPanel::ViewportPanel(ViewportsSplitter * splitterContainer) 
	: QWidget(), splitter(splitterContainer)
{
	// create specific scene widgets 
	sceneWidgets = new QViewWidget(splitterContainer);

	//// init widgets common to all types

	// Type selection
	typeOfView.addItem("View");
	typeOfView.addItem("Tree");
	typeOfView.addItem("Table");

	// Viewport selection 
	sceneViewports.addItem(QIcon(":/MainWindow/single.png"), tr("Single"));
	sceneViewports.addItem(QIcon(":/MainWindow/overunder.png"), tr("Over Under"));
	sceneViewports.addItem(QIcon(":/MainWindow/sideside.png"), tr("Side by Side"));
	sceneViewports.addItem(QIcon(":/MainWindow/2u1d.png"), tr("2 Up 1 Down"));
	sceneViewports.addItem(QIcon(":/MainWindow/1u2d.png"), tr("1 Up 2 Down"));
	sceneViewports.addItem(QIcon(":/MainWindow/quad.png"), tr("Quad"));

	// init tool stack
	toolStack.addWidget(sceneWidgets->getCurrentToolBar());
	toolStack.setCurrentWidget(sceneWidgets->getCurrentToolBar());
	toolStack.setMaximumHeight(30);

	mainToolBar.addWidget(&typeOfView);
	mainToolBar.addWidget(&sceneViewports);
	mainToolBar.addWidget(&toolStack);
	mainToolBar.setStyleSheet("border:0;");

	// init central stack
	centralStack.addWidget(sceneWidgets->getCurrentWidget());
	centralStack.setCurrentWidget(sceneWidgets->getCurrentWidget());

	// Assemble the panel
	sceneLayout.addWidget(&mainToolBar);
	sceneLayout.addWidget(&centralStack);

	// set viewport layout
	setLayout(&sceneLayout);

	// toolbar staked listeners
	connect(&typeOfView, SIGNAL(activated(int)), this, SLOT(setSelectedType(int)));     
	connect(&sceneViewports, SIGNAL(activated(int)), this, SLOT(setViewport(int)));     
}

ViewportPanel::~ViewportPanel()
{
	delete sceneWidgets;
}

void ViewportPanel::setSelectedType(int index)
{
	if (index == toolStack.currentIndex())
		return;
	switch(index)
	{
	case SCENE:
		toolStack.setCurrentWidget(sceneWidgets->getCurrentToolBar());
		centralStack.setCurrentWidget(sceneWidgets->getCurrentWidget());
		break;
	case TREE:
		break;
	}
}


void ViewportPanel::updateIndexLayout(const int& newIndex)
{
	sceneViewports.setCurrentIndex(newIndex);
}

void ViewportPanel::setViewport(int index)
{
	if (splitter)
		splitter->setViewportsLayout(this, index);
}

void ViewportPanel::setSplitter(ViewportsSplitter * splitterContainer)
{
	this->splitter = splitterContainer;
}

void ViewportPanel::addScene(osg::Group * sceneRoot, QString name)
{
	sceneWidgets->addScene(sceneRoot, name);
}

void ViewportPanel::removeScene(QString name, bool last)
{
	sceneWidgets->removeScene(name);
}

