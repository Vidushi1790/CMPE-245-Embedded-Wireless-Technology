/*
===============================================================================
 Name        : timer.c
 Author      : $VIDUSHI JAIN
 Version     : V1.10
 Copyright   : $(copyright)
 Description : RF WIRELESS TESTING
===============================================================================
*/

#include "board.h"
#include <stdio.h>
#include "chip.h"
#include "myLisa.h"
#include "gpio.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define SAMPLING_RATE_HZ	100
#define OVER_SAMPLING_RATE_HZ	1	//SAMPLING_RATE_HZ * 10
#define TICKRATE_HZ1 (11 * SAMPLING_RATE_HZ * OVER_SAMPLING_RATE_HZ)	/* 11 ticks per second */

#define LED_GREEN_PORT	3
#define LED_GREEN_PIN	25
#define MAX_DATA	1024
#define DATA_TX_RX_PORT	0
#define TX_PIN		2  //DIP SWITCH 5
#define RX_PIN 		3

int start_tx = 0;
int start_rx = 0;
int bit_index = 0;
int input_mode = 0 ;
int counter = 0;
int bit_count = 0;
int check_bit = 0;
uint32_t sampling_rate, oversampling_rate;

void send_one_bit(char* transmit_data, int bit_index)
{
	if(transmit_data[bit_index] == 0x01)
	{
		LPC_GPIO3->SET = (1<<LED_GREEN_PIN);
		LPC_GPIO->SET = (1<<TX_PIN);
	}
	else
	{
		LPC_GPIO3->CLR = (1<<LED_GREEN_PIN);
		LPC_GPIO->CLR = (1<<TX_PIN);
	}
}

/**
 * @brief	Handle interrupt from 32-bit timer
 * @return	Nothing
 */
void TIMER0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER0, 1))
	{
		if(start_tx)
		{
			if(bit_index < data_length)
			{
				send_one_bit(tx_data,bit_index);
				bit_index += 1;
			}
			else {
				bit_index = 0;
				start_tx = 0;
				input_mode = 0;
				LPC_GPIO->CLR = (1<<TX_PIN);
				printf("\nTransmission Done\n");
			}
		}
		else if(input_mode == 2)
		{
			if(LPC_GPIO->PIN & (1 << RX_PIN)){
				start_rx = true;
			}

		if(start_rx) {
			if (LPC_GPIO->PIN & (1 << RX_PIN))
			{
				check_bit += 1;
			} else {
				check_bit += 0;
			}
			counter++;

			if (counter == oversampling_rate) {
				float sampling = check_bit / oversampling_rate;
				if (sampling < 0.5)
					received_data[bit_count] = '0';
				else {
					received_data[bit_count] = '1';
				}
				bit_count++;
				total_data_received++;
				counter = 0;
				check_bit = 0;
			}

				if (bit_count == MAX_DATA) {
					bit_count = 0;
					data_received = 1;
					start_rx = 0;
				}
			}
		}

		Chip_TIMER_ClearMatch(LPC_TIMER0, 1);
	}
}

/**
 * @brief	main routine for blinky example
 * @return	Function should not exit.
 */
int main(void)
{
	uint32_t timerFreq,tick_rate;
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

    packet_s P;
    create_data_packet(&P);
    convert_data_packet(&P);

	LPC_GPIO3->DIR |= (1<<LED_GREEN_PIN);
    LPC_GPIO->DIR |= (1 << TX_PIN);	// Set as output
    LPC_GPIO->DIR &= ~(1 << RX_PIN); // Set as input

	printf("\n\nPlease Enter Sampling rate: \n");
    scanf("%d",&sampling_rate);

	printf("\n\nPlease Enter Over Sampling rate: \n");
    scanf("%d",&oversampling_rate);

    /* Enable timer 1 clock */
	Chip_TIMER_Init(LPC_TIMER0);

	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();
	tick_rate = (11 * sampling_rate * oversampling_rate) ;
	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 1);
	Chip_TIMER_SetMatch(LPC_TIMER0, 1, (timerFreq / tick_rate));
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 1);
	Chip_TIMER_Enable(LPC_TIMER0);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);


	/* LEDs toggle in interrupt handlers */
	while (1) {
		if(input_mode == 0)
		{
			printf("\n\nEnter a command to activate TX(1) & RX(2): \n");
			scanf("%d", &input_mode);
			if(input_mode == 1)
			{
				start_tx = 1;
				bit_index = 0;
			}
			else if(input_mode == 2)
			{
				if(data_received == 1) {
						reconvert_data_packet(received_data,total_data_received);
						check_confidence_and_ret_payload(&P,data_len);
						memset(payload_data,0,data_len);
						total_data_received = 0;
						data_received = 0;
						data_len = 0;
					    input_mode = 0;
					}
			}
		}
		__WFI();
	}

	return 0;
}
