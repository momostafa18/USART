/*
 * uart.c
 *
 *  Created on: Oct 9, 2022
 *      Author: mohamedmostafamohame
 */
#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"
#include "std_types.h"
#include <avr/interrupt.h>

#if(UART_TECHNIQUE==INTERRUPT)
volatile uint8 g_ubrr_Value = 0;
static volatile void(*g_uartRecieveCallBack)(void) = NULL_PTR;

ISR(USART_RXC_vect)
{
	g_ubrr_Value = UDR;
	(*g_uartRecieveCallBack)();
}
#endif

void UART_init(UART_CONFIG_TYPE (*config_type))
{
	uint16 UBR_value=0;
	/* Set the baud rate*/
	UBR_value=(uint32)(((F_CPU/(config_type->baudrate * 8UL)))-1);
	UBRRL=UBR_value;
	UBRRH=UBR_value >> 8;
	/* Set the double speed*/
	SET_BIT(UCSRA,U2X);
	/* Enable the transmitter and receiver*/
	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRB,RXEN);
	/* Set the number of data bit */
	SET_BIT(UCSRC,URSEL);
#if(UART_TECHNIQUE==INTERRUPT)
	SET_BIT(UCSRB,RXCIE);
#endif
	UCSRC = (UCSRC & 0xF9) | (config_type->db << 1);
	/* Set the parity mode*/
	UCSRC = (UCSRC & 0xCf) | (config_type->pm << 4);

}
void UART_sendByte(uint8 byte)
{
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR = byte;
}
#if(UART_TECHNIQUE==INTERRUPT)
void UART_receiveByteInterrupt()
{
	SET_BIT(UCSRB,RXCIE);
}
void UART_setCallBack(void (*recieve_ptrCallBack)(void))
{
	g_uartRecieveCallBack = recieve_ptrCallBack;
}
#elif(UART_TECHNIQUE==POLLING)
uint8 UART_receiveBytePolling(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
		while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
	    return UDR;
}
#endif
void UART_sendString(uint8 *s)
{
	uint8 count=0;
	UART_sendByte(0);
	while(s[count] != '\0')
	{
		UART_sendByte(s[count]);
		count++;
	}
	UART_sendByte(s[count]);
}
void UART_receiveString(uint8 *s)
{
	uint8 count=0;
	UART_sendByte(0);
#if(UART_TECHNIQUE==POLLING)
	s[count]=UART_receiveBytePolling();
	while(s[count] != '#')
	{
		count++;
		s[count]=UART_receiveBytePolling();
	}
	s[count]='\0';
#elif(UART_TECHNIQUE==INTERRUPT)
	UART_receiveByteInterrupt();
	s[count]=g_ubrr_Value;
	while(s[count]!='#')
	{
		count++;
		UART_receiveByteInterrupt();
		s[count]=g_ubrr_Value;
	}
	s[count]='\0';
#endif

}
