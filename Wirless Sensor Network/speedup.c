/* File: sppedup.c
Author: Siyuan Yan
Date: 19st October 2019
Purpose: The function is used for comparing the time of parallel encrption and serial encryption
*/
#include "AES.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[])
{   int i;
    FILE *fp=fopen("speedup2.txt","w");

    init_key();
    sprintf(buf, "9");
    fprintf(fp,"The experiment text is \n %s\n",buf);
    encrypt_p( buf, encrpted_buf);
    fprintf(fp,"The encryption content is %s\n",encrpted_buf);
    decrypt_p( encrpted_buf, decrypted_buf);
    fprintf(fp,"The decryption content is %s\n ",decrypted_buf);
    double t1,t2;
    clock_t start,end;
    double cpu_time_used;
    for (i=0;i<10;i++){  
        fprintf(fp,"iteration %d\n",i);
        start = clock();
        encrypt( buf, encrpted_buf);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(fp,"Serial encryption costs %lf\n",cpu_time_used);
        start = clock();
        decrypt(encrpted_buf, decrypted_buf);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(fp,"Serial decryption costs %lf\n",cpu_time_used);

        t1=omp_get_wtime();
        encrypt_p( buf, encrpted_buf);
        t2=omp_get_wtime();
        fprintf(fp,"parallel encrption using OpenMP costs %f\n",t2-t1);
        t1=omp_get_wtime();
        decrypt_p( encrpted_buf, decrypted_buf);
        t2=omp_get_wtime();
        fprintf(fp,"parallel decryption using OpenMP costs %f\n\n",t2-t1);
    }
    fclose(fp);

   return 0;
}
