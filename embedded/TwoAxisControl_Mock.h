
#ifndef _CS309_A4_TWO_AXIS_CONTROL_MOCK_H_
#define _CS309_A4_TWO_AXIS_CONTROL_MOCK_H_

#include <iostream>
#include "arduino_comm.h"

#include "../arduino/two_axis_control/two_axis_control_cmd.h"

class TwoAxisControlMock {

public:

    typedef enum { X_AXIS = SERVO_X_INDEX, Y_AXIS = SERVO_Y_INDEX } axis_t;

    TwoAxisControlMock();

    virtual ~TwoAxisControlMock();

    // throws
    void init(const char* serialPortPath, unsigned char xPin, unsigned char yPin, int baud = 9600);

    // throws
    void goToAbsolutePos(axis_t a, int degree) const;
    void goToRelativePos(axis_t a, double percent) const;

    void scanStart() const;
    void scanStop() const;

    /**
     * Tells the Arduino to move the servos in the direction as indicated.
     * @param degree the angle at which to move, in [0, 360)
     */
    void travelStart(double degree) const;
    void travelStop() const;

private:

    //arduino_comm_handle handle_;

    void setMaxPos(axis_t a, unsigned char degree) const;
    void setMinPos(axis_t a, unsigned char degree) const;

    void attachServo(axis_t a, unsigned char pin) const;

    void send(unsigned char cmd, unsigned char arg0, unsigned char arg1) const;

};

inline TwoAxisControlMock::TwoAxisControlMock()
{
    // do nothing
}

inline TwoAxisControlMock::~TwoAxisControlMock()
{
}

inline void TwoAxisControlMock::init(const char* serialPortPath, unsigned char xPin, unsigned char yPin, int baud)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::goToAbsolutePos(axis_t a, int degree) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::goToRelativePos(axis_t a, double percent) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::scanStart() const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::scanStop() const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::travelStart(double degree) const
{
        std::cout << __PRETTY_FUNCTION__ << degree << std::endl; 
}

inline void TwoAxisControlMock::travelStop() const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::setMaxPos(axis_t a, unsigned char degree) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::setMinPos(axis_t a, unsigned char degree) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::attachServo(axis_t a, unsigned char pin) const
{
    std::cout << __PRETTY_FUNCTION__ << std::endl; 
}

inline void TwoAxisControlMock::send(unsigned char cmd, unsigned char arg0, unsigned char arg1) const
{
   
}


#endif // _CS309_A4_TWO_AXIS_CONTROL_MOCK_H_
