/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "file_io.h"
#include "Note.h"

Note::Note() :
  length   { 0 },
  is_on    { false },
  channel  { 0 },
  tone     { 0 },
  velocity { 0 }
{
}

Note::~Note()
{
}

int Note::set(uint8_t *data, int length)
{
  this->length = length;

  is_on = (data[0] & 0xf0) == 0x90;
  channel = data[0] & 0x0f;
  tone = data[1];
  velocity = data[2];

  return 2;
}

void Note::dump()
{
  printf("   note: { length=%d, is_on=%s, channel=%d, tone=%d velocity=%d }\n",
    length,
    is_on ? "true" : "false",
    channel,
    tone,
    velocity);
}

