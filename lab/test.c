
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main (int argc,char *argv[]){
	MPI_Init(&argc,&argv);
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	FILE *ua,*tf,*output;
	int N,M;
	float *u,*a,*t;
	int i,j;
	if (rank == 0){
		ua = fopen("u_a.txt","r");
		fscanf(ua,"%d",&N);
		u = (float *)calloc(N,sizeof(float));
		a = (float *)calloc(N,sizeof(float));
		for (i=0;i<N;i++){
			fscanf(ua,"%f %f\n",&u[i],&a[i]);
		}
	}
	else if(rank == 1){
		tf = fopen("t.txt","r");
		fscanf(tf,"%d",&M);
		t = (float *)calloc(M,sizeof(float));
		for (i=0;i<M;i++){
			fscanf(tf,"%f",&t[i]);
		}
	}
	MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
	int Nper = N / (size-2);
	if (rank == 0){
		for (i=2;i<size;i++){
			MPI_Send(u+(i-2)*Nper,Nper,MPI_FLOAT,i,0,MPI_COMM_WORLD);
			MPI_Send(a+(i-2)*Nper,Nper,MPI_FLOAT,i,1,MPI_COMM_WORLD);
		}
		free(u);
		free(a);
	}
	else if(rank == 1){
		for (i=2;i<size;i++){
			MPI_Send(t+(i-2)*Nper,Nper,MPI_FLOAT,i,2,MPI_COMM_WORLD);
		}
		free(t);
	}
	else{
		u = (float*)calloc(Nper,sizeof(float));
		a = (float*)calloc(Nper,sizeof(float));
		t = (float*)calloc(Nper,sizeof(float));
		MPI_Recv(u,Nper,MPI_FLOAT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		// for (i=0;i<Nper;i++){
		// 	printf("%f\t",u[i]);
		// }
		MPI_Recv(a,Nper,MPI_FLOAT,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Recv(t,Nper,MPI_FLOAT,1,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	float *resBuff;
	if (rank ==0){
		resBuff = (float*)calloc(N,sizeof(float));
		for(i=2;i<size;i++){
			MPI_Recv(resBuff+(i-2)*Nper,Nper,MPI_FLOAT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
		output = fopen("result.txt","w");
		for (i=0;i<N;i++){
			fprintf(output,"%f\n",resBuff[i]);
		}
		fclose(output);
		
	}
	else if(rank != 1){
		//calculation
		resBuff = (float*)calloc(Nper,sizeof(float));
		for (i=0;i<Nper;i++){
			//printf("%f,%f,%f\n",u[i],a[i],t[i]);
			resBuff[i] = u[i] + a[i] * t[i];
		}
		MPI_Send(resBuff,Nper,MPI_FLOAT,0,0,MPI_COMM_WORLD);
		free(u);
		free(a);
		free(t);
	}

	// free(u);
	// free(a);
	// free(t);
	free(resBuff);
	MPI_Finalize();
	return 0;
}