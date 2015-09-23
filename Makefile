#Makefile

.SILENT:

sockets:
	cd Cliente; make sockets
	cd Servidor; make sockets

pipes:
	cd Cliente; make pipes
	cd Servidor; make pipes

queues:
	cd Cliente; make queues
	cd Servidor; make queues

clean:
	cd Cliente; make clean
	cd Servidor; make clean
