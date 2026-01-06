#ifndef ADC_MY_H
#define ADC_MY_H

#include "stm32f030x6.h"

// Чтение значения ADC
uint16_t AdcRead(void) {
	uint16_t result = 0;
    
    // Запускаем преобразование
    ADC1->CR |= ADC_CR_ADSTART;
    
    // Ждем завершения преобразования
    while(!(ADC1->ISR & ADC_ISR_EOC));
    
    // Читаем результат
    result = ADC1->DR;
    
    // Сбрасываем флаг
    ADC1->ISR |= ADC_ISR_EOC;
    
    return result;
}

#endif // ADC_MY_H