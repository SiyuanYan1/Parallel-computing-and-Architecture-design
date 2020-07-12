#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(){
    FILE *pInfile;
    FILE *fp;
    float a_coeff,b_coeff,c_coeff,x1,x2,disc;
    float x1r,x1i,x2r,x2i;
    char a,b,c;
    float arr[3];
    float arr1[4];
    int fileElementCount=0,constNeg=-1;
    int my_rank,p,count,i;
    MPI_Status status;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);


    if(my_rank==0){
        pInfile=fopen("quad.txt","r");
        fscanf(pInfile,"%d",&count);
        fscanf(pInfile,"%s %s %s",&a,&b,&c);  
    }
    MPI_Bcast(&count,1,MPI_INT,0,MPI_COMM_WORLD);

    switch(my_rank){
        case 0:{

            for(i=0;i<count;i++){
                fscanf(pInfile,"%f\t%f\t%f",&a_coeff,&b_coeff,&c_coeff );
                disc=b_coeff*b_coeff-4*a_coeff*c_coeff;
                arr[0]=disc;
                arr[1]=a_coeff;
                arr[2]=b_coeff;
                MPI_Send(arr,3,MPI_FLOAT,1,0,MPI_COMM_WORLD);
            }
            fclose(pInfile);
            break;
        }
        case 1:{
            
             for(i=0;i<count;i++){
                 MPI_Recv(arr,3,MPI_FLOAT,0,0,MPI_COMM_WORLD,&status);
                 disc=arr[0];
                 a_coeff=arr[1];
                 b_coeff=arr[2];
                 if(disc>=0){
                    x1r=(-b_coeff+sqrt(disc))/(2*a_coeff);
                    x2r=(-b_coeff-sqrt(disc))/(2*a_coeff);
                    x1i=0;
                    x2i=0;
                 }
                else{
                     x1r=(-b_coeff)/(2*a_coeff);
                     x2r=(-b_coeff)/(2*a_coeff);
                     x1i=sqrt(-disc)/(2*a_coeff);
                     x2i=-sqrt(-disc)/(2*a_coeff);
                     printf("num%f\n",x1r);
                 }
                 arr1[0]=x1r;
                 arr1[1]=x2r;
                 arr1[2]=x1i;
                 arr1[3]=x2i;
                 MPI_Send(arr1,4,MPI_FLOAT,2,0,MPI_COMM_WORLD);
             }   

            break;
        }
        case 2:{
            fp=fopen("roots.txt","w");
            fprintf(fp,"%d\n",count);
            fprintf(fp, "x1\tx2\tx1_real\tx1_img\tx2_real\tx2_img\n");
            for(i=0;i<count;i++){
                MPI_Recv(arr1,4,MPI_FLOAT,1,0,MPI_COMM_WORLD,&status);
                if(arr1[2]==0 && arr1[3]==0){
                    fprintf(fp,"%.1f\t%.1f\n",arr1[0],arr1[1]);
                }
                else{
                    printf("%.1f\t%.1f\t%.1f\t%.1f\n",arr1[0],arr1[2],arr1[1],arr1[3]);
                    fprintf(fp,"\t\t%.1f\t%.1f\t%.1f\t%.1f\n",arr1[0],arr1[2],arr1[1],arr1[3]);
                }

            }
            fclose(fp);
            break;
        }
    }
    MPI_Finalize();
    return 0;

}