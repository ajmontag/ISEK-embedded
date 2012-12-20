
#ifndef _CS309_A4_ARDUINO_COMM_H_
#define _CS309_A4_ARDUINO_COMM_H_

#include <stdint.h>   /* Standard types */

typedef int arduino_comm_handle; 

/**
 * takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
 * and a baud rate (bps) and connects to that port at that speed and 8N1.
 * opens the port in fully raw mode so you can send binary data.
 *returns valid fd, or -1 on error
 */
arduino_comm_handle arduino_comm_init(const char* serialport, int baud);

int arduino_comm_writebyte(arduino_comm_handle fd, uint8_t b);
int arduino_comm_writebytes(arduino_comm_handle fd, const uint8_t* buff, int len);

#endif // _CS309_A4_ARDUINO_COMM_H_
