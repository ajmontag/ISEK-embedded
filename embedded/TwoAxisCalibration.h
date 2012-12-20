
#ifndef _CS309_A4_TWO_AXIS_CALIBRATION_H_
#define _CS309_A4_TWO_AXIS_CALIBRATION_H_

#include "arduino_comm.h"
#include "TwoAxisControl.h"
#include "configurator.h"
#include <iostream>

/** 
 * A class for determining Calibration settings for the CalibratedServos on the Arduino. 
 */
class TwoAxisCalibration {

public:

    typedef enum {MIN, MAX} boundary_t;

    /** 
     * ctor
     * @param tac the TwoAxisControl for the servos to calibrate. 
     */
    TwoAxisCalibration(TwoAxisControl& tacontrol);

    /** 
     * dtor
     */
    ~TwoAxisCalibration();

    /**
     * Centers both the servo positions. Must be called at the start of the calibration routine.
     */
    void center();

    /**
     * Moves the specified servo as the number of degrees indicated.
     * @param deg degrees to move relative to current position.
     *      Can be positive or negative.
     */
    void move(TwoAxisControl::axis_t a, int deg);

    /**
     * Set the current position as the limit for the given boundary.
     */
    void set(TwoAxisControl::axis_t a, boundary_t bound);

    /**
     * Saves the boundaties to the given configuration file.
     * Should be called only after all 4 boundries have been set.
     */
    void saveToConfigFile(Configurator& config);

private:

    TwoAxisControl& tacontrol_;
    int xPos_, yPos_, xMin_, xMax_, yMin_, yMax_;

};

inline TwoAxisCalibration::TwoAxisCalibration(TwoAxisControl& tacontrol) :
    tacontrol_(tacontrol), xPos_(-1), yPos_(-1), xMin_(-1), xMax_(-1), yMin_(-1), yMax_(-1)
{
    // do nothing
}

inline TwoAxisCalibration::~TwoAxisCalibration()
{
    // do nothing
}

inline void TwoAxisCalibration::center()
{
    tacontrol_.goToAbsoluteCenter();
    xPos_ = 90;
    yPos_ = 90;
}

inline void TwoAxisCalibration::move(TwoAxisControl::axis_t a, int deg)
{
    switch (a) {
    case TwoAxisControl::X_AXIS:
        xPos_ += deg;
        if (xPos_ < 0) xPos_ = 0; 
        if (xPos_ > 180) xPos_ = 180; 
        tacontrol_.goToAbsolutePos(a, xPos_);
        break;
    case TwoAxisControl::Y_AXIS:
        yPos_ += deg;
        if (yPos_ < 0) yPos_ = 0; 
        if (yPos_ > 180) yPos_ = 180; 
        tacontrol_.goToAbsolutePos(a, yPos_);
        break;
    }
}

inline void TwoAxisCalibration::set(TwoAxisControl::axis_t a, boundary_t bound)
{
    switch (a) {
    case TwoAxisControl::X_AXIS:
        switch (bound) {
        case MIN:
            xMin_ = xPos_;
            break;
        case MAX:
            xMax_ = xPos_;
            break;
        }
        break;
    case TwoAxisControl::Y_AXIS:
        switch (bound) {
        case MIN:
            yMin_ = yPos_;
            break;
        case MAX:
            yMax_ = yPos_;
            break;
        }
        break;
    }
}

inline void TwoAxisCalibration::saveToConfigFile(Configurator& config)
{
    config.saveLimits(yMax_, yMin_, xMax_, xMin_);
}


#endif // _CS309_A4_TWO_AXIS_CALIBRATION_H_
