#include "stm32f4xx.h"                  // Device header

char i = 'U';
int main(){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//HABILITA O CLOCK DA PORTA A
		GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;// PA2 & PA3 COMO AF
    GPIOA-> AFR[0] |= 7 << 8 | 7 << 12 ;//seta a porta A como alternate function
		GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0;//seta como pull up
		GPIOA->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED2_Pos | 3 << GPIO_OSPEEDR_OSPEED3_Pos ; // velocidade como 50MHz
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2;
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT_3;

		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;//HABILITA O CLOCK DA USART
    USART2->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;//HABILITA A USART & HABILITA TRANSMISSAO DA USART
//    USART2->CR2 |= USART_CR2_CLKEN;//seta o clock da usart (mas nao precisou)
		
		USART2->BRR = SystemCoreClock/115200;//seta o baud ratE		
	
	while(1)
	{
		while(!(USART2->SR & USART_SR_RXNE));
		USART2->SR &= ~USART_SR_RXNE;
		i = USART2->DR;
		USART2->DR = i;
		while(!(USART2->SR & USART_SR_TC));
		USART2->SR &= ~USART_SR_TC;
	}
		
}
	