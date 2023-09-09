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

#include "Midi.h"

Midi::Midi() :
  in     { nullptr }
{
}

Midi::~Midi()
{
  close_file();
}

int Midi::open_file(const char *filename)
{
  in = fopen(filename, "rb");

  if (in == NULL)
  {
    return -1;
  }

  char type[4];
  int t = read_chunk_type(type);

  if (t < 0 || memcmp(type, "MThd", 4) != 0)
  {
    printf("Error: Missing 'MThd' chunk.\n");
    return -2;
  }

  header.read(in);

  tracks.reserve(header.tracks);

  for (int n = 0; n < header.tracks; n++) { tracks.emplace_back(); }

  int count = 0;

  while (true)
  {
    int t = read_chunk_type(type);
    if (t < 0) { break; }

    if (memcmp(type, "MTrk", 4) == 0)
    {
      if (count >= header.tracks)
      {
        printf("Error: Too many tracks.\n");
        return -5;
      }

      tracks[count].read(in);

      count++;
    }
      else
    {
      printf("Error: Unknown chunk '%.4s'\n", type);
      return -4;
    }

  }

  return 0;
}

void Midi::close_file()
{
  if (in != nullptr) { fclose(in); }
  in = nullptr;
}

void Midi::dump_header()
{
  header.dump();
}

void Midi::dump_tracks(bool show_bin, bool show_text)
{
  for (int n = 0; n < header.tracks; n++)
  {
    printf("Track: %d\n", n);
    tracks[n].dump_header();

    if (show_bin)  { tracks[n].dump_data_as_bin(); }
    if (show_text) { tracks[n].dump_data_as_text(); }
  }
}

int Midi::read_chunk_type(char *type)
{
  int length = fread(type, 1, 4, in);
  if (length != 4) { return -1; }

  return 0;
}

