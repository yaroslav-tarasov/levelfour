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
//! \file "FaceTrackerNode.cpp"
//! \brief Implementation file for FaceTrackerNode class.
//!
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    1.0
//! \date       28.04.2009 (last updated)
//!

#include "FaceTrackerNode.h"
#include <QtCore/QFile>
#include <QtCore/QDir>

const float PROBABILITY_INCREASE_PER_DETECTION = 0.2f;
const float PROBABILITY_DECREASE_PER_TIME_STEP = 0.1f;
const float THRESHOLD = 10.0f;
///
///   MISC FUNCTIONS
///


//!
//! Returns the normalized coordinates of the given CvPoint with respect to the dimensions of the given image.
//!
//! \param image    pointer to an image, whose dimensions should be used in calculating the normalized coordinates
//! \param p        the point to be normalized using the image's coordinates
//! \return The given coordinates positioned in the space of the given image.
//!
PointNDC toPointNDC (IplImage *image, CvPoint p)
{
    assert(image);
    PointNDC result;
    result.x = (float) p.x / image->width;
    result.y = (float) p.y / image->height;
    return result;
}


//!
//! Returns the int coordinates of the given NDC with respect to the dimensions of the given image.
//!
//! \param image    pointer to an image, whose dimensions should be used in calculating the int coordinates
//! \param x        the normalized x coordinate to convert
//! \param y        the normalized y coordinate to convert
//! \return The given coordinates positioned in the space of the given image.
//!
CvPoint toCvPoint (IplImage *image, float x, float y)
{
    assert(image);

    return cvPoint((int) (x * image->width), (int) (y * image->height));
}


//!
//! Returns the int coordinates of the given point's NDC with respect to the dimensions of the given image.
//!
//! \param image    pointer to an image, whose dimensions should be used in calculating the int coordinates
//! \param p        the normalized device coordinates of the point to convert
//! \return The coordinates of the given point in the space of the given image.
//!
CvPoint toCvPoint(IplImage *image, PointNDC p)
{
    return toCvPoint(image, p.x, p.y);
}


//!
//! Returns the int coordinates of the top left corner's NDC of the given rectangle with respect to the dimensions of the given image.
//!
//! \param image    pointer to an image, whose dimensions should be used in calculating the int coordinates
//! \param rect     the rectangle whose top left corner to convert
//! \return The coordinates of the given point in the space of the given image.
//!
CvPoint toCvPoint(IplImage *image, RectNDC rect)
{
    return toCvPoint(image, rect.x, rect.y);
}

///
///   DEFINES
///


//!
//! The name of the Haar classifier cascade file to use for detecting faces.
//!
#define HAAR_FILENAME "plugins/haarcascade_frontalface_alt.cascade"


///
/// Constructors and Destructors
///


//!
//! Constructor of the FaceTrackerNode class.
//!
//! \param name The name to give the new mesh node.
//!
FaceTrackerNode::FaceTrackerNode ( QString name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
    m_timestamp = QTime::currentTime();
    m_lastTimestamp = m_timestamp;
    m_focusTimeLimit = m_timestamp;
    m_switchUserNow = false;
    m_justSwitched = 0;

    m_focusPointTarget.x = 0.5;
    m_focusPointTarget.y = 0.5;
    
    m_focusPoint.x = 0.5;
    m_focusPoint.y = 0.5;

    m_stopRequested = false;
    m_cvWindowHandle = 0;
    
    // initialize data
    m_capture                  = NULL;
    m_memStorage               = NULL;
    m_haarClassifierCascade    = NULL;
    m_image                    = NULL;
	std::string cascadeFileName = HAAR_FILENAME;

    // check if the cascade file exists
    FILE *cascadeFile;
    errno_t err;
    if ((err = fopen_s(&cascadeFile, cascadeFileName.data(), "r")) != 0)
        Log::error("Could not load the cascade file.", "FaceTrackerNode::FaceTrackerNode");
	else
		fclose(cascadeFile);

    // load the cascade file
    m_haarClassifierCascade = (CvHaarClassifierCascade *) cvLoad(cascadeFileName.data());
    if (!m_haarClassifierCascade)
        Log::error("Could not load the cascade file.", "FaceTrackerNode::FaceTrackerNode");

    // initialize memory storage used for face detection
    m_memStorage = cvCreateMemStorage(0);

    // Set processing function for run switch.
    Parameter *runParameter = getParameter("run");
    if (runParameter)
        runParameter->setChangeFunction(SLOT(toggleRun()));

    // Set processing function for showcvwin switch.
    Parameter *windowParameter = getParameter("showcvwin");
    if (windowParameter)
        windowParameter->setChangeFunction(SLOT(toggleWindow()));

    QObject::connect(&m_timer, SIGNAL(timeout()), SLOT(onTimeout()));
}


//!
//! Destructor of the FaceTrackerNode class.
//!
FaceTrackerNode::~FaceTrackerNode ()
{
    m_stopRequested = true;
    while(isRunning()) {};

    if (m_capture)
        cvReleaseCapture(&m_capture);

    //if (m_image)
    //    cvReleaseImage(&m_image);
    
    if (m_memStorage) {
        cvReleaseMemStorage(&m_memStorage);
    }

    if (m_haarClassifierCascade) {
        cvReleaseHaarClassifierCascade(&m_haarClassifierCascade);
        //delete m_haarClassifierCascade;
        //m_haarClassifierCascade = NULL;
    }

    for (unsigned int i = 0; i < m_faces.size(); ++i)
        delete m_faces.at(i);
    m_faces.clear();

	cvDestroyWindow("Tracking Window");
	cvDestroyAllWindows();
	m_cvWindowHandle = 0;
}


///
/// Public Functions
///

//!
//! Main function for vision handling for the virtual agent.
//! Will be run in an own thread.
//!
void FaceTrackerNode::run()
{
    while (true)
    {
        // if stopping of thread has been requested, do so
        if ( !captureImageFromWebcam(&m_image) || m_stopRequested )
        {
            quit();
            return;
        }

        // detect and show faces in scene (if not in mouse control mode)
        detectFaces(m_image);
        drawDetectedFaces(m_image);

        // add debug information to image
        drawDebugInfo(m_image);
        decreaseProbabilities();


        if (isRunning()) {
            // send a message with the current list of faces as attractors to the Behavior system's message cache
            try {
                unsigned int attractorCount = (unsigned int) m_faces.size();
                //std::cout << "Faces: " << m_faces.size() << std::endl; 

                //if(m_lastTimestamp.msecsTo(m_timestamp) > 0) {
                m_lastTimestamp = m_timestamp;
                //if(abs(m_focusPoint.x - m_focusPointTarget.x) < 0.01 && abs(m_focusPoint.y - m_focusPointTarget.y) < 0.01) {
                m_attractors.clear();
                Attractor attractor;
                // use list of faces as attractors
                double probability = getDoubleValue("probability");
                for (unsigned int i = 0; i < attractorCount; ++i) {
                    //attractors[i].focusPoint = m_faces.at(i)->getPointOfAttraction();
                    //attractors[i].size       = m_faces.at(i)->getRect().w / m_image->width;
                    float prob = m_faces.at(i)->getProbability();   
                    //std::cout << "Prob: " << prob << std::endl; 
                    if(m_faces.at(i)->getProbability() > probability) {
                        //std::cout << i <<"fx: " << m_faces.at(i)->getPointOfAttraction().x << "fy: " << m_faces.at(i)->getPointOfAttraction().y << std::endl;
                        attractor.focusPoint = m_faces.at(i)->getPointOfAttraction();
                        attractor.size       = m_faces.at(i)->getRect().w / m_image->width;
                        m_attractors.push_back(attractor);
                    }
                }
                processAttractors();
                animateFocusPoint();
                //if (m_attractors.size() == 0)
                //    setValue("noFaceDetected", 1, true);
                //else
                //    setValue("noFaceDetected", 0, true);
                //setValue("faceX", m_focusPoint.x, true);
                //setValue("faceY", m_focusPoint.y, true);
            }
            catch (std::bad_alloc &) {
                //BorStatus::showError(//BorStatus::format("FaceTrackerNode::DoIt", "Error allocating memory for list of attractors. Message has not been sent."), true);
            }       
        }
        // NILZ: ADD: this is just a hack
        if ( m_cvWindowHandle != 0 )
        {
            //const char* winName = cvGetWindowName(m_cvWindowHandle);
            cvShowImage("Tracking Window", m_image);
        }
        unsigned int delay = getValue("delay").toUInt();
        msleep(delay);
    }
}


///
/// Public Slots
///


//!
//! Starts and stops the tracker.
//!
void FaceTrackerNode::toggleRun ()
{
    bool run = getValue("run").toBool();
    if ( run ) {
        m_capture = cvCaptureFromCAM(0);
        m_stopRequested = false;
        m_cvWindowHandle = cvNamedWindow("Tracking Window", CV_WINDOW_AUTOSIZE);
        cvMoveWindow("Tracking Window", 100, 100);
        start(QThread::LowestPriority);
        m_timer.start(10);
    }
    else {
        //cvReleaseCapture(&m_capture);
		cvDestroyWindow("Tracking Window");
        cvDestroyAllWindows();
        m_cvWindowHandle = 0;
        m_stopRequested = true;
        setValue("showcvwin", false);
        m_timer.stop();
    }
}


//!
//! Shows and hides tracking window.
//!
void FaceTrackerNode::toggleWindow ()
{
    bool show = getValue("showcvwin").toBool();

    if ( show ) {
        if ( !isRunning() ) {
            setValue("showcvwin", false);
            return;
        }
        m_cvWindowHandle = cvNamedWindow("Tracking Window", CV_WINDOW_AUTOSIZE);
        cvMoveWindow("Tracking Window", 100, 100);
    }
    else {
        cvDestroyWindow("Tracking Window");
        cvDestroyAllWindows();
        m_cvWindowHandle = 0;
    }

}

/////
/////   PRIVATE SLOTS
/////
//
////!
////! Shows and hides tracking window.
////!
void FaceTrackerNode::onTimeout ()
{
    double oldX = getValue("faceX").toDouble();
    double oldY = getValue("faceY").toDouble();

    m_mutex.lock();
    float deltaX = (m_focusPointTarget.x - oldX);
    float deltaY = (m_focusPointTarget.y - oldY);
    m_mutex.unlock();
    
    
    double length = sqrt(deltaX * deltaX + deltaY * deltaY);
    double animationSpeed = getDoubleValue("animationSpeed");
    double focusPointSpeed = getDoubleValue("focusPointSpeed");
    double focusPointMoveLimit = getDoubleValue("focusPointMoveLimit");
        
    double speed = 0.0;
    int switchDelay = getIntValue("switchDelay");
    //std::cout << switchDelay << std::endl;
    if (switchDelay == 0 || m_justSwitched > 0)
        speed = animationSpeed;
    else
        speed = focusPointSpeed;
        
    double newX = oldX + deltaX * speed;
    double newY = oldY + deltaY * speed;

    if (m_attractors.size() == 0)
        setValue("noFaceDetected", 1, true);
    else
        setValue("noFaceDetected", 0, true);

    setValue("faceX", newX, true);
    setValue("faceY", newY, true);       
}

///
///   PRIVATE FUNCTIONS
///

//!
//! Handler for attractor list.
//! Updates the coordinates of the focus point depending on the list of attractors.
//!
void FaceTrackerNode::processAttractors ()
{
    unsigned int attractorCount = m_attractors.size();
    m_attractorIndexOld = m_attractorIndex;
    int switchTime = getValue("switchTime").toInt();
    int switchTimeRand = getValue("switchTimeRand").toInt();
    int switchUserNow = getValue("switchUserNow").toBool();

    if (attractorCount == 1) {
        // select single attractor
        if ( QTime::currentTime() > m_focusTimeLimit)
          m_attractorIndex = 0;
    } 
    else {
        // check if current attractor has been focused long enough and if no main animation is currently running
        if (m_switchUserNow || QTime::currentTime() > m_focusTimeLimit) {
            m_switchUserNow = false;
            m_justSwitched = getIntValue("switchDelay");
            m_attractorIndex = attractorCount == 2
                ? !m_attractorIndex                // switch between 2 attractors
                : getRandom(attractorCount);       // select random attractor from the list
        }
    }
    // update the focus time limit if the attractor index has changed
    if (m_attractorIndex != m_attractorIndexOld) {
        m_focusTimeLimit = QTime::currentTime();
        //std::cout << "after: " << m_focusTimeLimit.toString().toStdString();
        m_focusTimeLimit = m_focusTimeLimit.addMSecs(switchTime + getRandom((unsigned int) switchTimeRand));
        //std::cout << "after: " << m_focusTimeLimit.toString().toStdString() << std::endl;
    }
    // check if a valid index into the list has been selected
    if (m_attractorIndex >= 0 && m_attractorIndex < (int) attractorCount) {
        // use the focus point of the currently selected attractor as the target point to look at
        m_mutex.lock();
        m_focusPointTarget = m_attractors[m_attractorIndex].focusPoint;
        m_mutex.unlock();
    }
    if (m_justSwitched > 0)
        m_justSwitched--;
}

//!
//! Animates the focus point according to the focus point target and whether or not a face is currently focused.
//!
void FaceTrackerNode::animateFocusPoint ()
{
    m_mutex.lock();
    int switchTime = getValue("switchTime").toInt();
    int switchTimeRand = getValue("switchTimeRand").toInt();
    bool randomLook = getValue("randomLook").toBool();
    double focusPointSpeed = getValue("focusPointSpeed").toDouble();
    double focusPointMoveLimit = getValue("focusPointMoveLimit").toDouble();

    // look around randomly (if activated)
    if (randomLook && QTime::currentTime() > m_focusTimeLimit && getRandom() > 0.95f) {
        m_attractorIndex     = -1;
        m_focusPointTarget.x = getRandom();
        m_focusPointTarget.y = getRandom();
        m_focusTimeLimit     = QTime::currentTime(); 
        m_focusTimeLimit.addMSecs(switchTime + getRandom((unsigned int) switchTimeRand));
    }

    // animate focus point
    PointNDC delta;
    delta.x = (m_focusPointTarget.x - m_focusPoint.x);
    delta.y = (m_focusPointTarget.y - m_focusPoint.y);
    double length = sqrt(delta.x * delta.x + delta.y * delta.y);
    //if (length > focusPointMoveLimit) {
        //m_focusPoint.x += delta.x * focusPointSpeed;
        //m_focusPoint.y += delta.y * focusPointSpeed;
        m_focusPoint.x = m_focusPointTarget.x;
        m_focusPoint.y = m_focusPointTarget.y;
    //}
    m_mutex.unlock();
    //if (abs(delta.x) > focusPointMoveLimit)
    //    m_focusPoint.x += delta.x;
    //if (abs(delta.y) > focusPointMoveLimit)
    //    m_focusPoint.y += delta.y;
}

//!
//! Captures an image from a webcam and returns it in the given image.
//!
//! \param image    returns the image taken by the webcam
//! \return True, if an image was successfully captured from the webcam, otherwise False.
//!
bool FaceTrackerNode::captureImageFromWebcam(IplImage **image)
{
    if (!isRunning())
        return false;

    // try to grab a frame from the webcam
    if (!m_capture || !cvGrabFrame(m_capture))
        return false;

    // try to retrieve the grabbed frame from the webcam
    IplImage *capturedFrame = cvRetrieveFrame(m_capture);
    if (!capturedFrame)
        return false;

    if (!isRunning())
        return false;

    bool result = true;

    // resize frame
    int width = capturedFrame->width;
    int height = capturedFrame->height;
    int customWidth = getUnsignedIntValue("imageWidth");
    if (customWidth > 0)
        width = customWidth;
    int customHeight = getUnsignedIntValue("imageHeight");
    if (customHeight > 0)
        height = customHeight;


    IplImage *frame = cvCreateImage(cvSize(width, height), capturedFrame->depth, capturedFrame->nChannels);
    //IplImage *frame = cvCreateImage(cvSize(capturedFrame->width, capturedFrame->height), capturedFrame->depth, capturedFrame->nChannels);
    //IplImage *frame = cvCreateImage(cvSize(capturedFrame->width, capturedFrame->height), IPL_DEPTH_8U, 1);
	//cvCvtColor(capturedFrame, frame, CV_BGR2GRAY);
	cvResize(capturedFrame, frame, CV_INTER_NN);    // nearest neighbour
	

    // create the image with the size of the captured frame if it doesn't exist
  //  if (!*image) {
  //      *image = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
		////*image = cvCreateImage(cvSize(capturedFrame->width, capturedFrame->height), IPL_DEPTH_8U, 1);
  //      // initialize mouse position to center of image
		////m_mouseX = (*image)->width  / 2;
  //      //m_mouseY = (*image)->height / 2;
  //  }
    if (!*image || (*image)->width != width || (*image)->height != height) {
        if (*image)
            cvReleaseImage(&*image);
        *image = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, frame->nChannels);
    }

    // copy the frame to the image
	if (capturedFrame->origin == IPL_ORIGIN_TL) {
        cvFlip(frame, *image, 1);
	}  
	else {
		//! \todo NILZ: get from opition later
		//Options::Core *optionsCore = Options::Core::GetInstance();
        //if (optionsCore)
		//if (true)
  //          //cvFlip(frame, *image, optionsCore->GetBool("Flip captured image") ? -1 : 0);
  //          cvFlip(frame, *image, -1);
  //      else
  //          result = false;
    }

    cvReleaseImage(&frame);

    return result;
}


//!
//! Detects all faces in the given image, and creates or updates face objects for each detected face.
//!
//! \param image    the image to detect faces in
//!
void FaceTrackerNode::detectFaces(IplImage *image)
{
    if (!isRunning())
        return;

    if (!image)
        return;

    RectNDC detectionRect;
    //int     imageWidth  = image->width;
    //int     imageHeight = image->height;
	float   scale = 2.0;

	IplImage *gray = cvCreateImage( cvSize(image->width, image->height), 8, 1 );
    IplImage *small_img = cvCreateImage( cvSize( cvRound (image->width/scale), cvRound (image->height/scale)), 8, 1 );
	int     imageWidth  = small_img->width;
    int     imageHeight = small_img->height;

    cvCvtColor( image, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage(m_memStorage);

    CvSeq *detectedObjects = cvHaarDetectObjects(
        small_img,                      // Image to detect objects in.
        m_haarClassifierCascade,    // Haar classifier cascade in internal representation.
        m_memStorage,               // Memory storage to store the resultant sequence of the object candidate rectangles.
        1.1,                        // The factor by which the search window is scaled between the subsequent scans, for example, 1.1 means increasing window by 10%.
        2,                          // Minimum number (minus 1) of neighbor rectangles that makes up an object. All the groups of a smaller number of rectangles than min_neighbors-1 are rejected. If min_neighbors is 0, the function does not any grouping at all and returns all the detected candidate rectangles, which may be useful if the user wants to apply a customized grouping procedure.
        CV_HAAR_DO_CANNY_PRUNING,   // Mode of operation. Currently the only flag that may be specified is CV_HAAR_DO_CANNY_PRUNING. If it is set, the function uses Canny edge detector to reject some image regions that contain too few or too much edges and thus can not contain the searched object. The particular threshold values are tuned for face detection and in this case the pruning speeds up the processing.
        cvSize(30, 30)              // The initial size of the search window.
    );

	cvReleaseImage( &gray );
    cvReleaseImage( &small_img );

    m_detectedObjectsCount = detectedObjects
                           ? detectedObjects->total
                           : 0;

    // iterate over all detected objects
    for (int i = 0; i < m_detectedObjectsCount; ++i) {
        CvRect *r = (CvRect *) cvGetSeqElem(detectedObjects, i);
        // get normalized device coordinates of rectangle
        detectionRect.x = (float) r->x      / imageWidth;
        detectionRect.y = (float) r->y      / imageHeight;
        detectionRect.w = (float) r->width  / imageWidth;
        detectionRect.h = (float) r->height / imageHeight;
        // iterate over all existing faces and check if detected object can be assigned to a face
        unsigned int n = 0;
        bool found = false;
        while (n < m_faces.size() && !found) {
			found = m_faces.at(n)->checkDetectionRect(detectionRect);
			if (!found)
                ++n;
        }
        // create new Face object if detection rect could not be associated with any existing face
        double minSize = getDoubleValue("minSize");
		double maxSize = getDoubleValue("maxSize");
		if (!found && detectionRect.w > minSize && detectionRect.w < maxSize)
				m_faces.push_back(new Face(detectionRect));	
    }
}


//!
//! Draws an overlay onto the image that shows the locations of detected faces in the scene.
//!
//! \param image    the image to show detected faces in
//!
void FaceTrackerNode::drawDetectedFaces(IplImage *image)
{
    if (!isRunning())
        return;

    if (!image)
        return;

    for (unsigned int i = 0; i < m_faces.size(); ++i) {
        Face     *face          = m_faces.at(i);

        // retrieve point positions in normalized device coordinates
        RectNDC   rect          = face->getRect();
        PointNDC  poa           = face->getPointOfAttraction();

        // calculate point positions
        CvPoint   pTopLeft      = toCvPoint(image, rect);
        CvPoint   pBottomRight  = toCvPoint(image, rect.x + rect.w, rect.y + rect.h);
        CvPoint   pCenter       = toCvPoint(image, poa);

        // calculate color from probability value (0.0..0.5..1.0 => red..yellow..green)
        float     probab        = face->getProbability();
        CvScalar  color         = CV_RGB(MIN(255 * (1.5f - probab), 255), MIN(255 * probab * 2, 255), 0);

        // draw face bounding rect
        cvRectangle(image, pTopLeft, pBottomRight, color, 3, 8, 0);

        // draw point of attraction
        cvCircle(image, pCenter, 5, color);
    }
}

//!
//! Draws some debug information into the given image.
//!
//! \param image    pointer to the image to draw the debug info in
//!
void FaceTrackerNode::drawDebugInfo(IplImage *image)
{
    if (!isRunning())
        return;

    if (!image)
        return;

    CvFont *font = new CvFont();
    cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX, 0.35f, 0.35f, 0, 1, 8);
    char buffer[200];
    CvScalar colorBlack = CV_RGB(  0,   0,   0);
    CvScalar colorWhite = CV_RGB(255, 255, 255);

	// draw the number of faces in the image
	switch (m_detectedObjectsCount) {
			case 0:  sprintf_s(buffer, 200, "no faces detected"); break;
			case 1:  sprintf_s(buffer, 200, "%d face detected",  m_detectedObjectsCount); break;
			default: sprintf_s(buffer, 200, "%d faces detected", m_detectedObjectsCount);
	}
	cvPutText(image, buffer, cvPoint(11, 21), font, colorBlack);
	cvPutText(image, buffer, cvPoint(10, 20), font, colorWhite);

	switch (m_faces.size()) {
			case 0:  sprintf_s(buffer, 200, "no faces in list"); break;
			case 1:  sprintf_s(buffer, 200, "%d face in list",  m_faces.size()); break;
			default: sprintf_s(buffer, 200, "%d faces in list", m_faces.size());
	}
	cvPutText(image, buffer, cvPoint(11, 35), font, colorBlack);
	cvPutText(image, buffer, cvPoint(10, 34), font, colorWhite);
    

    free(font);
}


//!
//! Decreases the probability values of the detected faces.
//! Is called on each time step (/frame).
//!
void FaceTrackerNode::decreaseProbabilities()
{
    if (!isRunning()) {
        return;
    }

    // traverse all faces
    unsigned int i = 0;
    while (i < m_faces.size()) {
        // decrease probability of face
        m_faces.at(i)->decreaseProbability();
        // delete face if probability is (near) zero
        if (abs(m_faces.at(i)->getProbability()) < EPSILON) {
            delete m_faces.at(i);
            m_faces.erase(m_faces.begin() + i);
        } else
            ++i;
    }
}