/****************************************************************************
**
** Copyright (C) L4
**
** Use of this file is limited according to the terms specified by
** L4.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "AnimationManagerComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

#include "TimelineWidget.h"

#include <QWidget>
#include <QMenu>
#include <QLabel>
#include <QScrollBar>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QMetaObject>
#include <QMap>
#include <QAction>
#include <QBoxLayout>


GCF_DEFINE_COMPONENT(AnimationManagerComponent)

struct AnimationManagerComponentData
{
	// The main panel
	QWidget* timelinePanel;
	// Layout for three children: scaleScrollBar, scrollArea, controlToolBar
	QVBoxLayout* verticalLayout;
	// scale toolbar and children
	QToolBar* scaleScrollBar;
	// Scrollbar
	QScrollBar* scaleScroller;

	// scroll area
	QScrollArea* scrollArea;
	TimelineWidget* timelineWidget;

	// Control tool bar and children
	QToolBar* controlToolBar;
	QSpinBox* startFrameSpinBox;
	QSpinBox* inFrameSpinBox;
	QSpinBox* outFrameSpinBox;
	QSpinBox* endFrameSpinBox;
	QSpinBox* frameStepSpinBox;
	QSpinBox* fpsSpinBox;

	// timeline control actions
    // scroll area actions
	QAction* autoScrollAction;
	QAction* scaleToFitAction;

	// player actions
	QAction* inFrameAction;
    QAction* previousKeyAction;
    QAction* previousFrameAction;
    QAction* playAction;
    QAction* realtimeAction;
    QAction* nextFrameAction;
    QAction* nextKeyAction;
    QAction* outFrameAction;
    QAction* separatorAction;
    QAction* playOptionsAction;
    QAction* playLoopingAction;
    QAction* playOnceAction;
    // averageCounter(0),
    // averageFps(0.0)
	QAction* keyOnAction;
	QAction* keyOffAction;
	QAction* setKeyAction;

	// Labels
	QLabel* playLabel;
	QLabel* scaleLabel;
	QLabel* startFrameLabel;
	QLabel* inFrameLabel;
	QLabel* outFrameLabel;
	QLabel* endFrameLabel;
	QLabel* fpsLabel;
	
};

AnimationManagerComponent & AnimationManagerComponent::instance()
{
    static AnimationManagerComponent* theInstance = GCF_CREATE_COMPONENT(AnimationManagerComponent);
    return *theInstance;
}

AnimationManagerComponent::AnimationManagerComponent()
{
    d = new AnimationManagerComponentData;
	
	d->timelinePanel = new QWidget;
	d->scrollArea = new QScrollArea;
	d->timelineWidget = new TimelineWidget(d->scrollArea);
	d->scrollArea->setWidget(d->timelineWidget);
		
	d->controlToolBar = new QToolBar;
	d->scaleScrollBar = new QToolBar;

	d->startFrameSpinBox = new QSpinBox;
	d->inFrameSpinBox = new QSpinBox;
	d->outFrameSpinBox = new QSpinBox;
	d->endFrameSpinBox = new QSpinBox;
	d->frameStepSpinBox = new QSpinBox;
	d->fpsSpinBox = new QSpinBox;

	d->verticalLayout = new QVBoxLayout;

	d->playLabel = new QLabel;
	d->playLabel->setText("Play every:");

	d->scaleLabel = new QLabel;
	d->scaleLabel->setText("Scale:");

	d->startFrameLabel = new QLabel;
	d->startFrameLabel->setText("Start:");

	d->inFrameLabel = new QLabel;
	d->inFrameLabel->setText("In:");

	d->outFrameLabel = new QLabel;
	d->outFrameLabel->setText("Out:");

	d->endFrameLabel = new QLabel;
	d->endFrameLabel->setText("End:");

	d->fpsLabel = new QLabel;
	d->fpsLabel->setText("FPS:");

	// create a scroll bar for the frame rectangle scale
    d->scaleScroller = new QScrollBar;
    d->scaleScroller->setOrientation(Qt::Horizontal);
    d->scaleScroller->setMinimum(3);
    d->scaleScroller->setMaximum(47);
    d->scaleScroller->setSingleStep(2);
    d->scaleScroller->setValue(TimelineWidget::DefaultFrameSize.width()+3);
    d->scaleScroller->setMinimumWidth(400);


    connect(d->scaleScroller, SIGNAL(valueChanged(int)), d->timelineWidget, SLOT(setFrameWidth(int)));
    connect(d->timelineWidget, SIGNAL(currentFrameSet(int)), SIGNAL(currentFrameSet(int)));
    connect(d->timelineWidget, SIGNAL(startFrameSet(int)), SIGNAL(startFrameSet(int)));
    connect(d->timelineWidget, SIGNAL(endFrameSet(int)), SIGNAL(endFrameSet(int)));
    connect(d->timelineWidget, SIGNAL(inFrameSet(int)), SIGNAL(inFrameSet(int)));
    connect(d->timelineWidget, SIGNAL(outFrameSet(int)), SIGNAL(outFrameSet(int)));

	// This causes a fatal crash
	// connect(d->autoScrollAction, SIGNAL(toggled(bool)), d->timelineWidget, SLOT(setAutoScroll(bool)));
		
	// set the frame range for the timeline panel
                int frameStep = 5;
                int frameRate = 24;
                int firstFrame = 0;
                int lastFrame = 100;
  
                // set the time frame indices for the timeline panel
                d->timelineWidget->setStartFrame(0);
                d->timelineWidget->setEndFrame(100);
                d->timelineWidget->setInFrame(0);
                d->timelineWidget->setOutFrame(100);
                d->timelineWidget->setCurrentFrame(22);
                d->timelineWidget->setStartFrame(0);

	// Actions
	d->autoScrollAction = new QAction(QIcon(":/AnimationManager/autoScrollIcon"), tr("Automatic Scrolling"), this);
	d->scaleToFitAction = new QAction(QIcon(":/AnimationManager/scaleToFitIcon"), tr("Scale to Fit [F]"), this);
	d->keyOnAction = new QAction(QIcon(":/AnimationManager/keyonIcon"), tr("Set Keyframe mode on"), this);
	d->keyOnAction->setCheckable(true);
	d->keyOffAction = new QAction(QIcon(":/AnimationManager/keyoffIcon"), tr("Set Keyframe mode off"), this);
	d->keyOffAction->setCheckable(true);
	d->setKeyAction = new QAction(QIcon(":/AnimationManager/setkeyIcon"), tr("Set key mark for frame"), this);

	d->inFrameAction = new QAction(QIcon(":/AnimationManager/firstFrameIcon"), tr("Jump to In Frame"), this);
    d->inFrameAction->setObjectName("inFrameAction");
    d->inFrameAction->setStatusTip(tr("Jump to In Frame [Home]"));
    d->inFrameAction->setToolTip(tr("In Frame"));
    d->inFrameAction->setShortcut(Qt::Key_Home);
    d->inFrameAction->setShortcutContext(Qt::ApplicationShortcut);
    d->inFrameAction->setAutoRepeat(false);
	
	d->previousKeyAction = new QAction(QIcon(":/AnimationManager/previousKeyIcon"), tr("Jump to Previous Keyframe"), this);
    d->previousKeyAction->setObjectName("previousKeyAction");
    d->previousKeyAction->setStatusTip(tr("Jump to Previous Keyframe [Ctrl+Left]"));
    d->previousKeyAction->setToolTip(tr("Previous Keyframe"));
    d->previousKeyAction->setShortcut(QKeySequence(tr("Ctrl+Left", "Previous Keyframe")));
    d->previousKeyAction->setShortcutContext(Qt::ApplicationShortcut);
	
	d->previousFrameAction = new QAction(QIcon(":/AnimationManager/previousFrameIcon"), tr("Go to Previous Frame"), this);
    d->previousFrameAction->setObjectName("previousFrameAction");
    d->previousFrameAction->setStatusTip(tr("Go to Previous Frame [Left]"));
    d->previousFrameAction->setToolTip(tr("Previous Frame"));
    d->previousFrameAction->setShortcut(Qt::Key_Left);
    d->previousFrameAction->setShortcutContext(Qt::ApplicationShortcut);
	
	d->playAction = new QAction(QIcon(":/AnimationManager/playIcon"), tr("Play"), this);
    d->playAction->setObjectName("playAction");
    d->playAction->setCheckable(true);
    d->playAction->setStatusTip(tr("Play back the animation [Space]"));
    d->playAction->setToolTip(tr("Play"));
    d->playAction->setShortcut(Qt::Key_Space);
    d->playAction->setShortcutContext(Qt::ApplicationShortcut);
    d->playAction->setAutoRepeat(false);


	d->realtimeAction = new QAction(QIcon(":/AnimationManager/realtimeIcon"), tr("Realtime Animation"), this);
    d->realtimeAction->setObjectName("realtimeAction");
    d->realtimeAction->setCheckable(true);
    d->realtimeAction->setStatusTip(tr("Activate the realtime mode [R]"));
    d->realtimeAction->setToolTip(tr("Realtime Animation"));
    d->realtimeAction->setText(tr("0"));
    d->realtimeAction->setShortcut(Qt::Key_R);
    d->realtimeAction->setShortcutContext(Qt::ApplicationShortcut);
    d->realtimeAction->setAutoRepeat(false);
	
	d->nextFrameAction = new QAction(QIcon(":/AnimationManager/nextFrameIcon"), tr("Go to Next Frame"), this);
    d->nextFrameAction->setObjectName("nextFrameAction");
    d->nextFrameAction->setStatusTip(tr("Go to Next Frame [Right]"));
    d->nextFrameAction->setToolTip(tr("Next Frame"));
    d->nextFrameAction->setShortcut(Qt::Key_Right);
    d->nextFrameAction->setShortcutContext(Qt::ApplicationShortcut);
	
	d->nextKeyAction = new QAction(QIcon(":/AnimationManager/nextKeyIcon"), tr("Jump to Next Keyframe"), this);
    d->nextKeyAction->setObjectName("nextKeyAction");
    d->nextKeyAction->setStatusTip(tr("Jump to Next Keyframe [Ctrl+Right]"));
    d->nextKeyAction->setToolTip(tr("Next Keyframe"));
    d->nextKeyAction->setShortcut(QKeySequence(tr("Ctrl+Right", "Next Keyframe")));
    d->nextKeyAction->setShortcutContext(Qt::ApplicationShortcut);

	d->outFrameAction = new QAction(QIcon(":/AnimationManager/lastFrameIcon"), tr("Jump to Out Frame"), this);
    d->outFrameAction->setObjectName("outFrameAction");
    d->outFrameAction->setStatusTip(tr("Jump to Out Frame [End]"));
    d->outFrameAction->setToolTip(tr("Out Frame"));
    d->outFrameAction->setShortcut(Qt::Key_End);
    d->outFrameAction->setShortcutContext(Qt::ApplicationShortcut);
    d->outFrameAction->setAutoRepeat(false);
	
	d->separatorAction = new QAction(this);
    d->separatorAction->setSeparator(true);
	
    d->playLoopingAction = new QAction(QIcon(":/AnimationManager/playLoopingIcon"), tr("Play Looping"), this);
    d->playLoopingAction->setObjectName("playLoopingAction");
    d->playLoopingAction->setCheckable(true);
    d->playLoopingAction->setChecked(true);
    d->playLoopingAction->setStatusTip(tr("Rewind the animation at the out frame"));
    d->playLoopingAction->setToolTip(tr("Play Once"));
	
	d->playOnceAction = new QAction(QIcon(":/AnimationManager/playOnceIcon"), tr("Play Once"), this);
    d->playOnceAction->setObjectName("playOnceAction");
    d->playOnceAction->setCheckable(true);
    d->playOnceAction->setStatusTip(tr("Stop playing the animation at the out frame"));
    d->playOnceAction->setToolTip(tr("Play Once"));

	// create looping mode group
    QActionGroup *loopingModeGroup = new QActionGroup(this);
    loopingModeGroup->addAction(d->playLoopingAction);
    loopingModeGroup->addAction(d->playOnceAction);
	
	d->playOptionsAction = new QAction(tr("Options"), this);
    d->playOptionsAction->setMenu(new QMenu("Play Options"));
    d->playOptionsAction->menu()->addActions(loopingModeGroup->actions());

	// averageCounter(0);
    // averageFps(0.0);
	
	// Assign actions to scrollToolBar
	d->scaleScrollBar->addAction(d->autoScrollAction);
	d->scaleScrollBar->addWidget(d->scaleLabel);
	d->scaleScrollBar->addWidget(d->scaleScroller);
	
	d->scaleScrollBar->addSeparator();

	d->scaleScrollBar->addAction(d->scaleToFitAction);
	d->scaleScrollBar->addAction(d->keyOnAction);
	d->scaleScrollBar->addAction(d->keyOffAction);
	d->scaleScrollBar->addAction(d->setKeyAction);

	// Assign actions to controlToolBar
	d->controlToolBar->addWidget(d->startFrameLabel);
	d->controlToolBar->addWidget(d->startFrameSpinBox);
	d->controlToolBar->addWidget(d->inFrameLabel);
	d->controlToolBar->addWidget(d->inFrameSpinBox);
	
	d->controlToolBar->addSeparator();

	d->controlToolBar->addAction(d->inFrameAction);
	d->controlToolBar->addAction(d->previousKeyAction);
	d->controlToolBar->addAction(d->previousFrameAction);
	d->controlToolBar->addAction(d->playAction);
	d->controlToolBar->addAction(d->nextFrameAction);
	d->controlToolBar->addAction(d->nextKeyAction);
	d->controlToolBar->addAction(d->outFrameAction);
	
	d->controlToolBar->addSeparator();
	
	d->controlToolBar->addAction(d->playOptionsAction);

	d->controlToolBar->addSeparator();

	d->controlToolBar->addAction(d->realtimeAction);
	d->controlToolBar->addWidget(d->outFrameLabel);
	d->controlToolBar->addWidget(d->outFrameSpinBox);
	d->controlToolBar->addWidget(d->endFrameLabel);
	d->controlToolBar->addWidget(d->endFrameSpinBox);
	d->controlToolBar->addWidget(d->playLabel);
	d->controlToolBar->addWidget(d->frameStepSpinBox);
	d->controlToolBar->addWidget(d->fpsLabel);
	d->controlToolBar->addWidget(d->fpsSpinBox);

	// set up signal/slot connections for the actions by their object name
    QMetaObject::connectSlotsByName(this);

	// Assemble Panel
	d->verticalLayout->addWidget(d->scrollArea);
	d->verticalLayout->insertWidget(0, d->scaleScrollBar);
	d->verticalLayout->insertWidget(2, d->controlToolBar);
	d->verticalLayout->setMargin(0);
	d->verticalLayout->setSpacing(0);


	d->timelinePanel->setLayout(d->verticalLayout);


	
}

// Events

//!
//! Sets the frame range available for the scene time to the given frame
//! indices.
//!
//! \param firstFrame The index of the first frame in the scene's time range.
//! \param lastFrame The index of the last frame in the scene's time range.
//!
void AnimationManagerComponent::setFrameRange ( int firstFrame, int lastFrame )
{
    d->startFrameSpinBox->setRange(firstFrame, lastFrame);
    d->endFrameSpinBox->setRange(firstFrame, lastFrame);
}


///
/// Public Slots
///

//!
//! Enables and disables certain widgets when realtime mode is toggled.
//!
//! \param realtime Indicates whether realtime mode is on or off.
//!
void AnimationManagerComponent::toggleRealtimeMode ( bool realtime )
{
    bool notRealtime = !realtime;
    d->startFrameSpinBox->setEnabled(notRealtime);
    d->endFrameSpinBox->setEnabled(notRealtime);
    d->inFrameSpinBox->setEnabled(notRealtime);
    d->outFrameSpinBox->setEnabled(notRealtime);
}

//!
//! Sets the frame step displayed in the frame step spin box to the given
//! value.
//!
//! \param frameStep The frame step currently used in the scene.
//!
void AnimationManagerComponent::setFrameStep ( int frameStep )
{
    d->frameStepSpinBox->setValue(frameStep);
}


//!
//! Sets the frame rate displayed in the FPS spin box to the given value.
//!
//! \param frameRate The frame rate currently used in the scene.
//!
void AnimationManagerComponent::setFrameRate ( int frameRate )
{
    d->fpsSpinBox->setValue(frameRate);
}


//!
//! Sets the index of the current frame in the scene's time.
//!
//! \param index The new index of the current frame in the scene's time.
//!
void AnimationManagerComponent::setCurrentFrame ( int index )
{
    if (d->timelineWidget)
        d->timelineWidget->setCurrentFrame(index);
}


//!
//! Sets the index of the start frame in the scene's time.
//!
//! \param index The new index of the start frame in the scene's time.
//!
void AnimationManagerComponent::setStartFrame ( int index )
{
    d->startFrameSpinBox->setValue(index);

    d->inFrameSpinBox->setMinimum(index);
    d->outFrameSpinBox->setMinimum(index);
    d->endFrameSpinBox->setMinimum(index);

    if (d->timelineWidget)
        d->timelineWidget->setStartFrame(index);
}


//!
//! Sets the index of the end frame in the scene's time.
//!
//! \param index The new index of the end frame in the scene's time.
//!
void AnimationManagerComponent::setEndFrame ( int index )
{
    d->endFrameSpinBox->setValue(index);

    d->startFrameSpinBox->setMaximum(index);
    d->inFrameSpinBox->setMaximum(index);
    d->outFrameSpinBox->setMaximum(index);

    if (d->timelineWidget)
        d->timelineWidget->setEndFrame(index);
}


//!
//! Sets the index of the in frame in the scene's time.
//!
//! \param index The new index of the in frame in the scene's time.
//!
void AnimationManagerComponent::setInFrame ( int index )
{
    d->inFrameSpinBox->setValue(index);

    d->outFrameSpinBox->setMinimum(index);

    if (d->timelineWidget)
        d->timelineWidget->setInFrame(index);
}


//!
//! Sets the index of the out frame in the scene's time.
//!
//! \param index The new index of the out frame in the scene's time.
//!
void AnimationManagerComponent::setOutFrame ( int index )
{
    d->outFrameSpinBox->setValue(index);

    d->inFrameSpinBox->setMaximum(index);

    if (d->timelineWidget)
        d->timelineWidget->setOutFrame(index);
}


///
/// Protected Events
///


//!
//! Handles key press events for the widget.
//!
//! \param event The description of the key event.
//!
void AnimationManagerComponent::keyPressEvent ( QKeyEvent *event )
{
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }

    switch (event->key()) {
        case Qt::Key_A:
            d->autoScrollAction->toggle();
            break;
        case Qt::Key_F:
            d->scaleToFitAction->trigger();
            break;
        default:
            break;
    }
}


///
/// Private Slots
///

//!
//! Sets the real-time mode on or off.
//!
//! \param realtime True -> realtime, false -> no-real-time.
//!
void AnimationManagerComponent::on_realtimeAction_triggered ( bool realtime )
{
    bool notRealtime = !realtime;
    d->startFrameSpinBox->setEnabled(notRealtime);
    d->inFrameSpinBox->setEnabled(notRealtime);
    d->outFrameSpinBox->setEnabled(notRealtime);
    d->endFrameSpinBox->setEnabled(notRealtime);
    d->controlToolBar->setEnabled(notRealtime);
}

//!
//! Event handler that is called when the Scale to Fit action has been
//! triggered.
//!
//! \param checked The state of the action.
//!
void AnimationManagerComponent::on_scaleToFitAction_triggered ( bool checked /* = false */ )
{
    int frameWidth = d->timelineWidget->calculateFrameWidth();
    d->scaleScroller->setValue(frameWidth);
}


//!
//! Event handler that is called when the value of the start frame spin
//! box has been changed.
//!
//! \param value The value of the spin box.
//!
void AnimationManagerComponent::on_startFrameSpinBox_valueChanged ( int value )
{
    emit startFrameSet(value);
}


//!
//! Event handler that is called when the value of the end frame spin
//! box has been changed.
//!
//! \param value The value of the spin box.
//!
void AnimationManagerComponent::on_endFrameSpinBox_valueChanged ( int value )
{
    emit endFrameSet(value);
}


//!
//! Event handler that is called when the value of the in frame spin
//! box has been changed.
//!
//! \param value The value of the spin box.
//!
void AnimationManagerComponent::on_inFrameSpinBox_valueChanged ( int value )
{
    emit inFrameSet(value);
}


//!
//! Event handler that is called when the value of the out frame spin
//! box has been changed.
//!
//! \param value The value of the spin box.
//!
void AnimationManagerComponent::on_outFrameSpinBox_valueChanged ( int value )
{
    emit outFrameSet(value);
}


//!
//! Event handler that is called when the value of the frame step spin
//! box has been changed.
//!
//! \param value The value of the spin box.
//!
void AnimationManagerComponent::on_frameStepSpinBox_valueChanged ( int value )
{
    if (value % 10 == 1 && value != 11)
        d->frameStepSpinBox->setSuffix(tr("st"));
    else if (value % 10 == 2 && value != 12)
        d->frameStepSpinBox->setSuffix(tr("nd"));
    else if (value % 10 == 3 && value != 13)
        d->frameStepSpinBox->setSuffix(tr("rd"));
    else
        d->frameStepSpinBox->setSuffix(tr("th"));

    emit frameStepSet(value);
}


//!
//! Event handler that is called when the value of the frame rate spin
//! box has been changed.
//!
//! \param value The value of the spin box.
//!
void AnimationManagerComponent::on_fpsSpinBox_valueChanged ( int value )
{
    emit frameRateSet(value);
}

AnimationManagerComponent::~AnimationManagerComponent()
{
    delete d;
}

QObject* AnimationManagerComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    AnimationManagerComponent* that = const_cast<AnimationManagerComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* AnimationManagerComponent::fetchWidget(const QString& completeName) const
{
	QStringList comps = completeName.split('.');
	
	if(comps.last() == "animationManager")
		
		return d->timelinePanel;
    
	return 0;

}

/*
This method is called before the GUIXML of the component is read, and 
just after the component object has been created. At this point the 
component settings has not been read/initialized. So dont use
GCF::AbstractComponent::settings() within this function
*/
void AnimationManagerComponent::initializeComponent()
{
    // Do nothing.
}

/*
This method is called just before the UI of the component is unloaded.
It is safe to use GCF::AbstractComponent::settings() here. Anything
you write into settings() in this function will be stored and loaded
back when the component class is used next
*/
void AnimationManagerComponent::finalizeComponent()
{
    // Do nothing
}

/*
This function is called after all the other components in the system 
are informed about the creation of this component. 

At this point the GUIXML of the component is read and processed. 
initializeComponent() will have already been called. aboutToActivate()
and justActivated() may not have been called.

This is the best time to hook up with objects exposed by other components.
Example:

    IPropertyEditor* propEditor = GCF::findObject<IPropertyEditor*>();
*/
void AnimationManagerComponent::creationAnnounced()
{
    // Do nothing
}

/*
This function is called when the destruction of this component has been
announced to other components in the system. This function is called
before finalizeComponent(), that means the UI of the component is 
still available. 

You can write into settings() within this function if needed.
*/
void AnimationManagerComponent::destructionAnnounced()
{
    // Do nothing
}

/*
This function is called before the component is about to be activated.
GUIXML of the component is read and processed before this function call.
*/
void AnimationManagerComponent::aboutToActivate()
{
    // Do nothing
}

/*
This function is called just after the component has been activated.
*/
void AnimationManagerComponent::justActivated()
{
    // Do nothing
}

/*
This function is called just before the component is about to get 
deactivated.
*/
void AnimationManagerComponent::aboutToDeactivate()
{
    // Do nothing
}

/*
This function is called just after the component has been deactivated.
*/
void AnimationManagerComponent::justDeactivated()
{
    // Do nothing
}

/*
This function is called when another component (pointed by comp) has
been created and initalized in the system. 

Within this function you can look for objects that implement known
interfaces within the new component.

Example:

    QList<IProjectPart*> projectParts = GCF::findObjects<IProjectPart*>(comp);
    Q_FOREACH(IProjectPart* projectPart, projectParts)
    {
        // Do something with projectPart
    }
*/
void AnimationManagerComponent::componentCreated(AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

/*
This function is called just before the component (pointed by comp)
is destroyed. When this function is called, the component and its
UI/objects are still available.

This function is good time to stop using objects referenced
within componentCreated().
*/
void AnimationManagerComponent::componentDestroyed(AbstractComponent* comp)
{
    Q_UNUSED(comp);
}

QObject* AnimationManagerComponent::containerObject()
{
    return this;
}

QString AnimationManagerComponent::productName() const
{
    return "AnimationManager";
}

QString AnimationManagerComponent::organization() const
{
    return "L4";
}

QImage AnimationManagerComponent::programLogo() const
{
    return QImage();
}

QString AnimationManagerComponent::version() const
{
    return "1.0";
}

QString AnimationManagerComponent::shortDescription() const
{
    return "Provides AnimationManager";
}

QString AnimationManagerComponent::homepage() const
{
    return "L4 website";
}

QString AnimationManagerComponent::bugAddress() const
{
    return "L4 website";
}

const QList<GCF::AboutPerson> AnimationManagerComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> AnimationManagerComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString AnimationManagerComponent::license() const
{
    return "L4 {{AppName}} License";
}

QString AnimationManagerComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(AnimationManagerComponent)

