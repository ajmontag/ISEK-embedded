/**
 * Display video from webcam
 *
 * Author  Nash
 * License GPL
 * Website http://nashruddin.com
 */

#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <time.h>

int main( int argc, char **argv )
{
    CvCapture *capture = 0;
    IplImage  *frame = 0;
    int       key = 0;
    int t;

    /* initialize camera */
    capture = cvCaptureFromCAM( 0 );

    /* always check */
    if ( !capture ) {
        fprintf( stderr, "Cannot open initialize webcam!\n" );
        return 1;
    }

    /* create a window for the video */
    cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );

    
    while( key != 'q' ) {
        t = (double)cvGetTickCount();
	printf("%g", t/((double)cvGetTickFrequency()*1000.));
        /* get a frame */
        frame = cvQueryFrame( capture );
        t = (double)cvGetTickCount() - t;
        printf("get frame time = %g ms\n", t/((double)cvGetTickFrequency()*1000.));

        /* always check */
        if( !frame ) break;

        t = (double)cvGetTickCount();
        printf("%g", t/((double)cvGetTickFrequency()*1000.));
        /* display current frame */
        cvShowImage( "result", frame );
        t = (double)cvGetTickCount() - t;
        printf("show time = %g ms\n", t/((double)cvGetTickFrequency()*1000.));

        /* exit if user press 'q' */
        key = cvWaitKey( 1 );
    }

    /* free memory */
    cvDestroyWindow( "result" );
    cvReleaseCapture( &capture );

    return 0;
}