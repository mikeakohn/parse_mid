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

#include "Note.h"

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

  uint8_t *get_track_data() { return data; }

  void dump_header();

  // FIXME: The new iterator could probably make these functions simpler.
  void dump_data_as_bin();
  void dump_data_as_text();
  void dump_as_json();

  class iterator
  {
  public:
    iterator(uint8_t *data, int length) :
       is_note { 0 },
       ptr     { 0 },
       length  { length },
       vlength { 0 },
       type    { 0 },
       data    { data }
    {
    }

    iterator(int ptr) : ptr { ptr }
    {
    }

    bool next();

    iterator operator ++();
    iterator operator ++(int inc);
    iterator operator *() { return *this; }
    bool operator ==(const iterator &rhs) { return ptr >= rhs.ptr; }
    bool operator !=(const iterator &rhs) { return ptr < rhs.ptr; }

    bool is_note;
    int ptr;
    int length;
    int vlength;
    int type;

    Note note;

    struct RawData
    {
      RawData() : data { nullptr }, count { 0 } { }

      uint8_t *data;
      int count;
    } raw_data;

  private:
    uint8_t *data;
  };

  iterator begin()
  {
    return iterator(data, length);
  }

  iterator end()
  {
    //return iterator(data + length);
    return iterator(length);
  }

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

