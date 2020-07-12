#include<stdlib.h>
#include<stdio.h>
#include <mpi.h>
#include <math.h>
int main(){
    int rank,size,num;
    MPI_Status stat;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank==0){
        scanf("%d",&num);
        MPI_Send(&num,1,MPI_INT,rank+1,0,MPI_COMM_WORLD);
    }
    else{
        MPI_Recv(&num,1,MPI_INT,rank-1,0,MPI_COMM_WORLD,&stat);
        if (rank<size-1)
            MPI_Send(&num,1,MPI_INT,rank+1,0,MPI_COMM_WORLD);
        
    }
    printf("received data in rank%d, val is %d\n",rank,num);
    MPI_Finalize();
    return 0;
}