#include "stm32f4xx.h"                  // Device header

#define INVERTE_LED			GPIOD->ODR ^= GPIO_ODR_OD12;

TIM_TypeDef *tim = TIM2;
GPIO_TypeDef *gpio = GPIOA;

int main()
{
	//LED TEST
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= GPIO_MODER_MODER12_0;
	
	//CONFIGURACAO DOS PINOS/LEDS
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	gpio -> MODER |= GPIO_MODER_MODER0_1| 
										GPIO_MODER_MODER1_1| 
										GPIO_MODER_MODER2_1|
										GPIO_MODER_MODER3_1;
	gpio->AFR[0] |= 1 << GPIO_AFRL_AFSEL0_Pos|
									 1 << GPIO_AFRL_AFSEL1_Pos|
	                 1 << GPIO_AFRL_AFSEL2_Pos|
									 1 << GPIO_AFRL_AFSEL3_Pos;		
	gpio->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED0_Pos|
										3 << GPIO_OSPEEDR_OSPEED1_Pos|
										3 << GPIO_OSPEEDR_OSPEED2_Pos|
										3 << GPIO_OSPEEDR_OSPEED3_Pos;

	
	//CONFIG TIMER pwm
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	tim->PSC = 1000 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	tim->ARR = 16000 - 1;//QUANTO 'TEMPO'LEVA PARA OCORRER UMA ONDA INTEIRA
	tim->CR1 |= TIM_CR1_CEN;
	
	tim->CCMR1 = (tim->CCMR1 & ~TIM_CCMR1_OC1M_Pos)| (6 << TIM_CCMR1_OC1M_Pos)|(6 << TIM_CCMR1_OC2M_Pos)
								 |(1 << TIM_CCMR1_OC1PE_Pos)|(1 << TIM_CCMR1_OC2PE_Pos);
								 
	tim->CCMR2 = (tim->CCMR2 & ~TIM_CCMR2_OC3M_Pos)| (6 << TIM_CCMR2_OC3M_Pos)|(6 << TIM_CCMR2_OC4M_Pos)
								 |(1 << TIM_CCMR2_OC3PE_Pos)|(1 << TIM_CCMR2_OC4PE_Pos);
	
	tim->CR1 |= TIM_CR1_ARPE;
	tim->CCR1 = 8000 - 1;//tempo alto
	tim->CCR2 = 8000 - 1;
	tim->CCR3 = 8000 - 1;
	tim->CCR4 = 8000 - 1;
								 
	tim->CCER = (tim->CCER & ~TIM_CCER_CC1E)| (1 << TIM_CCER_CC1E_Pos)|(1 << TIM_CCER_CC2E_Pos)
							 |(1 << TIM_CCER_CC3E_Pos)|(1 << TIM_CCER_CC4E_Pos);
	while(1)
	{
		while(!(TIM2->SR & TIM_SR_UIF));
		TIM2->SR &= ~(TIM_SR_UIF);
		INVERTE_LED
		
		
	}
}


