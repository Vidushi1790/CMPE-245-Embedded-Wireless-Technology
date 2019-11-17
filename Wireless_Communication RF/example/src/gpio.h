/*
===============================================================================
 Name        : gpio.h
 Author      : Vidushi Jain
 Version     : V1.00
 Description : LANDLINE TESTING
===============================================================================
*/

#ifndef __GPIO_H__
#define __GPIO_H__


#include <stdio.h>
#include <stdint.h>


void GPIOinitIn(uint8_t portNum, uint32_t pinNum);
void GPIOinitOut(uint8_t portNum, uint32_t pinNum);
void setGPIO(uint8_t portNum, uint32_t pinNum);
void clearGPIO(uint8_t portNum, uint32_t pinNum);



#endif //__GPIO_H__



