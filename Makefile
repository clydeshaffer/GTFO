OBJS = lib/serialib/lib/serialib.cpp  StateMachine/*.cpp StateMachine/Nodes/*.cpp gtfo.cpp
CPPC = g++

OBJ_NAME = bin/GTFO

ifeq ($(OS), Windows_NT)
	ifeq ($(XCOMP), yes)
		CPPC = i686-w64-mingw32-g++
		OBJ_NAME := $(OBJ_NAME).exe
	endif

	INCLUDE_PATHS = -I lib/serialib/lib

	#LIBRARY_PATHS specifies the additional library paths we'll need
	LIBRARY_PATHS = -L$(SDL_ROOT)/lib

	#COMPILER_FLAGS specifies the additional compilation options we're using
	# -w suppresses all warnings
	# -Wl,-subsystem,windows gets rid of the console window
	# change subsystem,windows to subsystem,console to get printfs on command line
	COMPILER_FLAGS = -w -Wl,-subsystem,console
	DEFINES = -D _WIN32

	#LINKER_FLAGS specifies the libraries we're linking against
	LINKER_FLAGS = -lmingw32 -Wl,-Bstatic -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lcomdlg32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc -lsetupapi
else
	COMPILER_FLAGS = -w -g
	LINKER_FLAGS = -lSDL2
	ifeq ($(OS), wasm)
		CC = emcc
		CPPC = emcc
		COMPILER_FLAGS += -s USE_SDL=2 -D WASM_BUILD -D EMBED_ROM_FILE='"$(ROMFILE)"'
		OBJ_NAME = bin/$(OS)/index.html
		LINKER_FLAGS += --embed-file $(ROMFILE) --shell-file web/shell.html -s EXPORTED_FUNCTIONS='["_LoadRomFile", "_main", "_SetButtons"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'
	endif
endif

all : $(OBJS)
	mkdir -p bin
	$(CPPC) $(OBJS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)