/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>
#include <termios.h>

class Serial
{
public:
  Serial();
  ~Serial();

  int open_port(const char *device, int baud);
  void close_port();

  int send_bytes(const uint8_t *data, int len);
  int read_bytes(uint8_t *data, int len, bool fill_buffer);

  bool is_open() { return fd != -1; }

private:
  int fd;
  struct termios oldtio;
};

#endif

