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
  bool as_json = false;
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
      if (strcmp(argv[n], "-json") == 0)
      {
        as_json = true;
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
      "    -bin         (Show tracks as hexdump)\n"
      "    -text        (Show tracks as text)\n"
      "    -json        (Show output in JSON format)\n");
    exit(0);
  }

  if (midi.open_file(filename) != 0)
  {
    printf("Error: Cannot open %s\n", filename);
    exit(1);
  }


  if (as_json)
  {
    midi.dump_as_json();
  }
    else
  {
    midi.dump_header();
    midi.dump_tracks(show_bin, show_text);
  }

  midi.close_file();

  return 0;
}

