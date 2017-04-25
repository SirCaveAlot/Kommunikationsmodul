/*
 * UART.h
 *
 * Created: 4/7/2017 3:15:29 PM
 *  Author: jakpa844
 */ 


#ifndef UART_H_
#define UART_H_

//ISR(USART0_RX_vect);

void USART_Init( );

void USART_Transmit(uint8_t data, int port);

void Interrupt_Init();

void UART_Queue_Init(void);

void UART_Queue_Put(uint8_t new);

void UART_Queue_Get(uint8_t *old);

#endif /* UART_H_ */