VPATH=../src:../tests

DEBUG=-DDEBUG -g
#CXXFLAGS=-Wall -O3 -std=c++11 -fPIC $(DEBUG)
CXXFLAGS=-Wall -std=c++11 -fPIC $(DEBUG)
LDFLAGS=
CC=gcc
CXX=g++
#CC=i686-w64-mingw32-gcc
#CXX=i686-w64-mingw32-g++

OBJECTS= \
  file_io.o \
  ControlChange.o \
  Header.o \
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
	$(CXX) -o ../libparse_mid.so -fPIC -shared \
	   $(OBJECTS) \
	   $(CXXFLAGS) \
	   $(LDFLAGS)

%.o: %.cpp %.h
	$(CXX) -c $< -o $*.o $(CXXFLAGS)

