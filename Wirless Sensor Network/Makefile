program: main.c baseStation.c nodes.c AES.c
	sudo mpicc -fopenmp main.c baseStation.c nodes.c AES.c
	mpirun -n 21 ./a.out
