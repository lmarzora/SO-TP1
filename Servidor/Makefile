#Makefile

.SILENT:

TARGET = server
OBJS = ./app/pokemonCenter.o ./middle/server.o ./app/guarderiaPokemon.o
SOCKETS = ./transport/sockets.o
FIFOS = ./transport/fifos.o
QUEUES = ./transport/queues.o
SHMEM = ./transport/shmem.o
SIGNALS = ./transport/signals.o
CC = gcc
LD = $(CC)
LDOPTS = -o
LIBS = -lpthread -lrt


$(TARGET):
sockets: $(OBJS) $(SOCKETS)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(SOCKETS) $(LIBS)
fifos: $(OBJS) $(FIFOS)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(FIFOS) $(LIBS)
queues: $(OBJS) $(QUEUES)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(QUEUES) $(LIBS)
shmem: $(OBJS) $(SHMEM)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(SHMEM) $(LIBS)
signals: $(OBJS) $(SIGNALS)
	$(LD) $(LDOPTS) $(TARGET) $(OBJS) $(SIGNALS) $(LIBS)

clean:
	rm -rf *.o $(TARGET)
	cd app; rm -rf *.o
	cd middle; rm -rf *.o
	cd transport; rm -rf *.o
