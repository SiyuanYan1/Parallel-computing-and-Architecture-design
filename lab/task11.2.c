
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdbool.h>

int BinSearch(int key, int *pId,int start,int end){
	while (start <= end){
		int mid = (end + start)/ 2;
		if (pId[mid] < key){
			start = mid + 1;
		}
		else if(pId[mid] > key){
			end = mid - 1;
		}
		else{
			return mid;
		}
	}
	return -1;
}

int main(int argc,char *argv[]){
	MPI_Init(&argc,&argv);
	int rank,num,i;
    int* buff1;
    double* buff2;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	FILE *fp;
    if(rank==0){
        fp=fopen("data.txt","r");
        fscanf(fp,"%d",&num);
        buff1=(int*)malloc(num*sizeof(int));
        buff2=(double*)malloc(num*sizeof(double));
        for(i=0;i<num;i++){
            fscanf(fp,"%d %lf",&buff1[i],&buff2[i]);
        }
        
    }
    MPI_Finalize();
    return 0;

}