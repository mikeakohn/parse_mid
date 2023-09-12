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
#include "ControlChange.h"
#include "MetaEvent.h"
#include "Note.h"
#include "Track.h"

Track::Track() :
  length                 { 0  },
  file_offset            { 0  },
  tempo_offset           { -1 },
  time_signature_offset  { -1 },
  key_signature_offset   { -1 },
  data                   { nullptr },
  ptr                    { 0  }
{
}

Track::~Track()
{
  free(data);
}

int Track::read(FILE *in)
{
  file_offset = (int)ftell(in) - 4;
  length = read_int32(in);
  data = (uint8_t *)malloc(length);
  int n = fread(data, 1, length, in);

  if (n != length) { return -1; }

  process();

  return 0;
}

int Track::get_tempo()
{
  if (tempo_offset == -1) { return -1; }

  return
    (data[tempo_offset + 2] << 16) |
    (data[tempo_offset + 3] << 8) |
    (data[tempo_offset + 4]);
}

void Track::get_time_signature(uint8_t *value)
{
  if (time_signature_offset == -1)
  {
    memset(value, 0, 4);
    return;
  }

  memcpy(value, data + time_signature_offset + 2, 4);
}

void Track::get_key_signature(uint8_t *value)
{
  if (key_signature_offset == -1)
  {
    memset(value, 0, 2);
    return;
  }

  memcpy(value, data + key_signature_offset + 2, 2);
}

void Track::dump_header()
{
  printf(" -- MThd ---\n");
  printf("            name: %s\n", name.c_str());
  printf("          length: %d\n", length);
  printf("     file_offset: %d\n", file_offset);
  //printf("  running_status: %d\n", running_status);
  //printf("        division: %d\n", division);
  //printf("           state: %d\n", state);
  printf("             ptr: %d\n", ptr);
  printf("            data: %p\n", data);
}

void Track::dump_data_as_bin()
{
  if (data == nullptr) { return; }

  for (int n = 0; n < length; n++)
  {
    printf(" %02x,", data[n]);
    if (((n + 1 ) % 16) == 0) { printf("\n"); }
  }

  printf("\n\n");
}

void Track::dump_data_as_text()
{
  if (data == nullptr) { return; }

  int ptr = 0;

  while (ptr < length)
  {
    Note note;
    int count;
    int vlength = get_vlength(data + ptr, count);

    ptr += count;

    int type = data[ptr++];
    int upper = type & 0xf0;

    if (type == 0xff)
    {
      printf("  MetaEvent: vlength=%d", vlength);
      int count = MetaEvent::dump(data + ptr);

      if (count == -1)
      {
        printf("Error: Unknown meta event %02x\n", data[ptr - 1]);
        return;
      }

      ptr += count;
    }
      else
    if (type == 0xf0 || type == 0xf7)
    {
      printf("  system_exclusive: [\n");
      for (int n = 0; n < vlength; n++)
      {
        printf(" %02x,", data[ptr++]);
        if (((n + 1 ) % 16) == 0) { printf("\n"); }
      }
      printf("  ]\n\n");
    }
      else
    if (upper == 0xa0)
    {
      printf("   PolyphonicKeyPressure: vlength=%d channel=%d value=%d\n",
        vlength,
        type & 0xf,
        data[ptr++]);
    }
      else
    if (upper == 0xb0)
    {
      printf("   ControlChange: vlength=%d channel=%d ", vlength, type & 0xf);
      ptr += ControlChange::dump(data);
    }
      else
    if (upper == 0xc0)
    {
      printf("   ProgramChange: vlength=%d channel=%d program_number=%d\n",
        vlength,
        type & 0xf,
        data[ptr++]);
    }
      else
    if (upper == 0xd0)
    {
      printf("   ChannelPressure: vlength=%d channel=%d value=%d\n",
        vlength,
        type & 0xf,
        data[ptr++]);
    }
      else
    if (upper == 0xe0)
    {
      printf("   PitchWheelChange: vlength=%d channel=%d value=%d\n",
        vlength,
        type & 0xf,
        data[0] | (data[1] << 7));
      ptr += 2;
    }
      else
    {
      ptr += note.set(data + ptr - 1, vlength);
      note.dump();
    }
  }
}

void Track::dump_as_json()
{
  printf("    {\n");
  printf("      \"name\": \"%s\",\n", name.c_str());
  printf("      \"length\": %d,\n", length);
  printf("      \"data\": [\n");

  if (data == nullptr)
  {
    printf("      ]\n");
    return;
  }

  int ptr = 0;

  while (ptr < length)
  {
    Note note;
    int count;
    int vlength = get_vlength(data + ptr, count);

    ptr += count;

    int type = data[ptr++];
    int upper = type & 0xf0;

    if (type == 0xff)
    {
      //printf("  MetaEvent: vlength=%d", vlength);
      int count = MetaEvent::dump_as_json(data + ptr, vlength);

      if (count == -1)
      {
        printf("Error: Unknown meta event %02x\n", data[ptr - 1]);
        return;
      }

      ptr += count;
    }
      else
    if (type == 0xf0 || type == 0xf7)
    {
      printf("  system_exclusive: [\n");
      for (int n = 0; n < vlength; n++)
      {
        printf(" %02x,", data[ptr++]);
        if (((n + 1 ) % 16) == 0) { printf("\n"); }
      }
      printf("  ]\n\n");
    }
      else
    if (upper == 0xa0)
    {
      printf("        { \"type\": \"polyphonic_key_pressure\": vlength=%d, channel=%d, value=%d }",
        vlength,
        type & 0xf,
        data[ptr++]);
    }
      else
    if (upper == 0xb0)
    {
      printf("        { \"type\": \"control_change\", vlength=%d, channel=%d }",
        vlength, type & 0xf);
      ptr += 2;
    }
      else
    if (upper == 0xc0)
    {
      printf("        { \"type\": \"program_change\", vlength=%d, channel=%d, program_number=%d }",
        vlength,
        type & 0xf,
        data[ptr++]);
    }
      else
    if (upper == 0xd0)
    {
      printf("        { \"type\": \"channel_pressure\", vlength=%d, channel=%d, value=%d }",
        vlength,
        type & 0xf,
        data[ptr++]);
    }
      else
    if (upper == 0xe0)
    {
      printf("        { \"type\": \"pitch_wheel_change\", vlength=%d, channel=%d, value=%d }",
        vlength,
        type & 0xf,
        data[0] | (data[1] << 7));
      ptr += 2;
    }
      else
    {
      ptr += note.set(data + ptr - 1, vlength);
      note.dump_as_json();
    }

    printf("%s", ptr != length ? ",\n" : "\n");

  }

  printf("      ]\n");
  printf("    }");
}

void Track::process()
{
  if (data == nullptr) { return; }

  int ptr = 0;

  while (ptr < length)
  {
    Note note;
    int count;
    int vlength = get_vlength(data + ptr, count);

    ptr += count;

    int type = data[ptr++];
    int upper = type & 0xf0;

    if (type == 0xff)
    {
      int count = MetaEvent::get_length(data + ptr);
      if (count == -1) { return; }

      switch (data[ptr])
      {
        case MetaEvent::TYPE_TRACK_NAME:
          vlength = get_vlength(data + ptr + 1, count);
          name = std::string((char *)(data + ptr + 1 + count), vlength);
          break;
        case MetaEvent::TYPE_TEMPO:
          tempo_offset = ptr;
          break;
        case MetaEvent::TYPE_TIME_SIGNATURE:
          time_signature_offset = ptr;
          break;
        case MetaEvent::TYPE_KEY_SIGNATURE:
          key_signature_offset = ptr;
          break;
        default:
          break;
      }

      ptr += count;
    }
      else
    if (type == 0xf0 || type == 0xf7)
    {
      ptr += vlength;
    }
      else
    if (upper == 0xa0)
    {
      ptr++;
    }
      else
    if (upper == 0xb0)
    {
      ptr += 2;
    }
      else
    if (upper == 0xc0)
    {
      ptr++;
    }
      else
    if (upper == 0xd0)
    {
      ptr++;
    }
      else
    if (upper == 0xe0)
    {
      ptr += 2;
    }
      else
    {
      ptr += 2;
    }
  }
}

