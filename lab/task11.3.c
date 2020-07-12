
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main (int argc,char *argv[]){
	MPI_Init(&argc,&argv);
	int rank,i,offset;
    int size,num,block,remain;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
    FILE* u_a_fp;
    MPI_Status status;
    FILE* t_fp;
    double *u,*a,*t;
    if (rank==0){
        u_a_fp=fopen("u_a.txt","r");
        fscanf(u_a_fp,"%d",&num);
        u=(double*)malloc(num*sizeof(double));
        a=(double*)malloc(num*sizeof(double));
        fscanf(u_a_fp,"%lf %lf",&u[0],&a[0]);
        for(i=0;i<num;i++){
            fscanf(u_a_fp,"%lf %lf",&u[i],&a[i]);
        }
    }
    if (rank==1){
        t_fp=fopen("t.txt","r");
        fscanf(t_fp,"%d",&num);
        t=(double*)malloc(num*sizeof(double)); 
        fscanf(t_fp,"%lf",&t[0]); 
        for(i=0;i<num;i++)
            fscanf(t_fp,"%lf",&t[i]);
    }
    MPI_Bcast(&num,1,MPI_INT,0,MPI_COMM_WORLD);
    block=num/(size-2);
    if (rank==0){
        for(i=2;i<size;i++){
            MPI_Send(u+block*(i-2),block,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
            MPI_Send(a+block*(i-2),block,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
        }
        free(u);
        free(a);
    }
    else if(rank==1){
        for(i=2;i<size;i++){
            MPI_Send(t+block*(i-2),block,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
        }
        free(t);
    }
    else{
        double* recv_u=(double*)malloc(block*sizeof(double));
        double* recv_a=(double*)malloc(block*sizeof(double));
        double* recv_t=(double*)malloc(block*sizeof(double));
        double* result=(double*)malloc(block*sizeof(double));
        MPI_Recv(recv_u,block,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
        MPI_Recv(recv_a,block,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
        MPI_Recv(recv_t,block,MPI_DOUBLE,1,0,MPI_COMM_WORLD,&status);
        for(i=0;i<block;i++){
            result[i]=recv_u[i]+(recv_a[i]*recv_t[i]);
        }
        MPI_Send(result,block,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }
    if(rank==0){
        FILE* res_fp=fopen("result.txt","w");
        double* res_buf=(double*) malloc(num*sizeof(double));
        for(i=2;i<size;i++){
            offset=(i-2)*block;
            MPI_Recv(res_buf+offset,num,MPI_INT,i,0,MPI_COMM_WORLD,&status);
        }
        for(i=0;i<num;i++){
            fprintf(res_fp,"%lf\n",res_buf[i]);
        }
       
    }

    MPI_Finalize();
    return 0;
}
