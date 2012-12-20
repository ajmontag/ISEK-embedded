/*
    The following is an OpenCV example for facedetection.
    Changes include:
        Optimization
        Servo Control integration
        Face tracking

*/
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "TwoAxisControl.h"
#include "FaceTracker.hpp"
#include "TwoAxisControl_Mock.h"
#include "configurator.h"
#include "polar.h"

#include <iostream>
#include <stdio.h>
#include <math.h>

Configurator* config; 

using namespace std;
using namespace cv;

ostream& operator<<(ostream& ost, const polar_t& p) { 
    ost << p.theta_deg << " deg, " << p.r; 
    return ost; 
}

polar_t detectFace(Mat& img, CascadeClassifier& cascade, double scale);

void help()
{
    cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
            "This classifier can recognize many ~rigid objects, it's most known use is for faces.\n"
            "Usage:\n"
            "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>\n"
               "   [filename|camera_index]\n\n"
            "see facedetect.cmd for one call:\n"
            "./facedetect --cascade=\"../../data/haarcascades/haarcascade_frontalface_alt.xml\"  --scale=1.3 \n"
            "Hit any key to quit.\n"
            "Using OpenCV version " << CV_VERSION << "\n" << endl;
}


//String cascadeName = "/home/pi/OpenCV-2.3.1/data/haarcascades/haarcascade_frontalface_alt.xml";


int main( int argc, const char** argv )
{
    CvCapture* capture = 0;
    Mat frame, frameCopy, image;
    const String scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const String cascadeOpt = "--cascade=";
    size_t cascadeOptLen = cascadeOpt.length();

    help();

    CascadeClassifier cascade;
    double scale = 1;

    // initialize the arduino connection for servo control
    TwoAxisControl tac;

    // initialize configurator and assign values
    config = new Configurator("configFile.xml");

    const char* serialPortPath = config->get_serialportpath();
    if (serialPortPath) {
        tac.init(serialPortPath, config->get_servo1(), config->get_servo2());
    
        // TODO ajm set calibration information
//        tac.setMaxPos(TwoAxisControl::Y_AXIS, config->get_maxY());
//        tac.setMaxPos(TwoAxisControl::X_AXIS, config->get_maxX());
//        tac.setMinPos(TwoAxisControl::Y_AXIS, config->get_minY());
//        tac.setMinPos(TwoAxisControl::X_AXIS, config->get_minX());

        // start the head in the center
        tac.goToAbsoluteCenter();
    }

    /*
    for (int i = 1; i < argc; i++) {
        cout << "Processing " << i << " " <<  argv[i] << endl;
        if (cascadeOpt.compare(0, cascadeOptLen, argv[i], cascadeOptLen) == 0) {
            cascadeName.assign( argv[i] + cascadeOptLen );
            cout << "  from which we have cascadeName= " << cascadeName << endl;
        } else if (scaleOpt.compare(0, scaleOptLen, argv[i], scaleOptLen) == 0) {
            if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
                scale = 1;
            cout << " from which we read scale = " << scale << endl;
        } else if (argv[i][0] == '-') {
            cerr << "WARNING: Unknown option %s" << argv[i] << endl;
        } else {
            inputName.assign(argv[i]);
        }
    } */

    if (!cascade.load(config->get_haarcascade())) {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=<cascade_path>]\n"
            "   [--scale[=<image scale>\n"
            "   [filename|camera_index]\n" << endl ;
        return -1;
    }

    int c = config->get_CAM();
    capture = cvCaptureFromCAM(c);
    if (!capture) {
        cout << "Capture from CAM " <<  c << " didn't work" << endl;
    } else {
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, config->get_imageWidth());
        cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, config->get_imageHeight());
    }

    if (config->get_showGUI()) {
        cvNamedWindow("result", 1);
    }

    if (capture) {
        // main frame loop
        cout << "In capture ..." << endl;

        // face tracker used for buffering face finding data in main loop
        FaceTracker ftrack(tac, config->get_deadzone(), 100, 5); 

        for (bool done = false; !done ; ) {
            IplImage* iplImg = cvQueryFrame(capture);
            frame = iplImg;
            if (frame.empty())
                break;
            if (iplImg->origin == IPL_ORIGIN_TL)
                frame.copyTo( frameCopy );
            else
                flip( frame, frameCopy, 0 );

            const polar_t p = detectFace(frameCopy, cascade, scale);

            ftrack.processFrame(p);

            /* This program is inteded to run for the duration of the power-on time
               This line provides delay between frames: 5 ms
               read delay time from config file?
            */
            if (waitKey(5) >= 0)
                done = true;
        }

        // cleanup
        cvReleaseCapture( &capture );

    } else {
        cout << "Unable to Capture from Camera" << endl;
    }

    cvDestroyWindow("result");

    return 0;
}

/**
 * Returns the polar coordinates representing where the face is, or { -1, -1 } if no face is found
 */
polar_t detectFace(Mat& img, CascadeClassifier& cascade, double scale)
{
    polar_t ret = POLAR_NIL;
    vector<Rect> faces;

    Mat gray, smallImg(cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1);

    cvtColor(img, gray, CV_BGR2GRAY);
    resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
    equalizeHist(smallImg, smallImg);

    // profiling
    //t = (double) cvGetTickCount();

    //detectMultiScale(const Mat& image, vector<Rect>& objects, double scaleFactor=1.1, int minNeighbors=3, int flags=0, Size minSize=Size(), Size maxSize=Size())
    cascade.detectMultiScale(smallImg, faces,
        1.1, 2, 0
        |CV_HAAR_FIND_BIGGEST_OBJECT
        |CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        , Size(30, 30));

    // profiling
    //t = (double) cvGetTickCount() - t;
    //printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );


    // return early if no faces found
    if (faces.empty()) {
        // show the image to keep the video displaying 
        if (config->get_showGUI()) {
            cv::imshow("result", img);
        }
        return ret;
    }

    // use the largest face as the one to track
    int largestFaceIndex = 0;
    int largestFaceSize = faces[0].width * faces[0].height;
    for (int i = 1; i < faces.size(); ++i) {
        const int currSize = faces[i].width * faces[i].height;
        if (currSize > largestFaceSize) {
            largestFaceIndex = i;
        }
    }

    const Rect& largestFace = faces[largestFaceIndex];

    // find the center of the face using the rectangle 
    CvPoint2D32f center;
    center.x = (largestFace.x + largestFace.width * 0.5) * scale;
    center.y = (largestFace.y + largestFace.height * 0.5) * scale;

    // place a circle around the face and show it in the GUI
    if (config->get_showGUI()) {
        Scalar color = CV_RGB(0,0,255); // blue
        int draw_radius = cvRound((largestFace.width + largestFace.height) * 0.25 * scale);
        circle(img, center, draw_radius, color, 3, 8, 0);
        cv::imshow("result", img);
    }

    // Converting to a format where 0,0 is the center of the screen
    center.x = center.x - config->get_imageWidth()/2;
    center.y = -(center.y - config->get_imageHeight()/2);

    // Invert X and Y due to motors
    if (config->get_invertY())
        center.y = -(center.y);
    if (config->get_invertX())
        center.x = -(center.x);

    // determine raduis
    static const float frameDiag = sqrt((config->get_imageWidth() * config->get_imageWidth()) + (config->get_imageHeight() * config->get_imageHeight())); 
    const float radius = sqrt((center.x * center.x) + (center.y * center.y)) / frameDiag;

    // Calculating angle and converting to degrees
    float angle = atan(center.y / center.x) * 180.0 / 3.14159;

    if (center.x < 0.0f && center.y > 0.0f)       // Will receive 0 to -90 but need 90 to 180
        angle = angle + 180.0f;
    else if (center.x > 0.0f && center.y < 0.0f)  // Will receive 0 to -90 but need 270 to 360
        angle = angle + 360.0f;
    else if (center.x < 0.0f && center.y < 0.0f)  // Will receive 0 to 90 but need 180 to 270
        angle = angle + 180.0f;

    ret.theta_deg = angle; 
    ret.r = radius; 
    return ret;
}
