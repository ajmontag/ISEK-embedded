
#include "arduino_comm.h"

#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
arduino_comm_handle arduino_comm_init(const char* serialport, int baud)
{
    struct termios toptions;
    arduino_comm_handle fd;
    fprintf(stderr,"init_serialport: opening port %s @ %d bps\n", serialport, baud);
    //fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    fd = open(serialport, O_RDWR | O_NOCTTY);

    if (fd == -1)  {
      perror("init_serialport: Unable to open port ");
      return -1;
    }
    if (tcgetattr(fd, &toptions) < 0) {
      perror("init_serialport: Couldn't get term attributes");
      return -1;
    }

    speed_t brate = baud; // let you override switch below if needed

    switch(baud) {
    case 4800:   brate = B4800;   break;
    case 9600:   brate = B9600;   break;
    case 19200:  brate = B19200;  break;
    case 38400:  brate = B38400;  break;
    case 57600:  brate = B57600;  break;
    case 115200: brate = B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;
    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
      perror("init_serialport: Couldn't set term attributes");
      return -1;
    }

    return fd;
}

int arduino_comm_writebyte(arduino_comm_handle fd, uint8_t b)
{
    int n = write(fd,&b,1);
    if( n!=1)
      return -1;
    return 0;
}

int arduino_comm_writebytes(int fd, const uint8_t* buff, int len)
{
    int n = write(fd, buff, len);
    if( n!=len )
      return -1;

    if(n == 3) {
        printf("Send %d, %d, %d\n", buff[0], buff[1], buff[2]);
        fflush(stdout);
    }

    return n;
}
