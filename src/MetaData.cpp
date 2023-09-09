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
#include "MetaData.h"

MetaData::MetaData() :
  sequence_number { 0 },
  tempo           { 500000 },
  division_usecs  { 0 }
{
  memset(smpte_offset, 0, sizeof(smpte_offset));
  memset(time_signature, 0, sizeof(time_signature));
  memset(key_signature, 0, sizeof(key_signature));

  time_signature[0] = 4;
  time_signature[1] = 4;

  division_usecs = tempo / 100;
}

MetaData::~MetaData()
{
}

void MetaData::set_division(int value)
{
  division = value;
  set_tempo(tempo); 
}

int MetaData::process(uint8_t *data, int &length)
{
  return 0;
}

int MetaData::get_meta_event(uint8_t *data, int &length)
{
  length = 0;

  switch (data[0])
  {
    case META_SEQUENCE_NUMBER:
      length = data[1];
      return 0x00;
    case META_TEXT_EVENT:
      return 0x01;
    case META_COPYRIGHT:
      return 0x02;
    case META_TRACK_NAME:
      return 0x03;
    case META_INSTRUMENT_NAME:
      return 0x04;
    case META_LYRIC_TEXT:
      return 0x05;
    case META_MARKER_TEXT:
      return 0x06;
    case META_CUE_POINT:
      return 0x07;
    case META_CHANNEL_PREFIX:
      return 0x20;
    case META_END_OF_TRACK:
      return 0x2f;
    case META_TEMPO:
      return 0x51;
    case META_SMPTE_OFFSET:
      return 0x54;
    case META_TIME_SIGNATURE:
      return 0x58;
    case META_KEY_SIGNATURE:
      return 0x59;
    case META_SEQUENCER_SPECIFIC:
      return 0x7f;
  }

  return -1;
}

int MetaData::dump_meta_event(uint8_t *data)
{
  int length;
  int count;
  int n;

  switch (data[0])
  {
    case META_SEQUENCE_NUMBER:
      if (data[1] == 0)
      {
        printf("MetaEvent sequence_number:\n");
      }
        else
      {
        printf("MetaEvent sequence_number: %02x %02x\n", data[2], data[3]);
      }

      break;
    case META_TEXT_EVENT:
      printf("MetaEvent text: ");

      length = get_vlength(data + 1, count);

      for (n = 0; n < length; n++)
      {
        printf("%c", data[1 + count + n]);
      }

      printf("\n");
      break;
    case META_COPYRIGHT:
      return 0x02;
    case META_TRACK_NAME:
      return 0x03;
    case META_INSTRUMENT_NAME:
      return 0x04;
    case META_LYRIC_TEXT:
      return 0x05;
    case META_MARKER_TEXT:
      return 0x06;
    case META_CUE_POINT:
      return 0x07;
    case META_CHANNEL_PREFIX:
      return 0x20;
    case META_END_OF_TRACK:
      return 0x2f;
    case META_TEMPO:
      return 0x51;
    case META_SMPTE_OFFSET:
      return 0x54;
    case META_TIME_SIGNATURE:
      return 0x58;
    case META_KEY_SIGNATURE:
      return 0x59;
    case META_SEQUENCER_SPECIFIC:
      return 0x7f;
  }

  return 0;
}

void MetaData::dump()
{
  printf(" -- Song Meta Data --\n");
  printf("  sequence_number: %d\n", sequence_number);
  printf("            tempo: %d\n", tempo);
  printf("     smpte_offset: %02x %02x %02x %02x %02x\n",
    smpte_offset[0],
    smpte_offset[1],
    smpte_offset[2],
    smpte_offset[3],
    smpte_offset[4]);
  printf("   time_signautre: %d/%d clock=%d num_notated_32_notes=%d\n",
    time_signature[0],
    time_signature[1],
    time_signature[2],
    time_signature[3]);
  printf("    key_signature: %d\n", tempo);
  printf("   division_usecs: %d\n", division_usecs);
}

void MetaData::set_tempo(int value)
{
  tempo = value;
  division_usecs = tempo / division;
}

