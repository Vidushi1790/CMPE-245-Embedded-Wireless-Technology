/*
===============================================================================
 Name        : myLisa.c
 Author      : Vidushi Jain
 Version     : V1.00
 Description : LISA HOMEWORK ASSIGNMENT
===============================================================================
*/

#include "myLisa.h"
#include <stdio.h>


char arbitrary_data[] = "Embedded Wireless Architecture Description Embedded wireless architecture with basic communication protocols and hands-on labs with state of the art embedded system development tools. Prerequisites: CMPE 180A and 180D, classified standing, or instructor co";
char tx_data[1024];
int data_length = 0;
char payload_data[512];

char sync_data[SYNC_SIZE] = {
										0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
		                                0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
										0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
	       	   	   	   	   	   	   	    0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF
                                     };
static int powerofTwo[] =
{
		1,2,4,8,16,32,64,128
};

int main()
{
    packet_s P;

    create_data_packet(&P);
    print_sync_packet(&P);
    convert_data_packet(&P);    
    //print_buffer(tx_data,data_length);
	
    reconvert_data_packet(tx_data,data_length);
    check_confidence_and_ret_payload(&P,payload_data); //Checke confidence level based on corrupted packets from the data

#if 0
    int corruptedBits = 0;
    
    uint8_t *file_data = (uint8_t*)malloc(BUFFER_SIZE *sizeof(uint8_t));
    memcpy(file_data, arbitrary_data, BUFFER_SIZE);
    
    create_data_file(file_data);  //Create a data file with 2k of Arbitrary data
    printf("\n------File Data w/o Sync Code------\n");
    print_file_data();
    
    srand(time(NULL));   //Find a random offset using rand() to write a Packet info to the File
    uint32_t offset = rand() % (BUFFER_SIZE - sizeof(packet_s));
    printf("\n\nOffset = %x|%d\n", offset, offset);
    
    memcpy(file_data + offset, &P, sizeof(packet_s)); //Write data to a file with sync code
    create_data_file(file_data);
    printf("\n------File Data w/ Sync Code------\n");
    print_file_data();

    corruptedBits = create_corrupted_sync_code(&P,&Pc); //Ask user to Enter corrupt percentage & create corrupted sync code based on corrupt input
    printf("\n------Corrupted Sync Code------\n");
    print_sync_packet(&Pc);
    
    memcpy(file_data + offset, &Pc, sizeof(packet_s)); //Write corrupted data to file again
    print_buffer(file_data,BUFFER_SIZE);
    create_data_file(file_data);
    printf("\n------File Data w/ Corrupted Sync Code------\n");
    print_file_data();

    check_confidence_and_return_payload(&P); //Checke confidence level based on corrupted packets from the data
#endif    
    return 0;
}



void convertDataToBinary(char data)
{
	int i = 0;
	for(i=7; i>=0; i--) {
		tx_data[data_length] = ((data >> i) & 1);
		data_length += 1;
	}
}

void create_data_packet(packet_s* P1)
{
	//Add SYNC Data to the TX Packet
	memcpy(P1->bytes, sync_data, SYNC_SIZE);

	//Add Payload to the TX Packet
	memcpy(P1->Payload, MSG, PAYLOAD_SIZE);
}

void convert_data_packet(packet_s* P)
{
	for (int i = 0; i < SYNC_SIZE; i++)
	{
		convertDataToBinary(P->bytes[i]);
	}

	for (int i = 0; i < PAYLOAD_SIZE; i++)
	{
		convertDataToBinary(P->Payload[i]);
	}
}

char convert_8_bits_to_char(const char* bits)
{
	int byte = 0;
	int count = 7;
	for(int i = 0;i < 8;i++)
	{
	  byte += (bits[i] - 48) * powerofTwo[count--];
	}
    char c = byte - '0';
	return c;
}

void reconvert_data_packet(char* rx_buff, int size)
{
    printf("\n\n------------\n");
	int bit_count = 0;
	char buffer[8] = {0};
	int data_len = 0;

	for (int i = 0; i < size; i++)
	{
       // printf("%u[%d] ",buff[bit_count],bit_count);
		buffer[bit_count++] = rx_buff[i];
		if (bit_count == 8) {
		    payload_data[data_len] = convert_8_bits_to_char(buffer);
            printf("%c ",payload_data[data_len]);
            data_len += 1;
			bit_count = 0;
		}
	}
}

void print_buffer(char* buff, int size)
{
    printf("\nBuffer data:");
    for (int i = 0; i < size; i++)
    {
    	if (i % 8 == 0)
    	{
   			printf("\n");
    	}
        printf("%u ", buff[i]);
    }
}

void create_data_file(char* buffer)
{
    FILE *fp;
    fp = fopen(FILENAME,"w");

    if(fp  == NULL)
    {
     printf("Error opening file!\n");
     exit(1);
    }

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        fprintf(fp, "%c", buffer[i]);
    }

    fclose(fp);
}

char* read_data_file(void)
{
    FILE *fp;
    fp = fopen(FILENAME,"r");

    char *read_data = (char*)malloc(BUFFER_SIZE *sizeof(uint8_t));

    if(fp  == NULL)
    {
     printf("Error opening file!\n");
     exit(1);
    }

    fread(read_data,BUFFER_SIZE,1,fp);

    printf("\n");

    fclose(fp);
    return read_data;
}

void print_file_data(void)
{
   FILE *fp;
   int c;

   fp = fopen(FILENAME,"r");
   while(1) {
      c = fgetc(fp);
      if( feof(fp) ) {
         break;
      }
      printf("%c", c);
   }
    
   printf("\n");
   fclose(fp);
}

void check_confidence_and_ret_payload(packet_s* P1, char *file_dump)
{
    int confidence_level;
    printf("\nPlease Enter Confidence Level:");
    scanf("%d",&confidence_level);

    float confidence_percent = 0.0;
    int payload_position = 0,calculated_confidence = 0;
    for(int i=0; i<SYNC_SIZE; i++)
    {
        for(int j = payload_position; j<BUFFER_SIZE; j++)
        {
            //printf("%d:%c|%d:%c\n",j,i,buffer[j],P1->bytes[i]);
            if(file_dump[j] == P1->bytes[i])
            {
                payload_position = j;
                calculated_confidence += 1;
                break;
            }
        }
    }
    printf("Calculated Confidence:%d | Payload Start Address:%d\n",calculated_confidence,payload_position);


    if(calculated_confidence >= confidence_level)
    {
        confidence_percent = ((float)calculated_confidence/32)*100;
        printf("\nConfidence percent: %f%%\n", confidence_percent);
        detect_and_print_payload(payload_position+1,file_dump); //Find the payload of the data
    }
    else
    {
        printf("\nSorry! Entered Confidence level is Less than Calculated Confidence Level");
        printf("\nCan't identified Payload\n");
    }
}

void check_confidence_and_return_payload(packet_s* P1)
{
    int confidence_level;
    printf("\nPlease Enter Confidence Level:");
    scanf("%d",&confidence_level);

    char *file_dump = read_data_file();
    
    float confidence_percent = 0.0;
    int payload_position = 0,calculated_confidence = 0;
    for(int i=0; i<SYNC_SIZE; i++)
    {
        for(int j = payload_position; j<BUFFER_SIZE; j++)
        {
            //printf("%d:%c|%d:%c\n",j,i,buffer[j],P1->bytes[i]);
            if(file_dump[j] == P1->bytes[i])
            {
                payload_position = j;
                calculated_confidence += 1;
                break;
            }
        }
    }
    printf("Calculated Confidence:%d | Payload Start Address:%d\n",calculated_confidence,payload_position);

    if(calculated_confidence >= confidence_level) 
    {
        confidence_percent = ((float)calculated_confidence/32)*100;
        printf("\nConfidence percent: %f%%\n", confidence_percent);
        detect_and_print_payload(payload_position+1,file_dump); //Find the payload of the data
    }
    else
    {
        printf("\nSorry! Entered Confidence level is Less than Calculated Confidence Level");
        printf("\nCan't identified Payload\n");
    }    
}

int create_corrupted_sync_code(packet_s* P1,packet_s* P2)
{
    int corrupt_percent;
    printf("\nPlease Enter Sync Field Corrupted Percentage(1-32):");
    scanf("%d",&corrupt_percent);
    
    memcpy(P2,P1,sizeof(packet_s));
    
    int BytesToChange = 0 , LeftBits =0;
    int BitsToChange = ((BUFFER_SIZE * corrupt_percent)/ 100);
    
    LeftBits = BitsToChange % 8;
    BytesToChange = BitsToChange/8;

    printf("BitsToChange: %d|%d|%d",BitsToChange,BytesToChange,LeftBits);

    for(int i=0; i< BytesToChange;i++)
    {
        P2->bytes[i] |= 0xFF;
    }
    
    for(int j=0;j<LeftBits;j++)
    {
        P2->bytes[BytesToChange] |= (1 << j);
    }
    
    return BitsToChange;
}

void detect_and_print_payload(int payload_address,char* buf)
{
	char identified_payload[PAYLOAD_SIZE];
    memcpy(identified_payload,buf + payload_address,PAYLOAD_SIZE);
    printf("\n-------Payload Identified-------\n%s",identified_payload);
    printf("\n------------------------------\n");
}

void print_sync_packet(packet_s* P)
{
	printf("------Displaying Packet------\n");
	printf("Sync Field:\n");
	for (int i = 0; i < SYNC_SIZE; i++)
	{
		printf("%x ", P->bytes[i]);
	}

    printf("\nPayload:\n");
	for (int i = 0; i < PAYLOAD_SIZE; i++)
	{
		printf("%X ", P->Payload[i]);
	}
	printf("\n\n-----END OF PACKET-----\n");
}