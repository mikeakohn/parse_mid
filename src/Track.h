/*

  parsemidi - .mid file parsing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This program falls under the BSD 3 license.

*/

#ifndef _TRACK_H
#define _TRACK_H

#include <stdint.h>
#include <string>

class Track
{
public:
  Track();
  ~Track();

  int read(FILE *in);

  int get_tempo();
  void get_time_signature(uint8_t *value);
  void get_key_signature(uint8_t *value);

  bool has_tempo()          { return tempo_offset != -1;          }
  bool has_time_signature() { return time_signature_offset != -1; }
  bool has_key_signature()  { return key_signature_offset != -1;  }

  void dump_header();
  void dump_data_as_bin();
  void dump_data_as_text();
  void dump_as_json();

private:
  void process();

  std::string name;
  int length;
  int file_offset;

  int tempo_offset;
  int time_signature_offset;
  int key_signature_offset;

  uint8_t *data;
  int ptr;
};

#endif

