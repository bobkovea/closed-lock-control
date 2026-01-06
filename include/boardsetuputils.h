#ifndef BOARDSETUPUTILS_H
#define BOARDSETUPUTILS_H

#include "stm32f030x6.h"

void SwitchToExternalClock()
{
	
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSE;
	
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
}

void SetPeripheralClock(const uint8_t enable)
{
	const uint32_t ahb_enr = RCC_AHBENR_GPIOAEN;   // GPIOA
	
	const uint32_t apb1_enr = 0;
							
    const uint32_t apb2_enr = RCC_APB2ENR_USART1EN |
							  RCC_APB2ENR_ADC1EN;
							
	if(enable)
	{
		RCC->AHBENR |= ahb_enr;
		RCC->APB1ENR |= apb1_enr;
		RCC->APB2ENR |= apb2_enr;

	}
	else
	{
		RCC->AHBENR &= ~ahb_enr;
		RCC->APB1ENR &= ~apb1_enr;
		RCC->APB2ENR &= ~apb2_enr;
	}
}

void ClockInit(void)
{	
	SwitchToExternalClock();
	SetPeripheralClock(1);
}
	

void GpioInit(void)
{ 
	
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER4 | GPIO_MODER_MODER9);
	
    GPIOA->MODER |= (3 << GPIO_MODER_MODER2_Pos) | // Analog
					(1 << GPIO_MODER_MODER4_Pos) | // Output
					(2 << GPIO_MODER_MODER9_Pos); // Alternate
					
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;
    GPIOA->AFR[1] |= (1 << GPIO_AFRH_AFSEL9_Pos);
	
}
void UsartInit(void)
{	
	USART1->BRR = 8000000 / 115200;
  
    // Включаем передатчик (TX) и сам UART
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void AdcInit(void)
{	
 // 2. Калибровка АЦП (обязательный шаг!)
    // Сбрасываем калибровку
    ADC1->CR &= ~ADC_CR_ADEN;
    ADC1->CR |= ADC_CR_ADCAL;
    
    // Ждем завершения калибровки
    while(ADC1->CR & ADC_CR_ADCAL);
    
    // 3. Включаем АЦП
    ADC1->CR |= ADC_CR_ADEN;
    // Ждем готовности АЦП
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    
    // 4. Настраиваем параметры
    // Одиночное преобразование, 12-битный режим
    ADC1->CFGR1 &= ~(ADC_CFGR1_CONT | ADC_CFGR1_RES);
    // Разрешение 12 бит (по умолчанию)
    
    // 5. Настраиваем время выборки
    // Для канала 2 устанавливаем время выборки 239.5 циклов (максимальное)
    ADC1->SMPR = 7; // 111b = 239.5 циклов для всех каналов
    
    // 6. Настраиваем последовательность преобразований
    // Один канал в последовательности
    ADC1->CHSELR = ADC_CHSELR_CHSEL2; // Выбираем канал 2 (PA2)
    
    // 7. Включаем АЦП
    ADC1->CR |= ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY));

}


#endif // BOARDSETUPUTILS_H