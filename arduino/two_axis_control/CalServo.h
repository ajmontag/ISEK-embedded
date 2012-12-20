/**
 * A calibrated servo.
 */

#ifndef _CS309_A4_CAL_SERVO_H_
#define _CS309_A4_CAL_SERVO_H_

#include <Arduino.h>

class CalServo : public Servo
{

public:

    CalServo() { clearCal(); }

    /**
     * Immediately moves the  servo to the position,
     * taking calibration values into account.
     */
    void calibratedWrite(int percent);
    int calibratedRead();

    /**
     * Set calibration values, in degrees.
     */
    void setMin(int min);
    void setMax(int max);

    void clearCal();

private:

    int min_;
    int max_;
};

inline void CalServo::calibratedWrite(int percent)
{
    const int degrees = map(percent, 0, 100, min_, max_);
    write(degrees);
}

inline int CalServo::calibratedRead()
{
    return map(read(), min_, max_, 0, 100);
}

inline void CalServo::setMin(int min)
{
    min_ = min;
}

inline void CalServo::setMax(int max)
{
    max_ = max;
}

inline void CalServo::clearCal()
{
    min_ = 0;
    max_ = 180;
}

#endif // _CS309_A4_CAL_SERVO_H_
