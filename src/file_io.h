/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _FILEIO_H
#define _FILEIO_H

int read_string(FILE *in, char *s, int n);
int read_int32(FILE *in);
int read_int16(FILE *in);

int get_vlength(uint8_t *data, int &count);

#endif

