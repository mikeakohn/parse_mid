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
  return -1;
}

int MetaData::dump_meta_event(uint8_t *data)
{
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

