#Makefile

.SILENT:

TARGET = server
OBJS = ./app/pokemonCenter.o ./middle/svstub.o ./transport/sockets.o
CC = gcc
LD = $(CC)
LDOPTS = -o


$(TARGET): $(OBJS)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS)



clean:
	rm -rf *.o *.bin $(TARGET)
