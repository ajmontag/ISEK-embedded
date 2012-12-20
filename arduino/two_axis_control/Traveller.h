/**
 * Traveller.h
 */

#ifndef _CS309_A4_TRAVELLER_H_
#define _CS309_A4_TRAVELLER_H_

#include <math.h>

#define DELTA_MAX 4

// fwd declare
class CalServo;

/**
 * A class for moving the servos.
 * This implementation does nothing,
 * it is ment to be inherited.
 * @see Traveller
 * @see Scanner
 */
class Traveller : public Mover
{

public:

    Traveller();

    virtual void init();

    /**
     * 50 is the middle of the calibrated area.
     * 0 is left, 100 is right
     */
    void init(short degree);

    virtual void stop();

protected:

    friend class UnitTesting; // TODO testing only

    virtual Mover::coord_t getNextPos();

    virtual long getDelayTime();

    byte maxSpeed_;

    byte speedX_;
    byte speedY_;

    bool xNeg_;
    bool yNeg_;

    int deltaX_;
    int deltaY_;

    bool hasStepped; 

};

inline Traveller::Traveller() : maxSpeed_(3), speedX_(0), speedY_(0), deltaX_(0), deltaY_(0) { /* do nothing */ }

inline void Traveller::init()
{
    // never to be used
}

inline void Traveller::init(short degree)
{
    // initialize travel params

    bool xNeg, yNeg;
    double rad = 0;

    if (degree < 90) {
        rad = degree * (M_PI / 180.0);
        xNeg = false;
        yNeg = false;
    } else if (degree < 180) {
        // quadrant II
        rad = (180.0 - degree) * (M_PI / 180.0);
        xNeg = true;
        yNeg = false;
    } else if (degree < 270) {
        // quadrant III
        rad = (degree - 180.0) * (M_PI / 180.0);
        xNeg = true;
        yNeg = true;
    } else {
        // quadrant IV
        rad = (360.0 - degree) * (M_PI / 180.0);
        xNeg = false;
        yNeg = true;
    }

    deltaY_ = sin(rad) * maxSpeed_;
    deltaX_ = cos(rad) * maxSpeed_;

    if (xNeg) {
        deltaX_ *= -1;
    }

    if (yNeg) {
        deltaY_ *= -1;
    }

    hasStepped = false; 
}

inline void Traveller::stop()
{
    /* nothing to do here */
}

inline Mover::coord_t Traveller::getNextPos()
{
    coord_t ret;

    if (hasStepped)
        return ret; 

    int xPos = servoX_->calibratedRead();
    int yPos = servoY_->calibratedRead();

    xPos += deltaX_;
    yPos += deltaY_;

    if (xPos >= 0 && xPos < 100) {
        ret.x = xPos;
    }

    if (yPos >= 0 && yPos < 100) {
        ret.y = yPos;
    }

    hasStepped = true; 
    return ret;
}

inline long Traveller::getDelayTime()
{
    // determines the speed at which the robotic head moves
    // this value seems to work well 
    return 5;
}

#endif // _CS309_A4_TRAVELLER_H_
