#ifndef UART_MY_H
#define UART_MY_H

#include "stm32f030x6.h"

// Функция для отправки одного символа
void UART1_SendChar(uint8_t ch) {
    // Ждем, пока буфер передатчика не освободится
    while (!(USART1->ISR & USART_ISR_TXE));
    // Отправляем символ
    USART1->TDR = ch;
}

// Функция для отправки строки
void UART1_SendString(char* str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

#endif // UART_MY_H