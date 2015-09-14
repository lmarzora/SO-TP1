#Makefile

.SILENT:

TARGET = server
OBJS = ./app/pokemonCenter.o ./middle/svstub.o
SOCKETS = ./app/pokemonCenter.o ./middle/svstub.o ./transport/sockets.o
PIPES = ./app/pokemonCenter.o ./middle/svstub.o ./transport/pipes.o
CC = gcc
LD = $(CC)
LDOPTS = -o


$(TARGET): $(OBJS)
sockets:
	$(LD) $(LDOPTS) $(TARGET) $(SOCKETS)
pipes:
	$(LD) $(LDOPTS) $(TARGET) $(PIPES)

clean:
	rm -rf *.o *.bin $(TARGET)
