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
#include "Serial.h"
#include "Track.h"

int main(int argc, char *argv[])
{
  Midi midi;
  Serial serial;
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
      if (strcmp(argv[n], "-port") == 0)
      {
        const char *port = argv[++n];
        int result = serial.open_port(port, 31250);

        if (result != 0)
        {
          printf("Error: Could not open serial port %s.\n", port);
          exit(-1);
        }

        printf("Opened serial port %s.\n", port);
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

  // Extra commands for SAM2695.
  if (serial.is_open())
  {
    uint8_t set_volume[] =
    {
      0xb0, 0x63, 0x37,
      0xb0, 0x62, 0x07,
      0xb0, 0x06, 0x7f
    };

    serial.send_bytes(set_volume, 9);

#if 0
    uint8_t auto_test[] =
    {
      0xb0, 0x63, 0x37,
      0xb0, 0x62, 0x51,
      0xb0, 0x06, 0x23
    };

    serial.send_bytes(auto_test, 9);
#endif
  }

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

          if (serial.is_open())
          {
#if 0
            uint8_t data[4];
            data[0] = track_data[n].raw_data.data[0];
            data[1] = track_data[n].raw_data.data[1];
            data[2] = track_data[n].raw_data.data[2];
            data[3] = 0;

            serial.send_bytes(data, 4);
#endif

            serial.send_bytes(track_data[n].raw_data.data, 3);
          }

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

  if (serial.is_open())
  {
    serial.close_port();
  }

  midi.close_file();

  return 0;
}

