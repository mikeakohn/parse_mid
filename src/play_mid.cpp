/*

  parsemidi - .mid file parsing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <vector>

#include "Midi.h"
#include "Track.h"

int main(int argc, char *argv[])
{
  Midi midi;
  const char *filename = nullptr;

  for (int n = 1; n < argc; n++)
  {
    if (argv[n][0] == '-')
    {
      if (strcmp(argv[n], "-json") == 0)
      {
      }
        else
      {
        printf("Error: Unknown option %s.\n", argv[n]);
        exit(1);
      }
    }
      else
    {
      filename = argv[n];
    }
  }

  if (filename == nullptr)
  {
    printf("Usage: %s [ options ] <file.mid>\n", argv[0]);
    printf(
      "    -json        (Show output in JSON format)\n");
    exit(0);
  }

  if (midi.open_file(filename) != 0)
  {
    printf("Error: Cannot open %s\n", filename);
    exit(1);
  }

  int track_count = midi.get_track_count();
  int divisions   = midi.get_division_count();
  int tempo       = midi.get_tempo();
 
  printf("   tracks: %d\n", track_count);
  printf("divisions: %d\n", divisions);
  printf("    tempo: %d\n", tempo);

  std::vector<Track::iterator> track_data;

  for (int n = 0; n < track_count; n++)
  {
    Track &track = midi.get_track(n);
    track_data.push_back(track.begin());
  }

#if 0
  track_data[0]++;

  //if (track_data[0] != track.end())
  {
    printf("%d\n", track_data[0].is_note);
  }

  Track &track = midi.get_track(2);

  for (auto it : track)
  {
    if (it.is_note)
    {
      printf("vtime: %d   %02x %02x %02x\n",
        it.vlength,
        it.raw_data.data[0],
        it.raw_data.data[1],
        it.raw_data.data[2]);
    }
  }
#endif

  midi.close_file();

  return 0;
}

