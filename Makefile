include config.mk

OBJECTS = pong_ramanathan.o

all : pong_ramanathan

pong_ramanathan.o :
		$(CC) $(CFLAGS) -c pong_ramanathan.cpp

pong_ramanathan :	$(OBJECTS)
		$(CC) $(CFLAGS) $(RELROFLAGS) $(OBJECTS) -o pong_ramanathan $(LDFLAGS)

install : pong_ramanathan
		mkdir -p $(DESTDIR)$(PREFIX)/bin
		cp -f pong_ramanathan $(DESTDIR)$(PREFIX)/bin
		chmod 755 $(DESTDIR)$(PREFIX)/bin/pong_ramanathan

uninstall : 
		rm -rf $(DESTDIR)$(PREFIX)/bin/pong_ramanathan

clean :
		rm -rf pong_ramanathan