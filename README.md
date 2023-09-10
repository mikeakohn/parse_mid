parse_mid
=========

Over the years I've done a few projects that required .mid file reading
(and sometimes writing). Since I'm again working on a project where I'd
like to read in a .mid file, I decided to clean up the code of one of
the projects by breaking it up into more generic modules and orchestrating
it all through a main module that could be turned into a .so file.

The main test program here is parse_mid. Running it on a .mid file
gives out put that looks like:

    -- MThd ---
               name: Electric Piano
             length: 6307
                ptr: 0
               data: 0x562cb3397040
     MetaEvent: vlength=0 track_name: Electric Piano
     ControlChange: vlength=0 channel=0 Unknown
     ProgramChange: vlength=0 channel=0 program_number=5
     MetaEvent: vlength=0 key_signature: sf=00 mi=00
      note: { len=3440 On  chan=0  velocity=57  tone=[ 71 B4 493.880 ] }
      note: { len=8    On  chan=0  velocity=46  tone=[ 67 G4 392.000 ] }
      note: { len=184  Off chan=0  velocity=0   tone=[ 67 G4 392.000 ] }
      note: { len=4    On  chan=0  velocity=65  tone=[ 62 D4 293.660 ] }
      note: { len=12   Off chan=0  velocity=0   tone=[ 71 B4 493.880 ] }

