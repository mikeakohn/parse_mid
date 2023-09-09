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

#include "file_io.h"
#include "MetaEvent.h"

int MetaEvent::dump(uint8_t *data)
{
  int length;
  int count;
  int bytes = 1;
  int n;

  switch (data[0])
  {
    case TYPE_SEQUENCE_NUMBER:
      length = get_vlength(data + 1, count);
      data += 1 + count;

      printf(" sequence_number:");
      for (n = 0; n < length; n++) { printf(" %02x", data[n]); }
      printf("\n");

      bytes += count + length;
      break;
    case TYPE_TEXT_EVENT:
      bytes += text_event(data, "text");
      break;
    case TYPE_COPYRIGHT:
      bytes += text_event(data, "copyright");
      break;
    case TYPE_TRACK_NAME:
      bytes += text_event(data, "track_name");
      break;
    case TYPE_INSTRUMENT_NAME:
      bytes += text_event(data, "instrument_name");
      break;
    case TYPE_LYRIC_TEXT:
      bytes += text_event(data, "lyric_text");
      break;
    case TYPE_MARKER_TEXT:
      bytes += text_event(data, "marker_text");
      break;
    case TYPE_CUE_POINT:
      bytes += text_event(data, "cue_point");
      break;
    case TYPE_CHANNEL_PREFIX:
      printf(" channel_prefix: %02x\n", data[2]);
      bytes += 3;
      break;
    case TYPE_END_OF_TRACK:
      printf(" end_of_track\n");
      bytes += 2;
      break;
    case TYPE_TEMPO:
      printf(" tempo: %d\n",
        (data[2] << 16) |
        (data[3] << 8) |
         data[4]);
      bytes += 4;
      break;
    case TYPE_SMPTE_OFFSET:
      printf( " smpte_offset: hr=%02x mn=%02x se=%02x fr=%02x ff=%02x\n",
        data[2],
        data[3],
        data[4],
        data[5],
        data[6]);
      bytes += 6;
      break;
    case TYPE_TIME_SIGNATURE:
      printf(" time_signature: %d/%d cc=%02x bb=%02x\n",
        data[2],
        data[3],
        data[4],
        data[5]);
      bytes += 5;
      break;
    case TYPE_KEY_SIGNATURE:
      printf(" key_signature: sf=%02x mi=%02x\n", data[2], data[3]);
      bytes += 3;
      break;
    case TYPE_SEQUENCER_SPECIFIC:
      length = get_vlength(data + 1, count);
      data += 1 + count;

      printf(" sequencer_specific:");
      for (n = 0; n < length; n++) { printf(" %02x", data[n]); }
      printf("\n");

      bytes += count + length;
      break;
    default:
      printf("Error: Unknown MetaEvent %02x\n", data[0]);
      return -1;
  }

  return bytes;
}

int MetaEvent::get_length(uint8_t *data)
{
  int length;
  int count;
  int bytes = 1;

  switch (data[0])
  {
    case TYPE_SEQUENCE_NUMBER:
      length = get_vlength(data + 1, count);
      bytes += count + length;
      break;
    case TYPE_TEXT_EVENT:
      bytes += text_length(data);
      break;
    case TYPE_COPYRIGHT:
      bytes += text_length(data);
      break;
    case TYPE_TRACK_NAME:
      bytes += text_length(data);
      break;
    case TYPE_INSTRUMENT_NAME:
      bytes += text_length(data);
      break;
    case TYPE_LYRIC_TEXT:
      bytes += text_length(data);
      break;
    case TYPE_MARKER_TEXT:
      bytes += text_length(data);
      break;
    case TYPE_CUE_POINT:
      bytes += text_length(data);
      break;
    case TYPE_CHANNEL_PREFIX:
      bytes += 3;
      break;
    case TYPE_END_OF_TRACK:
      bytes += 2;
      break;
    case TYPE_TEMPO:
      bytes += 4;
      break;
    case TYPE_SMPTE_OFFSET:
      bytes += 6;
      break;
    case TYPE_TIME_SIGNATURE:
      bytes += 5;
      break;
    case TYPE_KEY_SIGNATURE:
      bytes += 3;
      break;
    case TYPE_SEQUENCER_SPECIFIC:
      length = get_vlength(data + 1, count);
      bytes += count + length;
      break;
    default:
      printf("Error: Unknown MetaEvent %02x\n", data[0]);
      return -1;
  }

  return bytes;
}

int MetaEvent::text_length(uint8_t *data)
{
  int count;
  int length = get_vlength(data, count);
  return count + length;
}

int MetaEvent::text_event(uint8_t *data, const char *label)
{
  printf(" %s: ", label);
  int count = print_string(data + 1);
  printf("\n");
  return count;
}

int MetaEvent::print_string(uint8_t *data)
{
  int count;
  int length = get_vlength(data, count);

  char text[length + 1];
  memset(text, 0, sizeof(text));
  memcpy(text, data + count, length);
  printf("%s", text);

  return count + length;
}

