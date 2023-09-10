/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _META_DATA_H
#define _META_DATA_H

#include <stdint.h>

class MetaData
{
public:
  MetaData();
  ~MetaData();

  void set_division(int value);
  int process(uint8_t *data, int &length);
  int get_meta_event(uint8_t *data, int &length);
  int dump_meta_event(uint8_t *data);
  void dump();

private:
  void set_tempo(int value);

  int sequence_number;
  int tempo;
  char smpte_offset[5];
  char time_signature[4];
  char key_signature[3];
  useconds_t division_usecs;

  int division;
};

#endif

