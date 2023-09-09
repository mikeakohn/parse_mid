/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _TRACK_H
#define _TRACK_H

#include <stdint.h>
#include <string>

class Track
{
public:
  Track();
  ~Track();

  int read(FILE *in);
  void dump_header();
  void dump_data_as_bin();
  void dump_data_as_text();

private:
  void process();

  std::string name;
  int length;
  //int running_status;
  //int division;
  //int state;
  uint8_t *data;
  int ptr;
};

#endif

