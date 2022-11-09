/*
 * uart.h
 *
 *  Created on: Oct 9, 2022
 *      Author: mohamedmostafamohame
 */

#ifndef UART_H_
#define UART_H_
#include "std_types.h"



#define POLLING   0
#define INTERRUPT 1

#define UART_TECHNIQUE    POLLING


typedef enum
{
	DISABLED,EVEN_PARITY=2,ODD_PARITY
}PARITY_MODE;

typedef enum
{
	ONE_BIT,TWO_BITS
}STOP_BIT_SELECT;

typedef enum
{
	FIVE_BITS,SIX_BITS,SEVEN_BITS,EIGHT_BITS,NINE_BITS=7
}DATA_BIT_MODE;

typedef struct
{
	PARITY_MODE pm;
	STOP_BIT_SELECT sb;
	DATA_BIT_MODE db;
	uint32 baudrate;
}UART_CONFIG_TYPE;

#if(UART_TECHNIQUE==INTERRUPT)
extern volatile uint8 g_ubrr_Value;
#endif

void UART_init(UART_CONFIG_TYPE (*config_type));

void UART_sendByte(uint8 byte);
#if(UART_TECHNIQUE==INTERRUPT)

void UART_receiveByteInterrupt(void);
#elif(UART_TECHNIQUE==POLLING)
uint8 UART_receiveBytePolling(void);
#endif

void UART_sendString(uint8 *s);

void UART_receiveString(uint8 *s);
#if(UART_TECHNIQUE==INTERRUPT)
void UART_setCallBack(void (*recieve_ptrCallBack)(void));
#endif
#endif /* UART_H_ */
