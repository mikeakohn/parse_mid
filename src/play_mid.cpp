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
#include <unistd.h>

#include <chrono>
#include <vector>

#include "Midi.h"
#include "Track.h"

int main(int argc, char *argv[])
{
  Midi midi;
  const char *filename = nullptr;
  int channel = -1;

  for (int n = 1; n < argc; n++)
  {
    if (argv[n][0] == '-')
    {
      if (strcmp(argv[n], "-channel") == 0)
      {
        channel = atoi(argv[++n]);
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

  // tempo is microseconds per quarter note.
  // divisions is divisions per quarter note.
  int track_count = midi.get_track_count();
  int divisions   = midi.get_division_count();
  int tempo       = midi.get_tempo();
 
  printf("   tracks: %d\n", track_count);
  printf("divisions: %d\n", divisions);
  printf("    tempo: %d\n", tempo);

  std::vector<Track::iterator> track_data;
  std::vector<int> ticks;

  int count = 0;

  for (int n = 0; n < track_count; n++)
  {
    if (channel != -1 && channel != n) { continue; }

    Track &track = midi.get_track(n);
    track_data.push_back(track.begin());
    ticks.push_back(0);

    count += 1;
  }

  int used;
  int delay = tempo / divisions;

  while (true)
  {
    used = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int n = 0; n < count; n++)
    {
      if (!track_data[n].has_more_data()) { continue; }

      used += 1;

      // If anything is time of 0, run it and grab the next.
      while (track_data[n].has_more_data())
      {
        if (ticks[n] == 0)
        {
          track_data[n]++;

          if (track_data[n].is_note == false) { continue; }

          const int vlength = track_data[n].vlength;

          printf("%d: %02x %02x %02x (%d)\n",
            n,
            track_data[n].raw_data.data[0],
            track_data[n].raw_data.data[1],
            track_data[n].raw_data.data[2],
            vlength);

          if (vlength == 0) { continue; }

          ticks[n] = vlength;
        }

        ticks[n] -= 1;

        break;
      }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto delta = end - start;

    auto microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(delta).count();

    int current = delay - microseconds;

    if (current > 0) { usleep(current); }

    if (used == 0) { break; }
  }

#if 0
  Track &track = midi.get_track(0);

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

