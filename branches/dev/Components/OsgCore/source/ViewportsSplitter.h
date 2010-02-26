#ifndef VIEWPORTS_SPLITTER_H
#define VIEWPORTS_SPLITTER_H

#include <QSplitter>
#include "QOSGWidget.h"
#include "ViewportPanel.h"

class ViewportPanel;

class ViewportsSplitter : public QSplitter
{
	Q_OBJECT
public:
	ViewportsSplitter(QWidget * parent = 0);
	~ViewportsSplitter();

	void setViewportsLayout(ViewportPanel * vp, int index);

	void addScene(QOSGContainer * scene, QString sceneName);
	void removeScene(QOSGContainer * scene, QString sceneName);

private:
	bool eventFilter(QObject *object, QEvent *event);
	void splitSingle(ViewportPanel * vp);
	void splitVertical(ViewportPanel * vp);
	void splitHorizontal(ViewportPanel * vp);
	void split2Up1Down(ViewportPanel * vp);
	void split1Up2Down(ViewportPanel * vp);
	void splitQuad();

	ViewportPanel first, second, third, fourth;

	QSplitter top;
	QSplitter bottom;
};

#endif