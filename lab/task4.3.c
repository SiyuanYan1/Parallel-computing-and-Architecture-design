#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
	int rank, size;
	int theSize = 1, root = 0;
	float n, sum, output = 0.0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank==0){
        scanf("%f",&n);
    }
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    float x =rank/n;
   while(x<1.0){
    output=(1/n)*4/(1+x*x);
    x+=size/n;
   }
   MPI_Reduce(&output,&sum,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
   if(rank==0){
       printf("%f\n",sum);
   }
	MPI_Finalize();
	return 0;
}


// if (rank == root) {
// 		printf("Input integer n: ");
// 		fflush(stdout);
// 		scanf("%f", &n);
// 		printf("\n");
// 		fflush(stdout);
// 	}
// 	MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD);
// 	float x = rank/n;
// 	while (x < 1.0) {
// 		output += (1/n)*4/(1+x*x);
// 		x += size*theSize/n;
// 	};

// 	MPI_Reduce(&output, &sum, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
// 	if (rank == root) {
// 		printf("%f\n", sum);
// 	}