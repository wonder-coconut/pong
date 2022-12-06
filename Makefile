all : pong

pong : 
		g++ pong.cpp -o pong.o -lSDL2 -lSDL2_ttf -lSDL2_mixer