LINUX_OBJS = ../Objects/Linux/*.o* ../Dummy/AIDummy.o.Linux64
MAC_OBJS = ../Objects/Mac/*.o*  ../Dummy/AIDummy.o.MacOS

# Config
OPTIMIZE = 2 # Optimization level (0 to 3)
DEBUG    = 0 # Compile for debugging (0 or 1)
PROFILE  = 0 # Compile for profile (0 or 1)
32BITS   = 0 # Produce 32 bits objects on 64 bits systems (0 or 1)


# Do not edit past this line
############################

# The following two lines will detect all your players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ_LINUX = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) $(LINUX_OBJS) $(DUMMY_OBJ)
PLAYERS_OBJ_MAC = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) $(MAC_OBJS) $(DUMMY_OBJ)

# Flags

ifeq ($(strip $(PROFILE)),1)
	PROFILEFLAGS=-pg
endif
ifeq ($(strip $(DEBUG)),1)
	DEBUGFLAGS=-DDEBUG -g -rdynamic
endif
ifeq ($(strip $(32BITS)),1)
	ARCHFLAGS=-m32 -L/usr/lib32
endif

CXXFLAGS = -std=c++11 -Wall -Wno-unused-variable $(ARCHFLAGS) $(PROFILEFLAGS) $(DEBUGFLAGS) -O$(strip $(OPTIMIZE))

LDFLAGS  = -std=c++11 -lm $(ARCHFLAGS) $(PROFILEFLAGS) $(DEBUGFLAGS) -O$(strip $(OPTIMIZE)) -lrt

# Rules

Linux: GameLinux
Mac: GameMac

clean:
	rm -rf Game SecGame *.o *.exe Makefile.deps

# Order of objects is important here to deactivate standard sleep function.

GameLinux: Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o Game.o Main.o $(PLAYERS_OBJ_LINUX) Utils.o
	$(CXX) $^ -o $@ $(LDFLAGS)

GameMac: Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o Game.o Main.o $(PLAYERS_OBJ_MAC) Utils.o
	$(CXX) $^ -o $@ $(LDFLAGS)

SecGame: Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o SecGame.o SecMain.o Utils.o
	$(CXX) $^ -o $@ $(LDFLAGS)

%.exe: %.o Structs.o Settings.o State.o Info.o Random.o Board.o Action.o Player.o Registry.o SecGame.o SecMain.o Utils.o
	$(CXX) $^ -o $@ $(LDFLAGS)

Makefile.deps: *.cc
	$(CXX) $(CXXFLAGS) -MM *.cc > Makefile.deps

include Makefile.deps
