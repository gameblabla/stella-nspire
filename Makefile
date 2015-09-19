PRGNAME     = stella

EXESUFFIX = 
CC          	= gcc
CCP          	= g++
LD     			= g++

CFLAGS = -DLAYER -std=c++11 -O0 -g

# add SDL dependencies
SDL_LIB     = $(TOOLCHAIN)/lib
SDL_INCLUDE = $(TOOLCHAIN)/include 
CFLAGS += -I./emucore -I./common -I./opendingux

LDFLAGS     = -L$(SDL_LIB) $(CC_OPTS) -lstdc++ -lSDL 

# Files to be compiled
SRCDIR   =  ./emucore ./common ./opendingux .
VPATH    = $(SRCDIR)
SRC_C    = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_CP   = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))
OBJ_C    = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_CP   = $(notdir $(patsubst %.cpp, %.o, $(SRC_CP)))
OBJS     = $(OBJ_C) $(OBJ_CP)

# Rules to make executable
$(PRGNAME)$(EXESUFFIX): $(OBJS)  
	$(LD) $(CFLAGS) -o $(PRGNAME)$(EXESUFFIX) $^ $(LDFLAGS)

$(OBJ_C) : %.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_CP) : %.o : %.cpp
	$(CCP) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(PRGNAME)$(EXESUFFIX) *.o
