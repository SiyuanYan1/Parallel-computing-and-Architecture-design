/* File: baseStation.c
Author: Siyuan Yan
Date: 19st October 2019
Purpose: The baseStation function for simulating the base station in WSN
*/
#include "header.h"
#include "AES.h"
void master(MPI_Comm master_comm){
    extern int total_msg;
    FILE* fp;
    int nslaves,num_events=0;
    char master_buff[MAX_LEN];
    MPI_Status master_status;
    int total_msg_buff,num_msg;
    double start,end,t1,t2;
    time_t ltime;
    ltime=time(NULL);
    nslaves=20;
    int i;
    char recv_matrix[MAX_LEN];

    // double t1,t2;
    start=MPI_Wtime();
    // init_key();
    fp=fopen("log.txt","w");
    // fprintf(fp,"%s",asctime(localtime(&ltime)));

    while(nslaves>0){    
        t1=MPI_Wtime();
        MPI_Recv(encrpted_buf,MAX_LEN,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,master_comm,&master_status);
        t2=MPI_Wtime();
        if (master_status.MPI_TAG==MSG_EXIT)
            nslaves--;
       
        else if (master_status.MPI_TAG==MSG_EVENT){
            decrypt_p(encrpted_buf,decrypted_buf); 
            // decrypt(encrpted_buf,decrypted_buf);   
            
            fprintf(fp,"%s",decrypted_buf);
            fprintf(fp,"communication time between nodes and base station reported activation value is %f s\n\n",t2-t1);
            num_events++;
        }       
    }
    MPI_Recv(encrpted_buf,MAX_LEN,MPI_CHAR,MPI_ANY_SOURCE,MSG_COUNT,master_comm,&master_status);
    // decrypt(encrpted_buf,decrypted_buf);
    decrypt_p(encrpted_buf,decrypted_buf);
    total_msg_buff=atoi(decrypted_buf);
    end=MPI_Wtime();
    // printf("num events %d\n",num_events);
 

    //    MPI_Send(matrix,MAX_LEN,MPI_CHAR,0,MSG_MATRIX,master_comm);
    MPI_Recv(recv_matrix,MAX_LEN,MPI_CHAR,MPI_ANY_SOURCE,MSG_MATRIX,master_comm,&master_status);
    decrypt_p(recv_matrix,decrypted_buf);
    // printf("%s\n",decrypted_buf);
    fprintf(fp, "\nIteration\tNumber of msg passed\tNumber of events\n");
    fprintf(fp,"%s",decrypted_buf);
    total_msg_buff++;//reduce operation is a mssage passed in the network, a record is needed.
    // printf("master total:%d\n",total_msg_buff);
    fprintf(fp,"\nSimulation time is  %f s\n",end-start);
    fprintf(fp,"Number of messages passed throughout the network:%d\nNumber of events occurred throughout the network%d\n",total_msg_buff,num_events);
    fclose(fp);
}
