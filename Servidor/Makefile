#Makefile

.SILENT:

TARGET = server
OBJS = ./app/pokemonCenter.o ./middle/server.o
SOCKETS = ./transport/sockets.o
PIPES = ./transport/pipes.o
CC = gcc
LD = $(CC)
LDOPTS = -o


$(TARGET):
sockets: $(OBJS) $(SOCKETS)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(SOCKETS)
pipes: $(OBJS) $(PIPES)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(PIPES)

clean:
	rm -rf *.o $(TARGET)
	cd app; rm -rf *.o
	cd middle; rm -rf *.o
	cd transport; rm -rf *.o
