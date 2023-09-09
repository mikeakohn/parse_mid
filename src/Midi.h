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

private:
  int read_chunk_type(char *type);

  Header header;

  FILE *in;
  std::vector<Track> tracks;
};

#endif

