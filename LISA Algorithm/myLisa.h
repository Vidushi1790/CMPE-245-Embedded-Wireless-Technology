/*
===============================================================================
 Name        : myLisa.h
 Author      : Vidushi Jain
 Version     : V1.00
 Description : LISA HOMEWORK ASSIGNMENT
===============================================================================
*/

#ifndef __MY_LISA_H__
#define __MY_LISA_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


#define BUFFER_SIZE     256     //2k bits = 256 Bytes
#define SYNC_SIZE       32      //32*8
#define MSG "SJSU_CMPE245_LANDLINE_TESTING_DEMO_1234" //Payload Message
#define PAYLOAD_SIZE    sizeof(MSG)
#define FILENAME "mydata.txt"
#define CONFIDENCELVL 20
#define CORRUPT_PERCENT 1
#define TOTAL_SIZE (BUFFER_SIZE + sizeof(packet_s))

typedef struct
{
    char bytes[SYNC_SIZE];
    char Payload[PAYLOAD_SIZE];
}packet_s;

extern char arbitrary_data[] ;
extern char tx_data[1024];
extern char payload_data[512];
extern int data_length;

void print_sync_packet(packet_s* P);
void reconvert_data_packet(char* rx_buff, int size);
void create_data_packet(packet_s* P1);
void convert_data_packet(packet_s* P);
void print_buffer(char* buff, int size);
void create_data_file(char* buffer);
void print_file_data(void);
void check_confidence_and_return_payload(packet_s* P1);
void check_confidence_and_ret_payload(packet_s* P1, char *file_dump);
int create_corrupted_sync_code(packet_s* P1,packet_s* P2);
void detect_and_print_payload(int payload_address,char* buf);


#endif //__MY_LISA_H__



