// #include "header.h"
#include<stdio.h>
#include<omp.h>
#include<string.h>
#include<time.h>
#define keysize 1
#define KEY_LENGTH 256
#define NUM_THREADS 8
#define MAX_LEN 1024
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))
// xtime is a macro that finds the product of {02} and the argument to xtime modulo {1b}  
#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))
// The array that stores the round keys.
unsigned char RoundKey[240];
// The Key input to the AES Program
unsigned char Key[32];
char buf[MAX_LEN];
char buf1[MAX_LEN];
char buf[MAX_LEN];
char encrpted_buf[MAX_LEN];
char decrypted_buf[MAX_LEN];
//  char master_buff[10240];
void init_key();
void encrypt(char* chunk_input, char* chunk_output);
void decrypt(char* chunk_output, char* chunk_oo);
void encrypt_p( char* chunk_input, char* chunk_output);
void decrypt_p( char* chunk_output, char* chunk_oo);