/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _HEADER_H
#define _HEADER_H

#include <stdint.h>

class Header
{
public:
  Header();
  ~Header();

  int read(FILE *in);
  void dump();
  void dump_as_json();

  int length;
  int format;
  int tracks;
  int division;
};

#endif

