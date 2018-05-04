#include "stm32f4xx.h" 

#define GR GPIO_ODR_OD12
#define OR GPIO_ODR_OD13
#define RD GPIO_ODR_OD14
#define BL GPIO_ODR_OD15

int main(void){
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= GPIO_MODER_MODER12_0|   
										GPIO_MODER_MODER13_0|  
										GPIO_MODER_MODER14_0|  
										GPIO_MODER_MODER15_0;
	
GPIOD ->ODR |= (GR|OR|RD|BL);

while(1){
	
	
	

}

}