#Makefile

.SILENT:

sockets:
	cd Cliente; make sockets
	cd Servidor; make sockets

fifos:
	cd Cliente; make fifos
	cd Servidor; make fifos

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

fcntl:
	cd Cliente; make fcntl
