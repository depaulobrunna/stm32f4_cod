#include "stm32f4xx.h" 

#define GR GPIO_ODR_OD12
#define OR GPIO_ODR_OD13
#define RD GPIO_ODR_OD14
#define BL GPIO_ODR_OD15

uint32_t j;

uint8_t NUM_LED=4; 

uint8_t k;

void roda_led_hr(uint8_t num);
void pisca_led(uint8_t num);
void roda_led_ant_hr(uint8_t num);

int main(void)
{
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= GPIO_MODER_MODER12_0|   
										GPIO_MODER_MODER13_0|  
										GPIO_MODER_MODER14_0|  
										GPIO_MODER_MODER15_0;
	
	GPIOD ->ODR &= ~(GR|OR|RD|BL);
	
	while(1)
	{ 
		roda_led_hr(5);
		//pisca_led(5);
		//roda_led_ant_hr(5);
		//pisca_led(5);
	}
}

void roda_led_hr(uint8_t num)
{ 
	uint32_t i, j;
	for(k=0;k<num;k++)
		{ 
			for(i=0;i<4;)
			{
				GPIOD->ODR ^= ((1 << GPIO_ODR_OD12_Pos) << i)^(GPIOD->ODR); 
				for(j=0;j<600000;j++);
			}
		}
}

void pisca_led(uint8_t num)
{	
	uint32_t  j;
	for(k=0;k<num;k++)
		{		
			GPIOD->ODR &= ~(GR|OR|RD|BL);
			for(j=0;j<600000;j++);
			GPIOD->ODR ^= GR|OR|RD|BL;
			for(j=0;j<600000;j++);
		}
}
void roda_led_ant_hr(uint8_t num)
{ 
	uint32_t i, j, x;
	for(k=0;k<num;k++)
		{ 
//			GPIOD ->ODR &= ~(GR|OR|RD|BL);
//			GPIOD->ODR |= GR;
//			for(j=0;j<250000;j++);
//			GPIOD->ODR &= ~GR;
			i=0;
			if(i==0)
				{
					i=3;
						{	
							GPIOD->ODR ^= ((1 << GPIO_ODR_OD12_Pos) << i)^(GPIOD->ODR); 
							for(j=0;j<600000;j++);
						}
				}
			else
				{
					i--;
					GPIOD->ODR ^= ((1 << GPIO_ODR_OD12_Pos) << i)^(GPIOD->ODR); 
					for(j=0;j<600000;j++);
				}
			
		}
}

