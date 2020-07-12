# Parallel-computing
Parallel implementation of Mandelbrot Set and Wirless Sensor Network.
# Dependecy
- C
- MPI
- openmp

# instruction for implementing WSN:
    1.make
    2. get a log.txt file

    if you are interested in observing the encrypt/dycrypt time for parallel implementation using OpenMP and
    serial implementation. you can input the commands below.
    1. gcc -fopenmp speedup.c AES.c 
    2. ./a.out
    The result is written in the sppedup2.txt 

    the speedup2.txt is for encrypt/dycrypt the text "9"
    the speedup1.txt is for encrypt/dycrypt the text "hello world"
