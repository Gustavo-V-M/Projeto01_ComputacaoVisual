
TARGET = main

# Detect OS
ifeq ($(OS),Windows_NT)
	EXE_EXT = .exe
	RM = del /S
	CP = copy
	SDL_DIR = lib\\SDL3
	SDL_IMAGE_DIR = lib\\SDL3_image
	PATH_SEP = \\
else
	EXE_EXT =
	RM = rm -f
	CP = cp
	SDL_DIR = lib/SDL3
	SDL_IMAGE_DIR = lib/SDL3_image
	PATH_SEP = /
endif

SDL_INC_DIR = $(SDL_DIR)$(PATH_SEP)include
SDL_LIB_DIR = $(SDL_DIR)$(PATH_SEP)lib
SDL_DLL_DIR = $(SDL_DIR)$(PATH_SEP)bin
SDL_DLL_FILE = SDL3.dll

SDL_IMAGE_INC_DIR = $(SDL_IMAGE_DIR)$(PATH_SEP)include
SDL_IMAGE_LIB_DIR = $(SDL_IMAGE_DIR)$(PATH_SEP)lib
SDL_IMAGE_DLL_DIR = $(SDL_IMAGE_DIR)$(PATH_SEP)bin
SDL_IMAGE_DLL_FILE = SDL3_image.dll

CC = g++
CFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Wno-unused-result
LDFLAGS = -L$(SDL_LIB_DIR) -L$(SDL_IMAGE_LIB_DIR)
LDLIBS = -lSDL3 -lSDL3_image
PROJECT_INC_DIR = includes
INC_DIRS = $(addprefix -I, $(SDL_INC_DIR) $(PROJECT_INC_DIR) $(SDL_IMAGE_INC_DIR))

INC = $(wildcard includes/*.h)
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean debug

all: $(TARGET)$(EXE_EXT)

clean:
	$(RM) *.o
	$(RM) $(TARGET)$(EXE_EXT)

$(TARGET)$(EXE_EXT): $(OBJ)
	$(CC) $(CFLAGS) $(INC_DIRS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
ifeq ($(OS),Windows_NT)
	$(CP) $(SDL_DLL_DIR)\\$(SDL_DLL_FILE) .\\$(SDL_DLL_FILE)
	$(CP) $(SDL_IMAGE_DLL_DIR)\\$(SDL_IMAGE_DLL_FILE) .\\$(SDL_IMAGE_DLL_FILE)
endif

%.o: %.cpp $(INC)
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

debug:
	@echo SRC = $(SRC)
	@echo OBJ = $(OBJ)
	@echo INC = $(INC)