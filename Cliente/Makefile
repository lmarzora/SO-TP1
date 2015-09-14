#Makefile

.SILENT:

TARGET = trainer
OBJS = ./app/trainer.o ./middle/clstub.o
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


trainer.o:	../commons/myTypes.h

clean:
	rm -rf *.o $(TARGET)
	cd app; rm -rf *.o
	cd middle; rm -rf *.o
	cd transport; rm -rf *.o
