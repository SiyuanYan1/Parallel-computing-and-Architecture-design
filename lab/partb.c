#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include<math.h>
int main(){
    FILE* pInfile;
    double a_coeff,b_coeff,c_coeff,x1,x2,disc;
    double x1r,x1i,x2r,x2i;
    int fileElementCount=0,constNeg=-1;
    int my_rank;
    int i;
    int p;
    MPI_Status status;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    if (my_rank==0){
        pInfile=fopen("quad.txt","r");
        fscanf(pInfile,"%d",&fileElementCount);
    }
    double package[3];
    MPI_Bcast(&fileElementCount,1,MPI_INT,0,MPI_COMM_WORLD);

    switch(my_rank){
        case 0:{
      
            break;
        }
        case 1:{
            double buf[4];
            for(i=0;i<fileElementCount;i++){
                MPI_Recv(package,3,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
                disc=package[2];
                a_coeff=package[0];
                b_coeff=package[1];
                if (disc>0){
                    x1r=(b_coeff+sqrt(disc))/(2*a_coeff);
                    x2r=(b_coeff-sqrt(disc))/(2*a_coeff);
                    // x1i=0;
                    // x2i=0;
                    MPI_Send(&x1r,1,MPI_DOUBLE,2,0,MPI_COMM_WORLD);
                    MPI_Send(&x2r,1,MPI_DOUBLE,2,0,MPI_COMM_WORLD);
                    printf("??%lf\n",x1r);
                }
                else if(disc==0){
                    x1r=(-b_coeff)/(2*a_coeff);
                    x2r=(-b_coeff)/(2*a_coeff);
                    MPI_Send(&x1r,1,MPI_DOUBLE,2,0,MPI_COMM_WORLD);
                    MPI_Send(&x2r,1,MPI_DOUBLE,2,0,MPI_COMM_WORLD);
                    printf("%lf2\n",x1r);
                }
                else if (disc<0){
                    x1r=(-b_coeff)/(2*a_coeff);
                    x2r=(-b_coeff)/(2*a_coeff);
                    x1i=(sqrt(fabs(disc))/(2*a_coeff));
                    x2i=-(sqrt(fabs(disc))/(2*a_coeff));
                    MPI_Send(&x1r,1,MPI_DOUBLE,2,1,MPI_COMM_WORLD);
                    MPI_Send(&x2r,1,MPI_DOUBLE,2,1,MPI_COMM_WORLD);
                    MPI_Send(&x1i,1,MPI_DOUBLE,2,1,MPI_COMM_WORLD);
                    MPI_Send(&x2i,1,MPI_DOUBLE,2,1,MPI_COMM_WORLD);
                    printf("%lf3\n",x1r);
            }

            }
            
             break;
        }        
        case 2:{
            FILE* fp=fopen("roots.txt","w");
            fprintf(fp,"%d\n",fileElementCount);
            fprintf(fp,"x1\tx2\tx1_real\tx1_img\tx2_real\tx2_img\n");
            MPI_Recv(&x1r,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(status.MPI_TAG==0){
                MPI_Recv(&x2r,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD,&status);
                fprintf(fp,"%lf\t%lf\n",x1r,x2r);
            }
            if(status.MPI_TAG==1){
                MPI_Recv(&x2r,1,MPI_DOUBLE,1,1,MPI_COMM_WORLD,&status);
                MPI_Recv(&x1i,1,MPI_DOUBLE,1,1,MPI_COMM_WORLD,&status);
                MPI_Recv(&x2i,1,MPI_DOUBLE,1,1,MPI_COMM_WORLD,&status);
                fprintf(fp,"\t\t%lf\t%lf\t%lf\t%lf\n",x1r,x1i,x2r,x2i);
            }
            fclose(fp);
            break;
        }

       
    }
   
     MPI_Finalize();
        return 0;

}