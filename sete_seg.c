#include "stm32f4xx.h"                  // Device header

#define A GPIO_ODR_ODR_0 
#define B GPIO_ODR_ODR_1
#define C GPIO_ODR_ODR_2
#define D GPIO_ODR_ODR_3
#define E GPIO_ODR_ODR_4
#define F GPIO_ODR_ODR_5
#define G GPIO_ODR_ODR_6
#define DIS1 GPIO_ODR_ODR_8
#define DIS2 GPIO_ODR_ODR_9


uint8_t num[10] = {0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF}; 

int main(){
	uint32_t i;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//habilita o clock da porta A
	GPIOA->MODER |= GPIO_MODER_MODER0_0|//define os campos do registrador
									GPIO_MODER_MODER1_0|//como output
									GPIO_MODER_MODER2_0|
									GPIO_MODER_MODER3_0|
									GPIO_MODER_MODER4_0|
									GPIO_MODER_MODER5_0|
									GPIO_MODER_MODER6_0|
									GPIO_MODER_MODER8_0|
	                GPIO_MODER_MODER9_0;
	
	
	GPIOA->ODR |= B;
	GPIOA->ODR &= ~A;
	
	RCC->APB1ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->PSC = 5000 - 1;
	TIM1->ARR = 3200 - 1;
	TIM1->CR1 |= TIM_CR1_CEN;

	//x=DIS1;
//	GPIOA->ODR |= ~DIS2;
	while(1){
		//GPIOA->ODR &= ~A;
		while(!(TIM1->SR & TIM_SR_UIF));
		TIM1->SR &= ~TIM_SR_UIF;
		GPIOA->ODR ^= B;
			
//				for(i=0;i<10;i++)
////					{				
//						for(x=0;x<333;x++)
//							{							
//								GPIOA->ODR = 0xFF;//apaga os leds 
//								for(j=0;j<1000;j++);//desliga led 90%  do tempo
//								GPIOA->ODR &= ~(num[2]);//acende os leds usando o array de num
//								for(j=0;j<9000;j++);//acende 10% do tempo
//								GPIOA->ODR ^= DIS1 | DIS2;			
//							}
//					}
			
	}
}

