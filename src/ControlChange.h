/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _CONTROL_CHANGE_H
#define _CONTROL_CHANGE_H

#include <stdint.h>

class ControlChange
{
public:

  static int dump(uint8_t *data);

private:
  ControlChange()  { }
  ~ControlChange() { }
};

#endif

