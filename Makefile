include config.mk

OBJECTS = pong-ramanathan.o
ASSETS = paddlehit.wav wallhit.wav SevenSegment.ttf

all : pong-ramanathan

pong-ramanathan.o :
		$(CC) $(CFLAGS) -c pong-ramanathan.cpp

pong-ramanathan :	$(OBJECTS)
		$(CC) $(CFLAGS) $(RELROFLAGS) $(OBJECTS) -o pong-ramanathan $(LDFLAGS)

install : pong-ramanathan $(ASSETS)
		mkdir -p $(DESTDIR)$(PREFIX)/bin
		mkdir -p $(DESTDIR)$(PREFIX)/lib/pong-ramanathan
		cp -f paddlehit.wav $(DESTDIR)$(PREFIX)/lib/pong-ramanathan
		cp -f wallhit.wav $(DESTDIR)$(PREFIX)/lib/pong-ramanathan
		cp -f SevenSegment.ttf $(DESTDIR)$(PREFIX)/lib/pong-ramanathan
		cp -f pong-ramanathan $(DESTDIR)$(PREFIX)/bin
		chmod 755 $(DESTDIR)$(PREFIX)/bin/pong-ramanathan

uninstall : 
		rm -rf $(DESTDIR)$(PREFIX)/bin/pong-ramanathan
		rm -rf $(DESTDIR)$(PREFIX)/lib/pong-ramanathan

clean :
		rm -rf pong-ramanathan