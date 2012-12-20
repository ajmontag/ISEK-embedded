/**
 * two_axis_control_cmd.h
 *
 * Header containing commands which the two_axis_control will respond to.
 *
 * Commands are to be of the following format:
 *      1 byte: command code
 *      1 byte: servo index
 *      ...   : any additional parameters
 */



#ifndef _CS309_A4_TWO_AXIS_CONTROL_CMD_H_
#define _CS309_A4_TWO_AXIS_CONTROL_CMD_H_

///////
// Command codes.
//////

/**
 * Params:
 *   1 byte: an integer in [0, 180]
 */
#define CMD_SET_MAX_POS     0x01
#define CMD_SET_MIN_POS     0x02
#define CMD_GO_TO_POS       0x04

/**
 * Params:
 *   1 byte: an integer in [0, 100], representing the
 *      percentage position of the calibrated range.
 */
#define CMD_GO_TO_RELATIVE  0x05

/**
 * Params:
 *  1 byte: the degree of the direction of travel, LSB
 *  1 byte: the degree of the direction of travel, MSB
 */
#define CMD_BEGIN_TRAVEL    0x08
#define CMD_END_TRAVEL      0x09

/**
 * Params:
 *   1 byte: the second servo index.
 * this means the packet will look like this:
 *   [CMD, X_INDEX, Y_INDEX]
 */
#define CMD_SCAN_START      0x06
#define CMD_SCAN_STOP       0x07


/**
 * Params:
 *   1 byte: the max speed
 * this is purely relative, its just a
 * value in [0, 100]. 0 = slow, 100 = fast
 */
#define CMD_SET_MAX_SPEED   0x0A

/**
 * Params:
 *   1 byte: the pin number for the servo.
 */
#define CMD_ADD_SERVO       0x03



/**
 * Servo indicies.
 * All movers move along x and y
 */
#define SERVO_X_INDEX       0
#define SERVO_Y_INDEX       1


#endif // _CS309_A4_TWO_AXIS_CONTROL_CMD_H_
