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
//! \file "FaceTrackerNodePlugin.cpp"
//! \brief Header file for FaceTrackerNodePlugin class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    0.1
//! \date       22.01.2009 (last updated)
//!

#ifndef FACETRACKERNODE_H
#define FACETRACKERNODE_H

#include "Node.h"
#include "ParameterGroup.h"
#include <QtCore/QVariant>
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QTimer>

#include <vector>
#include "cv.h"         // from OpenCV
#include "cxcore.h"     // from OpenCV
#include "highgui.h"    // from OpenCV, for CvCapture, cvReleaseCapture, cvCaptureFromCAM, cvGrabFrame, cvRetrieveFrame, ...

#include "Face.h"
#include "Helper.h"

//!
//! Class in the Borealis application representing nodes that can
//! contai OGRE entities with animation.
//!
class FaceTrackerNode : public Node  //, public QThread
{

	Q_OBJECT

public: /// constructors and destrucor

    //!
    //! Constructor of the FaceTrackerNode class.
    //!
    //! \param name The name to give the new mesh node.
    //!
	FaceTrackerNode ( QString name, ParameterGroup *parameterRoot );

public: /// destructors

    //!
    //! Destructor of the FaceTrackerNode class.
    //!
    ~FaceTrackerNode ();

public: /// methods
    	
    virtual void run();

public slots:
       
    //!
    //! Starts and stops the tracker.
    //!
    void toggleRun ();

    //!
    //! Shows and hides tracking window.
    //!
    void toggleWindow ();

private slots:
       
    //!
    //! Called when timer times out.
    //!
    void onTimeout();

private: /// methods

    //!
    //! Handler for attractor list.
    //! Updates the coordinates of the focus point depending on the list of attractors.
    //!
    void processAttractors ();

    //!
    //! Animates the focus point according to the focus point target and whether or not a face is currently focused.
    //!
    void animateFocusPoint ();

    //!
    //! Captures an image from a webcam and returns it in the given image.
    //!
    //! \param image    returns the image taken by the webcam
    //! \return True, if an image was successfully captured from the webcam, otherwise False.
    //!
    bool captureImageFromWebcam(IplImage **image);

    //!
    //! Detects all faces in the given image, and creates or updates face objects for each detected face.
    //!
    //! \param image    the image to detect faces in
    //!
    void detectFaces(IplImage *image);

    //!
    //! Draws an overlay onto the image that shows the locations of detected faces in the scene.
    //!
    //! \param image    the image to show detected faces in
    //!
    void drawDetectedFaces(IplImage *image);

    //!
    //! Draws some debug information into the given image.
    //!
    //! \param image    pointer to the image to draw the debug info in
    //!
    void drawDebugInfo(IplImage *image);

    //!
    //! Decreases the probability values of the detected faces.
    //! Is called on each time step (/frame).
    //!
    void decreaseProbabilities();


private: /// attributes
    
    //!
    //! A mutex.
    //!    
    QMutex m_mutex;

    //!
    //! A Timer.
    //!    
    QTimer m_timer;

    //!
    //! The current timestamp.
    //!
    QTime m_timestamp;
    
    //!
    //! Timestamp from previous step.
    //!
    QTime m_lastTimestamp;

    //!
    //! Is termination of thread requested?
    //!
    bool m_stopRequested;
    

    //!
    //! How many objects are currently detected?
    //!
    int m_detectedObjectsCount;
    
    //!
    //! List of users which attract the agent.
    //!
    std::vector<Face *> m_faces;

    //!
    //! List of users which attract the agent.
    //!
    std::vector<Attractor> m_attractors;

    //!
    //! Index of currently atrracted user.
    //!
    int m_attractorIndex;

    //!
    //! Old index of attracted user.
    //!
    int m_attractorIndexOld;

    //!
    //! Should the attracted user be switched?
    //!
    bool m_switchUserNow;
    
    //!
    //! Has there been a user switching recently?
    //!
    int m_justSwitched;

    //!
    //! ???????
    //!
    QTime m_focusTimeLimit;

    //!
    //! ???????
    //!
    QTime m_smallFocusTimeLimit;

    //!
    //! OpenCV window handle.
    //!
    int m_cvWindowHandle;

    //!
    //! OpenCV memory storage.
    //!
    CvMemStorage *m_memStorage;

    //!
    //! OpenCV capture.
    //! 
    CvCapture *m_capture;

    //!
    //! IplImage.
    //! 
    IplImage *m_image;

    //!
    //! OpenCV haar classifier cascade.
    //! 
    CvHaarClassifierCascade *m_haarClassifierCascade;

    //!
    //! Configuration parameter group.
    //!
    ParameterGroup *m_configGroup;

    //!
    //! Output parameter group
    //!
    ParameterGroup *m_outputGroup;

    //!
    //! Focus point.
    //!
    PointNDC m_focusPoint;
    
    //!
    //! Focus point target.
    //!
    PointNDC m_focusPointTarget;
};


#endif
