/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Midi.h"

int main(int argc, char *argv[])
{
  Midi midi;
  bool show_bin = false;
  bool show_text = false;
  const char *filename = nullptr;

  for (int n = 1; n < argc; n++)
  {
    if (argv[n][0] == '-')
    {
      if (strcmp(argv[n], "-bin") == 0)
      {
        show_bin = true;
      }
        else
      if (strcmp(argv[n], "-text") == 0)
      {
        show_text = true;
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
    printf("Usage: %s [ -bin -text ] <file.mid>\n", argv[0]);
    exit(0);
  }

  if (midi.open_file(filename) != 0)
  {
    printf("Error: Cannot open %s\n", filename);
    exit(1);
  }

  midi.dump_header();
  midi.dump_tracks(show_bin, show_text);
  midi.close_file();

  return 0;
}

