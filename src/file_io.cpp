/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "file_io.h"

int read_string(FILE *in, char *s, int n)
{
  int t,ch;

  for(t = 0; t < n; t++)
  {
    ch = getc(in);
    if (ch == EOF) { return -1; }

    s[t] = ch;
  }

  s[t] = 0;

  return 0;
}

int read_int32(FILE *in)
{
  int c;

  c = getc(in);
  c = (c << 8) | getc(in);
  c = (c << 8) | getc(in);
  c = (c << 8) | getc(in);

  return c;
}

int read_int16(FILE *in)
{
  int c;

  c = getc(in);
  c = (c << 8) | getc(in);

  return c;
}

int get_vlength(uint8_t *data, int &count)
{
  int value = 0;

  count = 0;

  while (true)
  {
    int ch = data[count++];

    value = (value << 7) | (ch & 0x7f);
    if ((ch & 128) == 0) { break; }
  }

  return value;
}

