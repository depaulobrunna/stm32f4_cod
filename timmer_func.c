#include "stm32f4xx.h"                  // Device header

#define A GPIO_ODR_ODR_12 

int main(){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;//habilita o clock da porta A
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	
	
	GPIOD->ODR &= ~A;
	
	//confg pin timmer 
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB -> MODER |= GPIO_MODER_MODER3_1;//MODO AF
	GPIOB-> AFR[0] |= 1 << 12 ;
	GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED3_Pos;
	
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 16000 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM2->ARR = 1000 - 1;
	TIM2->CR1 |= TIM_CR1_CEN;
	
	//ADC CFG
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR2 |= ADC_CR2_ADON; //ENABLE ADC;

	
	while(1)
	{
		
		while(!(TIM2->SR & TIM_SR_UIF));
		TIM2->SR &= ~(TIM_SR_UIF);
		GPIOD->ODR ^= A;

	}
}

