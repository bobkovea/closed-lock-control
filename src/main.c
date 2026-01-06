#include "boardsetuputils.h"
#include "uart.h"
#include "utils.h"
#include "adc.h"

int main(void){
	
    ClockInit();
    GpioInit();
	UsartInit();
	AdcInit();
	
	while(1)
	{		
		uint16_t adcCount;
		
		GPIOA->BSRR |= GPIO_BSRR_BR_4;	
		delay_ms(500);
		
		//UART1_SendString("Hello UART!\r\n");
		
		adcCount = AdcRead();
		
		UART1_SendChar((uint8_t)(adcCount >> 8));
		UART1_SendChar((uint8_t)adcCount);
		
		GPIOA->BSRR |= GPIO_BSRR_BS_4;	
		delay_ms(500);
	}
}
