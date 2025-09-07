
TARGET = main

SDL_DIR = lib\\SDL3
SDL_INC_DIR = $(SDL_DIR)\\include
SDL_LIB_DIR = $(SDL_DIR)\\lib
SDL_DLL_DIR = $(SDL_DIR)\\bin
SDL_DLL_FILE = SDL3.dll

SDL_IMAGE_DIR = lib\\SDL3_image
SDL_IMAGE_INC_DIR = $(SDL_IMAGE_DIR)\\include
SDL_IMAGE_LIB_DIR = $(SDL_IMAGE_DIR)\\lib
SDL_IMAGE_DLL_DIR = $(SDL_IMAGE_DIR)\\bin
SDL_IMAGE_DLL_FILE = SDL3_image.dll


CC = g++
CFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Wno-unused-result
LDFLAGS = -L$(SDL_LIB_DIR) -L$(SDL_IMAGE_LIB_DIR)
LDLIBS = -lSDL3 -lSDL3_image
PROJECT_INC_DIR = includes
INC_DIRS = $(addprefix -I, $(SDL_INC_DIR) $(PROJECT_INC_DIR) $(SDL_IMAGE_INC_DIR))

SUBDIR = 
INC = $(wildcard includes/*.h)
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

clean:
	del /S *.o
	del /S $(TARGET).exe

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INC_DIRS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	copy $(SDL_DLL_DIR)\\$(SDL_DLL_FILE) .\\$(SDL_DLL_FILE)
	copy $(SDL_IMAGE_DLL_DIR)\\$(SDL_IMAGE_DLL_FILE) .\\$(SDL_IMAGE_DLL_FILE)

%.o: %.cpp $(INC)
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

debug:
	@echo SRC = $(SRC)
	@echo OBJ = $(OBJ)
	@echo INC = $(INC)