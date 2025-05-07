/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Serial.h"

Serial::Serial() : fd { -1 }
{
}

Serial::~Serial()
{
}

int Serial::open_port(const char *device, int baud)
{
  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1)
  {
    printf("Couldn't open serial device.\n");
    return -1;
  }

  struct termios newtio;

  tcgetattr(fd, &oldtio); 

  // For MIDI 31250 baud:
  // setserial -a -g /dev/ttyUSB0
  // setserial -v /dev/ttyUSB0 spd_cust divisor 768

  memset(&newtio, 0, sizeof(struct termios));
  newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;
  newtio.c_cc[VTIME] = 0;
  newtio.c_cc[VMIN] = 1;

  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);

  return 0;
}

void Serial::close_port()
{
  if (fd != -1)
  {
    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);
  }

  fd = -1;
}

int Serial::send_bytes(const uint8_t *data, int len)
{
  int count = 0;
  int n;

  while (count != len)
  {
    n = write(fd, data, len - count);

    if (n <= 0)
    {
      return -1;
    }

    count += n;
  }

  return 0;
}

int Serial::read_bytes(uint8_t *data, int len, bool fill_buffer)
{
  int count = 0;
  int n;

  while (count != len)
  {
    n = read(fd, data, len - count);

    if (n == -1)
    {
      return -1;
    }

    count += n;

    if (!fill_buffer) { break; }
  }

  return count;
}

