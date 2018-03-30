FILE=raytracer

########
#   Directories
S_DIR=source
B_DIR=build
GLMDIR=../glm/
########
#   Output
EXEC=$(B_DIR)/$(FILE)

# default build settings
CC_OPTS=-c -fopenmp  -Ofast -std=c++11 -pipe -Wno-switch -ggdb -g3 
LN_OPTS=-lm
CC=g++

########
#       SDL options
SDL_CFLAGS := $(shell sdl2-config --cflags)
GLM_CFLAGS := -I$(GLMDIR)
SDL_LDFLAGS := $(shell sdl2-config --libs)

.PHONY: all build clean

########
#   This is the default action
all:build


########
#   Object list
#
OBJ = $(B_DIR)/$(FILE).o


########
#   Main build rule
build:
	mkdir -p $(B_DIR)
	$(CC) $(CC_OPTS) -o $(B_DIR)/$(FILE).o $(S_DIR)/$(FILE).cpp -lm $(SDL_CFLAGS) $(GLM_CFLAGS)
	$(CC) -fopenmp -fopenmp -std=c++11 -o $(EXEC) $(OBJ) $(LN_OPTS) $(S_DIR)/light/intersection.cpp  `sdl2-config --cflags --libs`


clean:
	rm -f $(B_DIR)/*
