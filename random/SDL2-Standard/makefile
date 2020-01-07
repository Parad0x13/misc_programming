OBJS = main.cpp
CC = g++
INCLUDE_PATHS = -I./SDL2/include
LIBRARY_PATHS = -L./SDL2/lib

COMPILER_FLAGS = -Wl,-subsystem,windows -static-libgcc -static-libstdc++
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2

OBJ_NAME = SDL

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS)
