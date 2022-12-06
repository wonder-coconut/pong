CC = g++
PREFIX = /usr
CFLAGS = -Wall
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer
RELROFLAGS = -Wl,-z,relro,-z,now