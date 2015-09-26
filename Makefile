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

shmem:
	cd Cliente; make shmem
	cd Servidor; make shmem

clean:
	cd Cliente; make clean
	cd Servidor; make clean

signals:
	cd Cliente; make signals
	cd Servidor; make signals
