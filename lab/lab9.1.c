#include<mpi.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main(int argc, char* argv[]){
    FILE* fp;
    int size,rank,total,i,offset;
    double x_in,x_out;
    MPI_Status stat;
    double* recv_buff;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if (rank==0){
        fp=fopen("ExpResult.txt","r");
        fscanf(fp,"%d",&total);
        recv_buff=(double*)malloc(total*sizeof(double));
    }
 
    MPI_Bcast(&total,1,MPI_INT,0,MPI_COMM_WORLD);
    if (rank==3){
        recv_buff=(double*)malloc(total*sizeof(double));
        offset=0;
    }
    for(i=0;i<total;i++){
        if(rank==0){

            fscanf(fp,"%lf",&x_in);
            x_out=x_in-4*x_in+7;
            MPI_Send(&x_out,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD);
        }
        if(rank==1){
            MPI_Recv(&x_in,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&stat);
            x_out=x_in*x_in*x_in+sin(x_in/8);
            MPI_Send(&x_out,1,MPI_DOUBLE,2,0,MPI_COMM_WORLD);
        }
          if(rank==2){
            MPI_Recv(&x_in,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD,&stat);
            x_out=2*x_in*x_in*x_in*x_in+cos(4*x_in)+3*M_PI;
            MPI_Send(&x_out,1,MPI_DOUBLE,3,0,MPI_COMM_WORLD);
        }
        if(rank==3){
            MPI_Recv(&x_in,1,MPI_DOUBLE,2,0,MPI_COMM_WORLD,&stat);
            x_out=3*x_in*x_in-2*x_in+tan(x_in)/3;
            recv_buff[i]=x_out;
        }
    }
    if (rank==3)
        MPI_Send(recv_buff,total,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    if(rank==0){
        MPI_Recv(recv_buff,total,MPI_DOUBLE,3,0,MPI_COMM_WORLD,&stat);
        for(i=0;i<total;i++){
            printf("%lf\n",recv_buff[i]);
        }
    } 

    MPI_Finalize();
    return 0;

}

// #include<stdlib.h>
// #include<stdio.h>
// #include <mpi.h>
// #include <math.h>
// int main(){
//     int size,rank,i,total;
//     FILE* fp;
//     double num,x_in,x_out;
//     int tag=0;
//     MPI_Status status;
//     double* buf;
//     int offset;
//     double* recv_buf;
    
//     MPI_Init(NULL,NULL);
//     MPI_Comm_size(MPI_COMM_WORLD,&size);
//     MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//     if (rank==0){
       
//         fp=fopen("ExpResult.txt","r");
//         fscanf(fp,"%d",&total);
//         recv_buf=(double*)malloc(total*sizeof(double));
//     }
 
//     MPI_Bcast(&total,1,MPI_INT,0,MPI_COMM_WORLD);
//     if (rank==3){
//         buf=(double*)malloc(total*sizeof(double));
//         offset=0;

//     }
//     for (i=0;i<total;i++){
//         if (rank==0){
//             fscanf(fp,"%lf",&x_in);
//             x_out=x_in-4*x_in+7;        
//             MPI_Send(&x_out,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD);
//         }
//         if(rank==1){
//             MPI_Recv(&x_in,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
//             x_out=x_in*x_in*x_in+sin(x_in/8);
  
//             MPI_Send(&x_out,1,MPI_DOUBLE,2,tag,MPI_COMM_WORLD);
//         }
//         if (rank==2){
//             MPI_Recv(&x_in,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD,&status);
//             x_out=2*(x_in*x_in*x_in*x_in)+cos(4*x_in)+3*M_PI;
//             MPI_Send(&x_out,1,MPI_DOUBLE,3,tag,MPI_COMM_WORLD);
//         }
//         if(rank==3){
//             MPI_Recv(&x_in,1,MPI_DOUBLE,2,tag,MPI_COMM_WORLD,&status);
//             x_out=3*(x_in*x_in)-2*x_in+tan(x_in)/3;
//             // printf("%d\n",offset++);
//             buf[offset++]=x_out;
            
//         }
//     }
//     if (rank==3){
//         MPI_Send(buf,total,MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
//     }
//     if (rank==0){
//         MPI_Recv(recv_buf,total,MPI_DOUBLE,3,tag,MPI_COMM_WORLD,&status);
//         for(i=0;i<total;i++)
//             printf("%lf\n",recv_buf[i]);
//     }

//     MPI_Finalize();
// }
