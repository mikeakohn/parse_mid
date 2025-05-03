/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _MIDI_H
#define _MIDI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <vector>

#include "Header.h"
#include "Track.h"

class Midi
{
public:
  Midi();
  ~Midi();

  int open_file(const char *filename);
  void close_file();

  void dump_header();
  void dump_tracks(bool show_bin, bool show_text);
  void dump_as_json();

  int get_track_count()    { return header.tracks; }
  int get_division_count() { return header.division; }
  int get_tempo()          { return tempo; }

  uint8_t *get_track_data(int index)
  {
    if (index >= (int)tracks.size()) { return nullptr; }

    return tracks[index].get_track_data();
  }

  Track &get_track(int index)
  {
    if (index >= (int)tracks.size()) { index = 0; }

    return tracks[index];
  }

private:
  int read_chunk_type(char *type);
  void set_division(int value);
  void set_tempo(int value);

  Header header;

  // These come from MetaEvents in different tracks. Each track can
  // have its own value.
  int tempo;
  uint8_t time_signature[4];
  uint8_t key_signature[2];
  useconds_t division_usecs;

  int division;

  FILE *in;
  std::vector<Track> tracks;
};

#endif

