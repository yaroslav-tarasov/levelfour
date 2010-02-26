#include "ViewportsSplitter.h"

ViewportsSplitter::ViewportsSplitter(QWidget * parent) 
	: QSplitter(Qt::Vertical, parent)	
{
	first.setSplitter(this);
	second.setSplitter(this);
	third.setSplitter(this);
	fourth.setSplitter(this);
	splitQuad();
    this->installEventFilter(this);
}

ViewportsSplitter::~ViewportsSplitter() 
{
}

bool ViewportsSplitter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Show)
    {
		if (object == this)
        {
			setViewportsLayout(0,0);
		    this->removeEventFilter(this);
        }
    }
    return false;
}

void ViewportsSplitter::addScene(QOSGContainer * scene, QString sceneName)
{
	if (!scene || sceneName.size() <= 0)
		return;
	
	first.addSceneItem(sceneName);
	first.addSceneStack(scene);
}

void ViewportsSplitter::removeScene(QOSGContainer * scene, QString sceneName)
{
	// TODO iterate the list and look for the object scene that contains
	// the corresponding scene to remove it form the list
	// update the scene stack accordingly
	first.removeSceneItem(sceneName);
	first.removeSceneStack(scene);
}

void ViewportsSplitter::setViewportsLayout(ViewportPanel * vp, int index)
{
	switch (index)
	{
	case 0:
		splitSingle(vp);
		break;
	case 1:
		splitVertical(vp);
		break;
	case 2: 
		splitHorizontal(vp);
		break;
	case 3:
		split2Up1Down(vp);
		break;
	case 4:
		split1Up2Down(vp);
		break;
	case 5:
		splitQuad();
		break;
	}
}

void ViewportsSplitter::splitSingle(ViewportPanel * vp)
{
	first.setParent(this);
	second.setParent(0);
	third.setParent(0);
	fourth.setParent(0);

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::splitVertical(ViewportPanel * vp)
{
	setOrientation(Qt::Vertical);
	first.setParent(this);
	second.setParent(0);
	third.setParent(this);
	fourth.setParent(0);

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::splitHorizontal(ViewportPanel * vp)
{
	setOrientation(Qt::Horizontal);
	first.setParent(this);
	second.setParent(0);
	third.setParent(this);
	fourth.setParent(0);

	top.setParent(0);
	bottom.setParent(0);
}

void ViewportsSplitter::split2Up1Down(ViewportPanel * vp) 
{
	setOrientation(Qt::Vertical);

	top.setParent(this);
	first.setParent(&top);
	second.setParent(&top);

	bottom.setParent(this);
	third.setParent(&bottom);
	fourth.setParent(0);
}

void ViewportsSplitter::split1Up2Down(ViewportPanel * vp)
{
	setOrientation(Qt::Vertical);

	top.setParent(this);
	first.setParent(&top);
	second.setParent(0);

	bottom.setParent(this);
	third.setParent(&bottom);
	fourth.setParent(&bottom);
}

void ViewportsSplitter::splitQuad()
{
	setOrientation(Qt::Vertical);

	top.setParent(this);
	first.setParent(&top);
	second.setParent(&top);

	bottom.setParent(this);
	third.setParent(&bottom);
	fourth.setParent(&bottom);
}