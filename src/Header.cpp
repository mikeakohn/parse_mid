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
#include "Header.h"

Header::Header() :
  length   { 0 },
  format   { 0 },
  tracks   { 0 },
  division { 0 }
{
}

Header::~Header()
{
}

int Header::read(FILE *in)
{
  length = read_int32(in);
  format = read_int16(in);
  tracks = read_int16(in);
  division = read_int16(in);

  return 0;
}

void Header::dump()
{
  printf(" -- MThd ---\n");
  printf("   length: %d\n", length);
  printf("   format: %d\n", format);
  printf("   tracks: %d\n", tracks);
  printf(" division: %d\n", division);
  printf("\n");
}

void Header::dump_as_json()
{
  printf("  \"header\": {\n");
  printf("     \"length\": %d,\n", length);
  printf("     \"format\": %d,\n", format);
  printf("     \"tracks\": %d,\n", tracks);
  printf("     \"division\": %d\n", division);
  printf("  },\n");
}

