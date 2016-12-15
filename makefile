
STD = -g -Wall -Wpedantic -Wextra --std=gnu99 -O3

SDLF = `pkg-config --cflags sdl2`
SDLL = `pkg-config --libs sdl2`

M = -lm

CC = clang

# phyllotaxis
phyllotaxis: phyllotaxis.c
	$(CC) $(STD) $(SDLF) $(SDLL) -o phyllotaxis phyllotaxis.c $(M)
