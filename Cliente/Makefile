#Makefile

.SILENT:

TARGET = trainer
OBJS = ./app/trainer.o ./middle/clstub.o ./transport/sockets.o
CC = gcc
LD = $(CC)
LDOPTS = -o


$(TARGET): $(OBJS)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS)

trainer.o:	../commons/myTypes.h

clean:
	rm -rf *.o *.bin $(TARGET)
