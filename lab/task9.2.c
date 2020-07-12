
#include<stdio.h>

#include <stdlib.h> 
#include <math.h>
#include <time.h>
#include<mpi.h>
#include <unistd.h>


int triNum(int inNum){
    return (inNum*(inNum+1))/2;
}
int main(int argc,char* argv[]){

    int size,rank,num,i,remain,block,offset,offset1;
    int* arr;
    int *small_arr;
    int res;
    MPI_Status stat;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank==0){
        scanf("%d",&num);
         arr=(int*)malloc(num*sizeof(int));
    }
    MPI_Bcast(&num,1,MPI_INT,0,MPI_COMM_WORLD);
    block=num/size;
    remain=block%size;
    small_arr=(int*)malloc(block*sizeof(int));
    offset=0;
    if(rank==0){
        for(i=0;i<block;i++){
            arr[offset++]=triNum(i+1);
        }
    }
    else{
        for(i=rank*block;i<rank*block+block;i++){
            small_arr[offset++]=triNum(i+1);
        }
        MPI_Send(small_arr,block,MPI_INT,0,0,MPI_COMM_WORLD);

    }
    if (rank==0){
        for(i=1;i<size;i++){
            offset1=i*block;
            MPI_Recv(arr+offset1,block,MPI_INT,i,0,MPI_COMM_WORLD,&stat);
        }
        for(i=num-remain;i<num;i++){
            arr[i]=triNum(i+1);
        }
        for(i=0;i<num;i++){
            printf("%d\n",arr[i]);
        }
    }
    MPI_Finalize();
    return 0;
  


   

    

}


