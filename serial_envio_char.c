#include "stm32f4xx.h"                  // Device header

uint32_t i;
int main(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//HABILITA O CLOCK DA PORTA A
		GPIOA->MODER |= GPIO_MODER_MODER2_1;// PA2 TX OUTPUT
    GPIOA-> AFR[0] |= 7 << 8;//seta a porta A como alternate function
		GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0;//seta como pull up
		GPIOA->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED2_Pos ; // velocidade como 50MHz
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2;

		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;//HABILITA O CLOCK DA USART
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE;//HABILITA A USART & HABILITA TRANSMISSAO DA USART
//    USART2->CR2 |= USART_CR2_CLKEN;//seta o clock da usart (mas nao precisou)
		USART2->BRR = SystemCoreClock/115200;//seta o baud rate
	
		char nome[] = "Brunna\n";
		char *ch = &nome[0];
		
	
	while(1)
	{
		while(*ch)
			{
				USART2->DR = *ch++;//onde a usart recebe o dado
				while(!(USART2->SR & USART_SR_TC));//espera a flag DE TRANSMISSAO
			}
			ch = &nome[0];
	}
		
}
	
		
    
