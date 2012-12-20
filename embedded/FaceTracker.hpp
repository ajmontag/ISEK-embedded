
#ifndef _CS309_A4_FACE_TRACKER_H_
#define _CS309_A4_FACE_TRACKER_H_

#include "TwoAxisControl.h"
#include "polar.h"
#include <iostream>

/** 
 * A class for correlating face locations to servo movements. 
 * Filters input as to avoid false positives/negatives. 
 * Initiates "travel" when a face is found. 
 * Initiates "scanning" when a face is not found. 
 */
class FaceTracker {

public:

    /** 
     * ctor
     * @param tac an already initialized TwoAxisControl for controlling servos. 
     * @param deadzone a value in [0, 1] which indicates the radius away from center for
     *          which to consider a face already in the center of the image. 
     * @param noFaceThreshold the number of frames to go before starting to scan for faces
     * @param faceConfirmFrames the number of frames for which a face should be present before
     *          attempting to follow it. 
     */
    FaceTracker(TwoAxisControl& tac, double deadzone, unsigned long noFaceThreshold, unsigned long faceConfirmFrames);

    /** 
     * dtor
     */
    virtual ~FaceTracker();

    /** 
     * @param p the location of the face in the frame, 
     *    or POLAR_NIL if no face was found in the current frame. 
     */
    void processFrame(const polar_t p);

private:

    TwoAxisControl& tac_; 

    const double deadzone_; 
    const unsigned long noFaceThreshold_;
    unsigned long noFaceFrameCount_; 
    const unsigned long faceConfirmFrames_; 
    unsigned long faceFrameCount_; 

    void onFoundFace(const polar_t& p); 
    void onNotFoundFace(); 

};

inline FaceTracker::FaceTracker(TwoAxisControl& tac, double deadzone, unsigned long noFaceThreshold, unsigned long faceConfirmFrames) : 
    tac_(tac), deadzone_(deadzone), noFaceThreshold_(noFaceThreshold), noFaceFrameCount_(0), faceConfirmFrames_(faceConfirmFrames), faceFrameCount_(0)
{
    // do nothing
}

inline FaceTracker::~FaceTracker()
{
    // do nothing
}

inline void FaceTracker::processFrame(const polar_t p) 
{
    if (p == POLAR_NIL) {
        // only call onFoundFace if we have seen it for a few frames. 
        // sometimes a random "face" shows up for one frame. 
        if (faceFrameCount_ > 0) {
            --faceFrameCount_;
        } else {
            ++noFaceFrameCount_;
            onNotFoundFace();  
        }
    } else {

        // only call onFoundFace if we have seen it for a few frames. 
        // sometimes a random "face" shows up for one frame. 
        if (faceFrameCount_ == faceConfirmFrames_) {
            noFaceFrameCount_ = 0; 
            onFoundFace(p);
        } else {
            ++faceFrameCount_;
            std::cout << "potential face, frame count = " << faceFrameCount_ << std::endl;  
        } 
    }
}

inline void FaceTracker::onFoundFace(const polar_t& p)
{
    //std::cout << "Face at: " << p.theta_deg << std::endl; 

    if (noFaceFrameCount_) {
        tac_.scanStop(); 
    }
    
    if (p.r > deadzone_) {
        // outside of deadzone, so we move
        tac_.travelStart(p.theta_deg);
    }
}

inline void FaceTracker::onNotFoundFace()
{    
    if (noFaceFrameCount_ == noFaceThreshold_) {
        tac_.travelStop();
        tac_.scanStart(); 
    }
}

#endif // _CS309_A4_FACE_TRACKER_H_
