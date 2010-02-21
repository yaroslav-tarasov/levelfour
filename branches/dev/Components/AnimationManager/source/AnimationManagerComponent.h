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

#ifndef ANIMATIONMANAGERCOMPONENT_H
#define ANIMATIONMANAGERCOMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

#include <QWidget>
#include <QScrollBar>
#include <QToolBar>
#include "TimelineWidget.h"

struct AnimationManagerComponentData;
class AnimationManagerComponent : public GCF::AbstractComponent, virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IAboutData)
    GCF_DECLARE_COMPONENT(AnimationManagerComponent)

public:
    static AnimationManagerComponent & instance();
    ~AnimationManagerComponent();

	void setFrameRange ( int firstFrame, int lastFrame );

protected:
    AnimationManagerComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    void destructionAnnounced();
    void aboutToActivate();
    void justActivated();
    void aboutToDeactivate();
    void justDeactivated();
    void componentCreated(AbstractComponent* comp);
    void componentDestroyed(AbstractComponent* comp);

public:
    // IContainer implementation
    QObject* containerObject();
    
    // IAboutData implementation
    QString productName() const;
    QString organization() const;
    QImage programLogo() const;
    QString version() const;
    QString shortDescription() const;
    QString homepage() const;
    QString bugAddress() const;
    const QList<GCF::AboutPerson> authors() const;
    const QList<GCF::AboutPerson> credits() const;
    QString license() const;
    QString copyrightStatement() const;

private:
    AnimationManagerComponentData* d;

public slots: //

    //!
    //! Enables and disables certain widgets when realtime mode is toggled.
    //!
    //! \param realtime Indicates whether realtime mode is on or off.
    //!
    void toggleRealtimeMode ( bool realtime );

    //!
    //! Sets the frame step displayed in the frame step spin box to the given
    //! value.
    //!
    //! \param frameStep The frame step currently used in the scene.
    //!
    void setFrameStep ( int frameStep );

    //!
    //! Sets the frame rate displayed in the FPS spin box to the given value.
    //!
    //! \param frameRate The frame rate currently used in the scene.
    //!
    void setFrameRate ( int frameRate );

    //!
    //! Sets the index of the current frame in the scene's time.
    //!
    //! \param index The new index of the current frame in the scene's time.
    //!
    void setCurrentFrame ( int index );

    //!
    //! Sets the index of the start frame in the scene's time.
    //!
    //! \param index The new index of the start frame in the scene's time.
    //!
    void setStartFrame ( int index );

    //!
    //! Sets the index of the end frame in the scene's time.
    //!
    //! \param index The new index of the end frame in the scene's time.
    //!
    void setEndFrame ( int index );

    //!
    //! Sets the index of the in frame in the scene's time.
    //!
    //! \param index The new index of the in frame in the scene's time.
    //!
    void setInFrame ( int index );

    //!
    //! Sets the index of the out frame in the scene's time.
    //!
    //! \param index The new index of the out frame in the scene's time.
    //!
    void setOutFrame ( int index );

signals: //

    //!
    //! Signal that is emitted when the index of the current frame in the
    //! scene's time has been changed.
    //!
    //! \param index The new index of the current frame in the scene's time.
    //!
    void currentFrameSet ( int index );

    //!
    //! Signal that is emitted when the index of the start frame in the scene's
    //! time has been changed.
    //!
    //! \param index The new index of the start frame in the scene's time.
    //!
    void startFrameSet ( int index );

    //!
    //! Signal that is emitted when the index of the end frame in the scene's
    //! time has been changed.
    //!
    //! \param index The new index of the end frame in the scene's time.
    //!
    void endFrameSet ( int index );

    //!
    //! Signal that is emitted when the index of the in frame in the scene's
    //! time has been changed.
    //!
    //! \param index The new index of the in frame in the scene's time.
    //!
    void inFrameSet ( int index );

    //!
    //! Signal that is emitted when the index of the out frame in the scene's
    //! time has been changed.
    //!
    //! \param index The new index of the out frame in the scene's time.
    //!
    void outFrameSet ( int index );

    //!
    //! Signal that is emitted when the frame step for the scene has been
    //! changed.
    //!
    //! \param frameStep The new frame step to use for the scene.
    //!
    void frameStepSet ( int frameStep );

    //!
    //! Signal that is emitted when the frame rate for the scene has been
    //! changed.
    //!
    //! \param frameRate The new frame rate to use for the scene.
    //!
    void frameRateSet ( int frameRate );

protected: // events

    //!
    //! Handles key press events for the widget.
    //!
    //! \param event The description of the key event.
    //!
    virtual void keyPressEvent ( QKeyEvent *event );

private slots: //

    //!
    //! Sets the real-time mode on or off.
    //!
    //! \param realtime True -> realtime, false -> no-real-time.
    //!
    void on_realtimeAction_triggered ( bool realtime );

    //!
    //! Event handler that is called when the Scale to Fit action has been
    //! triggered.
    //!
    //! \param checked The state of the action.
    //!
    void on_scaleToFitAction_triggered ( bool checked = false );

    //!
    //! Event handler that is called when the value of the start frame spin
    //! box has been changed.
    //!
    //! \param value The value of the spin box.
    //!
    void on_startFrameSpinBox_valueChanged ( int value );

    //!
    //! Event handler that is called when the value of the end frame spin
    //! box has been changed.
    //!
    //! \param value The value of the spin box.
    //!
    void on_endFrameSpinBox_valueChanged ( int value );

    //!
    //! Event handler that is called when the value of the in frame spin
    //! box has been changed.
    //!
    //! \param value The value of the spin box.
    //!
    void on_inFrameSpinBox_valueChanged ( int value );

    //!
    //! Event handler that is called when the value of the frame step spin
    //! box has been changed.
    //!
    //! \param value The value of the spin box.
    //!
    void on_frameStepSpinBox_valueChanged ( int value );

    //!
    //! Event handler that is called when the value of the out frame spin
    //! box has been changed.
    //!
    //! \param value The value of the spin box.
    //!
    void on_outFrameSpinBox_valueChanged ( int value );

    //!
    //! Event handler that is called when the value of the frame rate spin
    //! box has been changed.
    //!
    //! \param value The value of the spin box.
    //!
    void on_fpsSpinBox_valueChanged ( int value );

private: // data

    //!
    //! The scroll bar widget for adjusting the scale of the timeline widget's
    //! frames.
    //!
    QScrollBar* scaleScroller;

    //!
    //! The widget for accessing the scene model's time.
    //!
    TimelineWidget* timelineWidget;

    //!
    //! The tool bar that will receive actions for controlling the scene's
    //! time.
    //!
    //! \see getControlToolBar
    //!
    QToolBar* controlToolBar;

};

#endif

