#include "stm32f4xx.h"                  // Device header

#define LEDT GPIO_ODR_OD6

const uint16_t duty[6] =  {125,118,87,39,7,1};
volatile uint8_t pos = 0,led = 0;

void TIM7_IRQHandler(void);
int main()
{
	//LED TESTE
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC -> MODER |= GPIO_MODER_MODER6_0;
	GPIOC->ODR &= ~LEDT;
	//CONFIGURACAO DOS PINOS/LEDS
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= GPIO_MODER_MODER12_1| 
										GPIO_MODER_MODER13_1| 
										GPIO_MODER_MODER14_1|
										GPIO_MODER_MODER15_1;
	GPIOD->AFR[1] |= 2 << GPIO_AFRH_AFSEL12_Pos|
									 2 << GPIO_AFRH_AFSEL13_Pos|
	                 2 << GPIO_AFRH_AFSEL14_Pos|
									 2 << GPIO_AFRH_AFSEL15_Pos;		
	GPIOD->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED12_Pos|
										3 << GPIO_OSPEEDR_OSPEED13_Pos|
										3 << GPIO_OSPEEDR_OSPEED14_Pos|
										3 << GPIO_OSPEEDR_OSPEED15_Pos;
	
	//CONFIG TIMER pwm
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	TIM4->PSC = 256 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM4->ARR = 125 - 1;//QUANTO 'TEMPO'LEVA PARA OCORRER UMA ONDA INTEIRA 
	TIM4->CR1 |= TIM_CR1_CEN;
	
	TIM4->CCMR1 |= 6 << TIM_CCMR1_OC1M_Pos|
								 6 << TIM_CCMR1_OC2M_Pos|
								 TIM_CCMR1_OC1PE|
								 TIM_CCMR1_OC2PE;
	TIM4->CCMR2 |= 6 << TIM_CCMR2_OC3M_Pos|
								 6 << TIM_CCMR2_OC4M_Pos|
								 TIM_CCMR2_OC3PE|
								 TIM_CCMR2_OC4PE;
	
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->CCR1 = 1 - 1;//tempo alto
	TIM4->CCR2 = 126 - 1;
	TIM4->CCR3 = 1 - 1;
	TIM4->CCR4 = 126 - 1;
								 
	TIM4->CCER |= TIM_CCER_CC1E|
								TIM_CCER_CC2E|
								TIM_CCER_CC2P|
								TIM_CCER_CC3E|
								TIM_CCER_CC4E|
								TIM_CCER_CC4P;
	//forçar os ccr2 e 4 para valor baixo
  
	//TIMER CONT
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	TIM7->PSC = 256 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM7->ARR = 12500 - 1;//QUANTO 'TEMPO'LEVA PARA OCORRER UMA ONDA INTEIRA 125 ms
	TIM7->CR1 |= TIM_CR1_CEN;
	TIM7->DIER |= TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM7_IRQn);
	while(1);
//	{
//		while(!(TIM7->SR & TIM_SR_UIF));
//		TIM7->SR &= ~(TIM_SR_UIF);
//		GPIOC->ODR ^= LEDT;
//		
//	}
}
void TIM7_IRQHandler(void)
{
	TIM7->SR &= ~TIM_SR_UIF;
	GPIOC->ODR ^= LEDT;
	TIM4->CCR2 = (duty[pos++] - 1);
	if(pos > 6)
	{
		pos = 0;
		TIM4->CCR2 = duty[pos++];
	}
}
void leds()
{
	
}