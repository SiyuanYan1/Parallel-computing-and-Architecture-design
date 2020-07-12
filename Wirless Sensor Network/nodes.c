/* File: nodes.c
Author: Siyuan Yan
Date: 19st October 2019
Purpose: The node function for simulating 20 nodes in WSN
*/
#include "header.h"
#include "AES.h"
#include <ifaddrs.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void slaves(MPI_Comm comm_cart,MPI_Comm master_comm,int nbrs[4]){ 
    extern int total_msg;
    int i,rand_num,count,num_event=0,rank,iteration=0,num_msg=0,activation_value;
    int msg_count,event_count;
    int recv_msg,recv_event;
    double encrypt_time1,encrypt_time2;
    int recv_buff[4]={-1,-1,-1,-1};
    char recv_buf1[4][MAX_LEN];
    char matrix[MAX_LEN*100];
    time_t time1;
    char *time_s;
    MPI_Request requests[8];
    MPI_Status status[8];
    MPI_Comm_rank(comm_cart,&rank);
    int countArray[4][10]={0};
    int sliding_window[1][4];
    memset(sliding_window,-1,sizeof(sliding_window));
 

    while(iteration<50){
        msg_count=0;
        event_count=0;
        recv_msg=0;
        recv_event=0;
        srand(time(0)+rank+iteration);
        count=0;
        rand_num=rand()%10; //generate a random number that range is between 0 and 9
        memset(recv_buff,-1,sizeof(recv_buff));
        for (i=0;i<4;i++){ //send val to all adj nodes,receive values from all adj nodes
            if (nbrs[i]>=0){  //if the node has adj node
                num_msg++;
                msg_count++;
                sprintf(buf,"%d",rand_num);
                encrypt_p(buf,encrpted_buf);
                MPI_Isend(encrpted_buf,MAX_LEN,MPI_CHAR,nbrs[i],0,comm_cart,&requests[count++]);
                MPI_Irecv(encrpted_buf,MAX_LEN,MPI_CHAR,nbrs[i],0,comm_cart,&requests[count++]);	
                decrypt_p(encrpted_buf,decrypted_buf);
                recv_buff[i]=atoi(decrypted_buf);
            }             
        }
        MPI_Waitall(count,requests,status);
        memcpy(sliding_window[0],sliding_window[1],sizeof(int)*4);
        memcpy(sliding_window[1],sliding_window[2],sizeof(int)*4);
        memcpy(sliding_window[2],recv_buff,sizeof(int)*4);
        //check if an event happen for current node
        memset(countArray,0,sizeof(countArray));
        for(i=0;i<4;i++){
            if (recv_buff[i]!=-1){  //it means receive rand val from the adj node
                countArray[i][recv_buff[i]]=1;
            }
        }
        //sliding window
       for(i=0;i<4;i++){
            if (sliding_window[0][i]!=-1 && (sliding_window[0][i]==recv_buff[0]|| sliding_window[0][i]==recv_buff[1]||sliding_window[0][i]==recv_buff[2]||sliding_window[0][i]==recv_buff[3])){
                countArray[i][sliding_window[0][i]]=1;
            }
            if (sliding_window[1][i]!=-1 && (sliding_window[1][i]==recv_buff[0]|| sliding_window[1][i]==recv_buff[1]||sliding_window[1][i]==recv_buff[2]||sliding_window[1][i]==recv_buff[3])){
                countArray[i][sliding_window[0][i]]=1;
            }
        }
        int j;
        for (i=0;i<10;i++){
            int sum_count=0;
            for(j=0;j<4;j++){
                sum_count+=countArray[j][i];
            }

            if (sum_count>=3){ //event occurs
                    activation_value=i;
                    time1 = time(NULL);
                    time_s = ctime(&time1);
                    time_s[strlen(time_s)-1] = '\0';
                       // get ip address for each node
                    struct ifaddrs *ifaddr, *tem;
                    int val;
                    val = getifaddrs(&ifaddr);
                    tem = ifaddr;	
                  
                    
                    sprintf(buf,"%s: Iteration: %d activation value:%d Reference Node:%d[IP address:%s ]Adjacent nodes: %d %d %d %d\n",
                    time_s,iteration,activation_value,rank,inet_ntoa(((struct sockaddr_in*)ifaddr->ifa_addr)->sin_addr),nbrs[0],nbrs[1],nbrs[2],nbrs[3]);
                    freeifaddrs(ifaddr);
                    encrypt_p(buf,encrpted_buf);
                    MPI_Send(encrpted_buf,MAX_LEN,MPI_CHAR,0,MSG_EVENT,master_comm);
                    event_count++;
                    msg_count++;
                    num_msg++;    
            }     
        }     
         
        MPI_Reduce(&event_count,&recv_event,1,MPI_INT,MPI_SUM,0,comm_cart);
        MPI_Reduce(&msg_count,&recv_msg,1,MPI_INT,MPI_SUM,0,comm_cart);
        if (rank==0){
            sprintf(&matrix[strlen(matrix)],"%d \t\t\t%d\t\t\t%d\n",iteration,recv_msg,recv_event);
        }
        iteration++;
      
    }
	MPI_Send(buf,0,MPI_CHAR,0,MSG_EXIT,master_comm);
    num_msg++;
    MPI_Reduce(&num_msg,&total_msg,1,MPI_INT,MPI_SUM,0,comm_cart);
    if (rank==0){
        sprintf(buf,"%d",total_msg);
        encrypt_p(buf,encrpted_buf);
        MPI_Send(encrpted_buf,MAX_LEN,MPI_CHAR,0,MSG_COUNT,master_comm);
        num_msg++;
        encrypt_p(matrix,encrpted_buf);
        MPI_Send(encrpted_buf,MAX_LEN,MPI_CHAR,0,MSG_MATRIX,master_comm);
    }
}
