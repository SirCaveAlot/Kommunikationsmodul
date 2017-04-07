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

void USART_Transmit(uint8_t data);

void Interrupt_Init();

uint8_t USART_Receive( void );

#endif /* UART_H_ */