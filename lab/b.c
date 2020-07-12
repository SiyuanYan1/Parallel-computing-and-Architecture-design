#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<mpi.h>
#define SEED 35791246

int main(){
    int niter=0;
    double x,y;
    int i,count=0;
    int size,rank,block,remain,sum;
    int* arr;
    double z,pi;
    MPI_Status stat;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if (rank==0){
        arr=(int*)malloc(size*sizeof(int));
         printf("enter a iter :\n");
         scanf("%d",&niter);
    }
    MPI_Bcast(&niter,1,MPI_INT,0,MPI_COMM_WORLD);
    srand(SEED);
    count=0;
    block=niter/size;
    remain=niter%size;
    for(i=block*rank;i<block*rank+block;i++){
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        z = x*x+y*y;
        if (z<=1) count++;
    }
    if (rank!=0)
        MPI_Send(&count,1,MPI_INT,0,0,MPI_COMM_WORLD);
    if (rank==0){
        sum=count;
        arr[0]=count;
        for(i=1;i<size;i++){
            MPI_Recv(arr+i,1,MPI_INT,i,0,MPI_COMM_WORLD,&stat);
            sum+=arr[i];
        }
        count=0;
        for(i=niter-remain;i<niter;i++){
            x = (double)rand()/RAND_MAX;
            y = (double)rand()/RAND_MAX;
            z = x*x+y*y;
            if (z<=1) count++;
        }
        sum+=count;
        pi=(double)sum/niter*4;
        printf("%g\n",pi);
       
    }
    MPI_Finalize();
    return 0;
}

