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

#include "MetaEvent.h"
#include "Midi.h"

Midi::Midi() :
  tempo           { 500000 },
  division_usecs  { 0 },
  in              { nullptr }
{
  memset(time_signature, 0, sizeof(time_signature));
  memset(key_signature, 0, sizeof(key_signature));

  time_signature[0] = 4;
  time_signature[1] = 4;

  division_usecs = tempo / 100;
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

  set_division(header.division);
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

  bool tempo_set = false;
  bool time_signature_set = false;
  bool key_signature_set = false;

  for (auto &track : tracks)
  {
    if (track.has_tempo() && tempo_set == false)
    {
      set_tempo(track.get_tempo());
      tempo_set = true;
    }

    if (track.has_time_signature() && time_signature_set == false)
    {
      track.get_time_signature(time_signature);
      time_signature_set = true;
    }

    if (track.has_key_signature() && key_signature_set == false)
    {
      track.get_key_signature(key_signature);
      key_signature_set = true;
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

  printf("  starting_tempo: %d\n", tempo);
  printf("  time_signature: %d/%d  cc=%d bb=%d\n",
    time_signature[0],
    time_signature[1],
    time_signature[2],
    time_signature[3]);
  printf("   key_signature: %s %s\n\n",
    MetaEvent::get_key_signature(key_signature[0], key_signature[0] == 0),
    key_signature[0] == 0 ? "Major" : "Minor");
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

void Midi::dump_as_json()
{
  printf("{");
  header.dump_as_json();

  printf("  \"starting_tempo\": %d,\n", tempo);
  printf("  \"tracks\": [\n");

  for (int n = 0; n < header.tracks; n++)
  {
    tracks[n].dump_as_json();
    printf("%s", n == header.tracks - 1 ? "\n" : ",\n");
  }

  printf("  ]\n");
  printf("}\n");
}

int Midi::read_chunk_type(char *type)
{
  int length = fread(type, 1, 4, in);
  if (length != 4) { return -1; }

  return 0;
}

void Midi::set_division(int value)
{
  division = value;
  set_tempo(tempo); 
}

void Midi::set_tempo(int value)
{
  tempo = value;
  division_usecs = tempo / division;
}

