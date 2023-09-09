VPATH=../src:../tests

DEBUG=-DDEBUG -g
CXXFLAGS=-Wall -O3 -std=c++11 $(DEBUG)
LDFLAGS=
CC=gcc
CXX=g++
#CC=i686-w64-mingw32-gcc
#CXX=i686-w64-mingw32-g++

OBJECTS= \
  file_io.o \
  ControlChange.o \
  Header.o \
  MetaData.o \
  MetaEvent.o \
  Midi.o \
  Note.o \
  Track.o

default: $(OBJECTS)
	$(CXX) -o ../parse_mid ../src/parse_mid.cpp \
	   $(OBJECTS) \
	   $(CXXFLAGS) \
	   $(LDFLAGS)

lib: $(OBJECTS)
	$(CXX) -o ../libparsemid.so ../src/parsemid.cpp -fPIC -shared \
	   $(OBJECTS) \
	   $(CXXFLAGS) \
	   $(LDFLAGS)

%.o: %.cpp %.h
	$(CXX) -c $< -o $*.o $(CXXFLAGS)

