#Makefile

.SILENT:

TARGET = trainer
OBJS = ./app/trainer.o ./middle/clstub.o
SOCKETS = ./app/trainer.o ./middle/clstub.o ./transport/sockets.o
PIPES = ./app/trainer.o ./middle/clstub.o ./transport/pipes.o
CC = gcc
LD = $(CC)
LDOPTS = -o


$(TARGET): $(OBJS)
sockets:
	$(LD) $(LDOPTS) $(TARGET) $(SOCKETS)
pipes:
	$(LD) $(LDOPTS) $(TARGET) $(PIPES)

trainer.o:	../commons/myTypes.h

clean:
	rm -rf *.o *.bin $(TARGET)
