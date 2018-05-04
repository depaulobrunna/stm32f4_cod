#include "stm32f4xx.h" 
uint32_t x ; 
uint32_t i,j;

#define VM GPIO_ODR_OD12


// Device header
int main(void){
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= GPIO_MODER_MODER12_0|  //define a função dos 
										GPIO_MODER_MODER13_0|  //campos do registrador
										GPIO_MODER_MODER14_0|  // como saida
										GPIO_MODER_MODER15_0;
	GPIOD->ODR |= GPIO_ODR_OD12|  //seta como 1111 os campos do registrador
								GPIO_ODR_OD13|
								GPIO_ODR_OD14|
								GPIO_ODR_OD15;
	
	x = (GPIO_ODR_OD12|
			GPIO_ODR_OD13|
			GPIO_ODR_OD14|
			GPIO_ODR_OD15);
// faz um & do registrador com seus campos tornando tdos 1111
	while(1){
	
	for(i=0;i<16;i++){
		//GPIOD->ODR &= ~(x);
		//GPIOD->ODR |= i<<GPIO_ODR_OD12_Pos ;
		GPIOD->ODR |= (GPIOD->ODR &= ~x) | (i<<GPIO_ODR_OD12_Pos) ; 
		for(j=0;j<150000;j++);//DELAY
		}
	
	}
	
}
