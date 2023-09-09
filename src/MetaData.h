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

  enum
  {
    META_SEQUENCE_NUMBER    = 0x00,
    META_TEXT_EVENT         = 0x01,
    META_COPYRIGHT          = 0x02,
    META_TRACK_NAME         = 0x03,
    META_INSTRUMENT_NAME    = 0x04,
    META_LYRIC_TEXT         = 0x05,
    META_MARKER_TEXT        = 0x06,
    META_CUE_POINT          = 0x07,
    META_CHANNEL_PREFIX     = 0x20,
    META_END_OF_TRACK       = 0x2f,
    META_TEMPO              = 0x51,
    META_SMPTE_OFFSET       = 0x54,
    META_TIME_SIGNATURE     = 0x58,
    META_KEY_SIGNATURE      = 0x59,
    META_SEQUENCER_SPECIFIC = 0x7f,
  };

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

