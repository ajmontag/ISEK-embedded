
#ifndef _CS309_A4_TWO_AXIS_CONTROL_H_
#define _CS309_A4_TWO_AXIS_CONTROL_H_

#include "arduino_comm.h"
#include <iostream>

#include "../arduino/two_axis_control/two_axis_control_cmd.h"

/**
 * A class for controlling two CalibratedServos on the Arduino via serial. 
 */
class TwoAxisControl {

public:

    typedef enum { X_AXIS = SERVO_X_INDEX, Y_AXIS = SERVO_Y_INDEX } axis_t;

    /** 
     * ctor
     * does nothing
     */
    TwoAxisControl();

    /**
     * dtor
     * closes the serial port
     */
    virtual ~TwoAxisControl();

    /** 
     * @brief initialized the tac for use. 
     * @param serialPortPath the serail port to use. ex: /dev/ttyUSB0
     * @param xPin yPin the pins that the servos are on
     * @param baud the baud rate to use for serail communication
     * @throws std::exception on a serial open error
     */
    void init(const char* serialPortPath, unsigned char xPin, unsigned char yPin, int baud = 9600);

    /**
     * @brief control methods for servos
     * @throws std::exception on serial write error
     */
    void goToAbsolutePos(axis_t a, int degree) const;
    void goToRelativePos(axis_t a, double percent) const;
    void goToRelativeCenter() const;
    void goToAbsoluteCenter() const;

    /** 
     * @brief initiates or terminates scanning for new faces on the Arduino
     */
    void scanStart() const;
    void scanStop() const;

    /**
     * Tells the Arduino to move the servos in the direction as indicated.
     * @param degree the angle at which to move, in [0, 360)
     */
    void travelStart(double degree) const;
    void travelStop() const;

//private:

    arduino_comm_handle handle_;

    void setMaxPos(axis_t a, unsigned char degree) const;
    void setMinPos(axis_t a, unsigned char degree) const;

    void attachServo(axis_t a, unsigned char pin) const;

    void send(unsigned char cmd, unsigned char arg0, unsigned char arg1) const;

    mutable bool stopped_; 

};

inline TwoAxisControl::TwoAxisControl() : handle_(NULL), stopped_(false)
{
    // do nothing
}

inline TwoAxisControl::~TwoAxisControl()
{
    close(handle_);
}

inline void TwoAxisControl::init(const char* serialPortPath, unsigned char xPin, unsigned char yPin, int baud)
{
    std::cout << "TwoAxisControl::init x = " << (int) xPin << ", y = " << (int) yPin << std::endl;
    handle_ = arduino_comm_init(serialPortPath, baud);
    if (handle_ == -1) {
        throw std::exception(); // TODO error opening serial port
    }

    attachServo(X_AXIS, xPin);
    attachServo(Y_AXIS, yPin);
}

inline void TwoAxisControl::goToAbsolutePos(axis_t a, int degree) const
{
    send(CMD_GO_TO_POS, a, degree);
}

inline void TwoAxisControl::goToRelativePos(axis_t a, double percent) const
{
    send(CMD_GO_TO_RELATIVE, a, static_cast<char>(percent * 100.0));
}

inline void TwoAxisControl::goToRelativeCenter() const
{
    goToRelativePos(X_AXIS, 50);
    goToRelativePos(Y_AXIS, 50);
}

inline void TwoAxisControl::goToAbsoluteCenter() const
{
    goToAbsolutePos(X_AXIS, 90);
    goToAbsolutePos(Y_AXIS, 90);
}


inline void TwoAxisControl::scanStart() const
{
    stopped_ = false; 
    send(CMD_SCAN_START, 0, 0);
}

inline void TwoAxisControl::scanStop() const
{
    send(CMD_SCAN_STOP, X_AXIS, 0);
}

inline void TwoAxisControl::travelStart(double degree) const
{
    stopped_ = false; 
    //std::cout << "TwoAxisControl::travelStart(" << degree << ")" << std::endl;
    short s = (short) degree;
    send(CMD_BEGIN_TRAVEL, (unsigned char) (s & 0x00FF), (unsigned char) ((s >> 8) & 0x00FF));
}

inline void TwoAxisControl::travelStop() const
{
    //std::cout << "TwoAxisControl::travelStop()" << std::endl;
    if (stopped_) return; 
    stopped_ = true; 
    send(CMD_END_TRAVEL, 0, 0);
}

inline void TwoAxisControl::setMaxPos(axis_t a, unsigned char degree) const
{
    send(CMD_SET_MAX_POS, a, degree);
}

inline void TwoAxisControl::setMinPos(axis_t a, unsigned char degree) const
{
    send(CMD_SET_MIN_POS, a, degree);
}

inline void TwoAxisControl::attachServo(axis_t a, unsigned char pin) const
{
    send(CMD_ADD_SERVO, a, pin);
}

inline void TwoAxisControl::send(unsigned char cmd, unsigned char arg0, unsigned char arg1) const
{
    unsigned char buff[3];
    buff[0] = cmd;
    buff[1] = arg0;
    buff[2] = arg1;
    const int result = arduino_comm_writebytes(handle_, buff, 3);

    if (result == -1) {
        throw std::exception(); // TODO serial port runtime error
    }
}


#endif // _CS309_A4_TWO_AXIS_CONTROL_H_
