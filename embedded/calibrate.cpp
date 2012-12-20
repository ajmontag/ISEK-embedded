#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"
#include "TwoAxisControl.h"
#include "TwoAxisCalibration.h"
#include "configurator.h"

int main(int argc, char** argv)
{
	TwoAxisControl tac;
	Configurator config("configFile.xml");
	TwoAxisControl::axis_t axis = TwoAxisControl::Y_AXIS;
	TwoAxisCalibration::boundary_t bound = TwoAxisCalibration::MIN;

	bool done = 0;

	const char* serialPortPath = config.get_serialportpath();
	tac.init(serialPortPath, config.get_servo1(), config.get_servo2());
	TwoAxisCalibration tacal(tac);
	tacal.center();

	const char* name = "Calibrate Servos";
	cvResizeWindow( name, 300, 200 );
	IplImage *img1 = cvLoadImage("images/lower.png");
	IplImage *img2 = cvLoadImage("images/upper.png");
	IplImage *img3 = cvLoadImage("images/left.png");
	IplImage *img4 = cvLoadImage("images/right.png");

	cvNamedWindow( name );

	// Main loop
	while( !done ){
		// display set upper limit
		
		switch (axis) {
	    case TwoAxisControl::Y_AXIS:
	        switch (bound) {
	        case TwoAxisCalibration::MIN:
	            cvShowImage(name, img1);
	            cvWaitKey(10);  
	            break;
	        case TwoAxisCalibration::MAX:
	            cvShowImage(name, img2);
	            cvWaitKey(10);
	            break;
	        }
	        break;
	    case TwoAxisControl::X_AXIS:
	        switch (bound) {
	        case TwoAxisCalibration::MIN:
	            cvShowImage(name, img3);
	            cvWaitKey(10);
	            break;
	        case TwoAxisCalibration::MAX:
	            cvShowImage(name, img4);
	            cvWaitKey(10);
	            break;
	        }
	        break;
    	}

		char k = cvWaitKey(0);
		std:: cout << "key = " << (int) k << std::endl; 

		if(k == 82 && axis == TwoAxisControl::Y_AXIS ) // up
		{
			tacal.move(axis, 2);
		}
		else if(k == 84 && axis == TwoAxisControl::Y_AXIS ) // down
		{
			tacal.move(axis, -2);
		}
		else if(k == 81 && axis == TwoAxisControl::X_AXIS ) // right
		{
			tacal.move(axis, -2);
		}
		else if(k == 83 && axis == TwoAxisControl::X_AXIS) // left
		{
			tacal.move(axis, 2);
		}
		else if(k == 10 || k == -115) //enter
		{
			// send mesaage to save as step
			switch (axis) {
		    case TwoAxisControl::Y_AXIS:
		        switch (bound) {
		        case TwoAxisCalibration::MIN:
		            tacal.set(axis, bound);
		            bound = TwoAxisCalibration::MAX;
		            tacal.center();
		            break;
		        case TwoAxisCalibration::MAX:
		            tacal.set(axis, bound);
		            bound = TwoAxisCalibration::MIN;
			    	axis = TwoAxisControl::X_AXIS;
			    	tacal.center();
		            break;
		        }
		        break;
		    case TwoAxisControl::X_AXIS:
		        switch (bound) {
		        case TwoAxisCalibration::MIN:
		            tacal.set(axis, bound);
		            bound = TwoAxisCalibration::MAX;
		            tacal.center();
		            break;
		        case TwoAxisCalibration::MAX:
		            tacal.set(axis, bound);
		            done = true;
		            tacal.center();
		            break;
		      	}
		        break;
		    }
		}
	}

	tacal.saveToConfigFile(config);

	cvReleaseImage( &img1 );
	cvReleaseImage( &img2 );
	cvReleaseImage( &img3 );
	cvReleaseImage( &img4 );
	cvDestroyWindow( name );

	return 0;
}