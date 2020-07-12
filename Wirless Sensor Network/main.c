/* File: main.c
Author: Siyuan Yan
Date: 19st October 2019
Purpose: The main logic for simulating the WSN
*/
#include "header.h"
#include "AES.h"
void slaves(MPI_Comm comm_cart,MPI_Comm master_comm,int nbrs[4]);
void master(MPI_Comm master_comm);
int total_msg;
int main(){
    MPI_Init(NULL,NULL);
    int size,rank;
    int nbrs[4];
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int ndims=2, dims[2]={5,4},periods[2]={0,0},reorder=1,coords[2];
    MPI_Comm comm_cart,new_comm;
   
    
    if (size!=NNODES){// #nodes should be 21
        printf("Number of Nodes must be %d\n",NNODES);
        MPI_Finalize();
        exit(0);
    }
    //slpit communicator into a basestation and 20 nodes
    MPI_Comm_split(MPI_COMM_WORLD,rank==0,rank,&new_comm);
    init_key();
    if (rank==0){
  
        master(MPI_COMM_WORLD);
    }  
    else{
      
        MPI_Cart_create(new_comm,ndims,dims,periods,reorder,&comm_cart);
	    MPI_Cart_shift(comm_cart,0,1,&nbrs[0],&nbrs[1]);
    	MPI_Cart_shift(comm_cart,1,1,&nbrs[2],&nbrs[3]);
      
        slaves(comm_cart,MPI_COMM_WORLD,nbrs);      
    }
    MPI_Finalize();
    return 0;
}

