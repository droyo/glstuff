CC=gcc

INCLUDES = ../includes
CFLAGS += -I$(INCLUDES)
LFLAGS += `sdl-config --cflags --libs` -lGL -lGLU

SRC = geometry.c luacons.c pathing.c ridge.c tga_loader.c opengl_functions.c
OBJ = $(SRC:.c=.o)

all: ridge luacons

luacons: luacons.o opengl_functions.o
	$(CC) -c $(LFLAGS) luacons.o opengl_functions.o -o $@

ridge: ridge.o opengl_functions.o
	$(CC) -c $(LFLAGS) ridge.o opengl_functions.o -o $@

.c.o:
	$(CC) -c $(LFLAGS) $(CFLAGS) $< -o $@

clean:
	rm $(OBJ) luacons ridge
