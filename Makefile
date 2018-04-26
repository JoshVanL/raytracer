.PHONY: build clean

RAS=rasterizer

########
#   Directories
S_DIR=source
B_DIR=build
GLMDIR=$(S_DIR)/glm
########
#   Output
RAYEXEC=$(B_DIR)/$(RAY)
RASEXEC=$(B_DIR)/$(RAS)

# default build settings
CC_OPTS=-fopenmp -O3 -std=c++11 -c -lglfw3 -pipe -Wno-switch -ggdb -DGLEW_BUILD  -g3 -ggdb -pthread -lSOIL -lm

#links { "SOIL", "glfw3", "opengl32", "gdi32", "glu32" }

CC=g++

########
#       SDL options
SDL_CFLAGS := $(shell sdl2-config --cflags) -lSOIL -lglut -lGLU -lGL
GLM_CFLAGS := -I$(GLMDIR)
SDL_LDFLAGS := $(shell sdl2-config --libs) -lSOIL -lGL

RAYOBJ = $(B_DIR)/$(RAY).o
RASOBJ = $(B_DIR)/$(RAS).o


build:
	mkdir -p build
	$(CC) $(CC_OPTS) -o $(B_DIR)/$(RAS).o $(S_DIR)/$(RAS).cpp $(SDL_CFLAGS) $(GLM_CFLAGS)
	$(CC) $(LN_OPTS) -fopenmp -o $(RASEXEC) $(RASOBJ) $(SDL_LDFLAGS)
	rm build/*.o

clean:
	rm -f $(B_DIR)/*
