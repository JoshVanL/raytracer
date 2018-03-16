RAY=raytracer
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
CC_OPTS=-fopenmp -Ofast -std=c++11 -c -pipe -Wno-switch -ggdb -g3 -ggdb
LN_OPTS=
CC=g++

########
#       SDL options
SDL_CFLAGS := $(shell sdl2-config --cflags)
GLM_CFLAGS := -I$(GLMDIR)
SDL_LDFLAGS := $(shell sdl2-config --libs)

RAYOBJ = $(B_DIR)/$(RAY).o
RASOBJ = $(B_DIR)/$(RAS).o


Build:
	mkdir -p build
	$(CC) $(CC_OPTS) -o $(B_DIR)/$(RAS).o $(S_DIR)/$(RAS).cpp $(SDL_CFLAGS) $(GLM_CFLAGS)
	$(CC) $(LN_OPTS) -fopenmp -o $(RASEXEC) $(RASOBJ) $(SDL_LDFLAGS)

clean:
	rm -f $(B_DIR)/*
