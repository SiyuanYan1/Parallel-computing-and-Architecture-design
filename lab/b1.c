#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
int main()
{
	FILE *pInfile,*pOutfile;
	float a_coeff, b_coeff, c_coeff, x1, x2, disc;
	float x1r, x1i, x2r, x2i;
	int fileElementCount = 0, constNeg = -1;;
	int my_rank;
	int p;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	// WRITE PART(a) HERE
	float package[3];
	if (my_rank == 0){
		pInfile = fopen("quad.txt","r");
		fscanf(pInfile,"%d",&fileElementCount);
	}
	MPI_Bcast(&fileElementCount,1,MPI_FLOAT,0,MPI_COMM_WORLD);
	switch (my_rank){
		case 0:{
		// CONTINUE WITH PART (a) HERE
		char a,b,c;
		fscanf(pInfile,"%c\t%c\t%c",&a,&b,&c);
		for (p=0;p<fileElementCount;p++){
			fscanf(pInfile,"%.1f\t%.1f\t%.1f",&a_coeff,&b_coeff,&c_coeff);
			disc = pow(b_coeff,2) - 4 * a_coeff * c_coeff;
			package[0] = a_coeff;
			package[1] = b_coeff;
			package[2] = disc;
			MPI_Send(package,3,MPI_FLOAT,1,0,MPI_COMM_WORLD);
		}
		fclose(pInfile);
		break;
		}
		case 1:{
		// WRITE PART (b) HERE
			MPI_Recv(package,3,MPI_FLOAT,0,0,MPI_COMM_WORLD,&status);
			a_coeff = package[0];
			b_coeff = package[1];
			disc = package[2]
			if (disc == 0){
				x1 = x2 = -b_coeff / (2 * a_coeff);
			}
			else if(disc < 0){
				x1r = -b_coeff / (2 * a_coeff);
				x1i = sqrt(abs(disc)) / (2 * a_coeff);
				x2r = -b_coeff / (2 * a_coeff);
				x2i = -sqrt(abs(disc)) / (2 * a_coeff);
				MPI_Send()
			}
			else{
				x1 = (-b_coeff + sqrt(d)) / (2 * a_coeff);
				x2 = (-b_coeff - sqrt(d)) / (2 * a_coeff);
			}
			if (disc >= 0){
				MPI_Send(&x1,1,MPI_FLOAT,2,1,MPI_COMM_WORLD);
				MPI_Send(&x2,1,MPI_FLOAT,2,1,MPI_COMM_WORLD);
			}
			else{
				MPI_Send(&x1r,1,MPI_FLOAT,2,2,MPI_COMM_WORLD);
				MPI_Send(&x1i,1,MPI_FLOAT,2,2,MPI_COMM_WORLD);
				MPI_Send(&x2r,1,MPI_FLOAT,2,2,MPI_COMM_WORLD);
				MPI_Send(&x2i,1,MPI_FLOAT,2,2,MPI_COMM_WORLD);
			}
		break;
		}
		case 2:{
		// WRITE PART (c) HERE
		pOutfile = fopen("roots.txt","w");
		fprintf(pOutfile,"%d\n",fileElementCount);
		fprintf(pOutfile,"x1\tx2\tx1_real\tx1_img\tx2_real\tx2_img\n");
		for (p=0;p<fileElementCount;p++){
			MPI_Recv(&x1,1,MPI_FLOAT,1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			if (status.MPI_TAG == 1){
				MPI_Recv(&x2,1,MPI_FLOAT,1,1,MPI_COMM_WORLD,&status);
				fprintf(pOutfile,"%1f\t%1f\n",x1,x2);
			}
			else if(status.MPI_TAG == 2){
				x1r = x1;
				MPI_Recv(&x1i,1,MPI_FLOAT,1,2,MPI_COMM_WORLD,&status);
				MPI_Recv(&x2r,1,MPI_FLOAT,1,2,MPI_COMM_WORLD,&status);
				MPI_Recv(&x2i,1,MPI_FLOAT,1,2,MPI_COMM_WORLD,&status);
				fprintf(pOutfile,"\t\t%1f\t%1f\t%1f\t%1f\n",x1r,x1i,x2r,x2i);
			}
		}
		fclose(pOutfile);
		break;
		}
	}
	MPI_Finalize();
	return 0;
}