/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _META_EVENT_H
#define _META_EVENT_H

#include <stdint.h>

class MetaEvent
{
public:

  static int dump(uint8_t *data);
  static int get_length(uint8_t *data);

  enum
  {
    TYPE_SEQUENCE_NUMBER    = 0x00,
    TYPE_TEXT_EVENT         = 0x01,
    TYPE_COPYRIGHT          = 0x02,
    TYPE_TRACK_NAME         = 0x03,
    TYPE_INSTRUMENT_NAME    = 0x04,
    TYPE_LYRIC_TEXT         = 0x05,
    TYPE_MARKER_TEXT        = 0x06,
    TYPE_CUE_POINT          = 0x07,
    TYPE_CHANNEL_PREFIX     = 0x20,
    TYPE_END_OF_TRACK       = 0x2f,
    TYPE_TEMPO              = 0x51,
    TYPE_SMPTE_OFFSET       = 0x54,
    TYPE_TIME_SIGNATURE     = 0x58,
    TYPE_KEY_SIGNATURE      = 0x59,
    TYPE_SEQUENCER_SPECIFIC = 0x7f,
  };

private:
  MetaEvent()  { }
  ~MetaEvent() { }

  static int text_length(uint8_t *data);
  static int text_event(uint8_t *data, const char *label);
  static int print_string(uint8_t *data);
};

#endif

