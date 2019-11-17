/*
===============================================================================
 Name        : gpio.c
 Author      : Vidushi jain
 Version     : 1
 Copyright   : $(copyright)
 Description : GPIO Input/Output Functionality
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "gpio.h"
#include "board.h"
#include "chip.h"

//Initialize the port and pin as input.
void GPIOinitIn(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO->DIR &= ~(1 << pinNum);
	}
	else if (portNum == 1)
	{
		LPC_GPIO1->DIR &= ~(1 << pinNum);
	}
	else if (portNum == 2)
	{
		LPC_GPIO2->DIR &= ~(1 << pinNum);
	}
	else
	{
		puts("Not a valid port!\n");
	}
}

//Initialize the port and pin as outputs.
void GPIOinitOut(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO->DIR |= (1 << pinNum);
	}
	else if (portNum == 1)
	{
		LPC_GPIO1->DIR |= (1 << pinNum);
	}
	else if (portNum == 2)
	{
		LPC_GPIO2->DIR |= (1 << pinNum);
	}
	else if (portNum == 3)
	{
		LPC_GPIO3->DIR |= (1 << pinNum);
	}
	else
	{
		puts("Not a valid port!\n");
	}
}

void setGPIO(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO->SET = (1 << pinNum);
	}
	else if (portNum == 3)
	{
		LPC_GPIO3->SET = (1 << pinNum);
	}
	//Can be used to set pins on other ports for future modification
	else
	{
		puts("Only port 0 is used, try again!\n");
	}
}

//Deactivate the pin
void clearGPIO(uint8_t portNum, uint32_t pinNum)
{
	if (portNum == 0)
	{
		LPC_GPIO->CLR = (1 << pinNum);
	}
	else if (portNum == 3)
	{
		LPC_GPIO3->CLR = (1 << pinNum);
	}
	//Can be used to clear pins on other ports for future modification
	else
	{
		puts("Only port 0 is used, try again!\n");
	}
}

