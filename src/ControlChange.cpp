/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "ControlChange.h"

int ControlChange::dump(uint8_t *data)
{
  int c = data[0];
  int v = data[0];

  switch (c)
  {
    case 122:
      printf("Local Control %s\n", v == 0 ? "Off" : "On");
      break;
    case 123:
      printf("All Notes Off\n");
      break;
    case 124:
      printf("Omni Mode Off\n");
      break;
    case 125:
      printf("Omni Mode On\n");
      break;
    case 126:
      printf("Mono Mode On (Channel Count: %d)\n", v);
      break;
    case 127:
      printf("Poly Mode On\n");
      break;
    default:
      printf("Unknown\n");
      break;
  }

  return 2;
}

