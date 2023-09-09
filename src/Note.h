/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _NOTE_H
#define _NOTE_H

#include <stdint.h>

class Note
{
public:
  Note();
  ~Note();

  int set(uint8_t *data, int length);
  void dump();

private:
  int length;
  bool is_on;
  int channel;
  int tone;
  int velocity;
};

#endif

