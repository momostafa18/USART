/*
 ================================================================================================
 Name        : Exercise1.c
 Author      : Mohamed Tarek
 Description : Exercise to test the UART functionality
 Date        : 4/11/2014
 ================================================================================================
 */

#include "uart.h"
#include<avr/io.h>
uint8 data;
uint8 newdata=0;
uint8 Str[20];

void mycall(void)
{
	newdata=1;
}
int main(void)
{
	//uint8 data;
	UART_CONFIG_TYPE config={DISABLED,ONE_BIT,EIGHT_BITS,9600};
	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_init(&config);
	SREG |=(1<<7);
	UART_setCallBack(mycall);
    while(1)
    { 
    	/*//data=UART_receiveBytePolling(); /* Receive Byte from Terminal */
    	/*UART_receiveByteInterrupt();
    	if(newdata==1)
    	{
     	UART_sendByte(data);
     	newdata=0;
    	}*//* Re-send the received byte to Terminal */

		UART_receiveString(Str);
		/* Receive String from Terminal */
   if(newdata==1)
{
	    UART_sendString(Str);
	    newdata=0;
}
		}/* Re-send the string to Terminal2 */
    }

