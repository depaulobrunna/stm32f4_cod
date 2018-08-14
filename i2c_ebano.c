#include "stm32l0xx.h"

//#define MMA_8653_ADDR (uint8_t) 0x1D // ACELERÔMETRO
#define MMA_8653_ADDR (uint8_t) 0xA0 // MEMÓRIA
#define WHO_I_AM_ADDR (uint8_t) 0x0D
#define WHO_I_AM_EVAL (uint8_t) 0x5A

char *sys_msg[] = {"System initialized.\n",
                                     "Starting comunication loop.\n",
                                     "End of comunitation loop.\n",
                                     "Start Condition Error Loop.\n",
                                   "Stop Condition Error Loop.\n",
                                     "\nCorrect WHO I AM!!!.",
                                     "\nIncorrect WHO I AM!!!."};
char *com_msg[] = {"Start Condition Generated.\n",
                                     "Device address in bus.\n",
                                     "Received address acknoledge.\n",
                                     "Register address to be written in the bus.\n",
                                     "Value to be written in the bus.\n",
                                     "Received register read acknoledge.\n"};
char *flg_msg[] = {"\nTransmission Complete Flag Set.",
                                     "\nTransmission Flag Reset.",
                                     "\nStop Flag Set.",
                                     "\nStop Flag Reset.",
                                     "\nNot Acknoledge Flag Set.",
                                     "\nNot Acknoledge Flag Reset.\n",
                                     "\nReception Flag Set.",
                                     "\nReception Flag Reset.",};
char *teste[] = {"Teste 1.\n","Teste 2.\n"};
                                     
void I2C1_IRQHandler(void);
void SERIAL_TX(char *chr);
void MEM_WRITE(uint8_t MEM_ADDR, uint8_t MEM_REG, uint8_t ADDR_VALUE);
void MEM_READ(uint8_t MEM_ADDR, uint8_t MEM_REG);

char dado;
uint32_t i = 100000;
                             
// PINO 8 COMO FUNÇÃO EM MODO ALTERNATIVA NÃO GERA START CONDITION!!!
// PINO 8 COMO FUNÇÃO ALERNATIVA 4 É SCL, LOGO SE NÃO ASSOCIA À FUNÇÃO 4
// NÃO GERA CLOCK E, PORTANTO, NÃO GERA START CONDITION
int main()
{   
    // CONFIGURA GPIO DA USART
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN; // HABILITA CLOCK PARA PORTA A
    GPIOA->MODER &=  ~GPIO_MODER_MODE9_0; // PINOS RX/TX
    GPIOA->AFR[1] |= 4<<4; // SELECIONA FUNÇÃO ALTERNATIVA PARA PINOS RX/TX
    // CONFIGURA USART
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // HABILITA CLOCK PARA A USART1
    USART1->BRR = SystemCoreClock/115200; // BAUDRATE DE 115200
    USART1->CR1 |= USART_CR1_TE|USART_CR1_UE; // HABITA TRANSMISSÃO E USART1 E INTERRUPÇÃO DE RECEPÇÃO
    //    CONFIGURA GPIO DA I2C1
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0); // GERA START CONDITION
    GPIOB->OTYPER |= GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9;
    GPIOB->OSPEEDR |= 3<<GPIO_OSPEEDER_OSPEED8_Pos | 3<<GPIO_OSPEEDER_OSPEED9_Pos;
    GPIOB->PUPDR |= 1<<GPIO_PUPDR_PUPD8_Pos | 1<<GPIO_PUPDR_PUPD9_Pos;
    GPIOB->AFR[1] |= 4<<GPIO_AFRH_AFRH0_Pos | 4<<GPIO_AFRH_AFRH1_Pos;
    // CONFIGURA I2C
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC->CCIPR |= 1<<RCC_CCIPR_I2C1SEL_Pos; // SYSCLK COMO FONTE DE CLOCK
    I2C1->TIMINGR = 0x00000509; // 100kHz, 100ns rise time, 10ns fall time, analog filter off
    I2C1->CR1 |= I2C_CR1_STOPIE|I2C_CR1_NACKIE|I2C_CR1_RXIE;//|I2C_CR1_ERRIE;
    I2C1->CR1 |= I2C_CR1_PE|1<<I2C_CR1_ANFOFF_Pos;
    // HABILITA INTERRUPÇÃO DA I2C
    NVIC_EnableIRQ(I2C1_IRQn);
    // ENVIO DE MENSAGEM DE INICIALIZAÇÃO
    SERIAL_TX(sys_msg[0]);
    // INICIALIZA TRANSMISSÃO DE ENDEREÇO DO SLAVE VIA I2C
    while(1)
    {
        MEM_WRITE(MMA_8653_ADDR,0x00,WHO_I_AM_EVAL);
        MEM_READ(MMA_8653_ADDR,0x00);
        if(dado == WHO_I_AM_EVAL)
        {
            SERIAL_TX(sys_msg[5]);
        }
        else
        {
            SERIAL_TX(sys_msg[6]);
        }
        while(1);
//        i = 1000000;
//        while(i--);
    }
}

void I2C1_IRQHandler(void)
{
    if(I2C1->ISR & I2C_ISR_RXNE)
    {
        SERIAL_TX(flg_msg[6]);
        dado = (uint8_t) I2C1->RXDR;
        SERIAL_TX(flg_msg[7]);
        USART1->TDR = dado;
//        while(!(USART1->ISR & USART_ISR_TC));
//        USART1->ISR &= ~USART_ISR_TC;
    }
   
    if(I2C1->ISR & I2C_ISR_TC)
    {
        SERIAL_TX(flg_msg[0]);
        while(I2C1->ISR & I2C_ISR_TC)
        {
            I2C1->ISR &= ~I2C_ISR_TC;
        }
        SERIAL_TX(flg_msg[1]);
    }
   
    if(I2C1->ISR & I2C_ISR_NACKF)
    {
        SERIAL_TX(flg_msg[4]);
        while(I2C1->ISR & I2C_ISR_NACKF)
        {
            while(I2C1->ISR & I2C_ISR_STOPF)
            {
                SERIAL_TX(flg_msg[2]);
                while((I2C1->ISR & I2C_ISR_STOPF))
                {
                    I2C1->ICR |= I2C_ICR_STOPCF;
                }
                SERIAL_TX(flg_msg[3]);
            }
            I2C1->ICR |= I2C_ICR_NACKCF;
            SERIAL_TX(flg_msg[5]);
            }
    }
   
    if(I2C1->ISR & I2C_ISR_STOPF)
    {
        SERIAL_TX(flg_msg[2]);
        while((I2C1->ISR & I2C_ISR_STOPF))
        {
            I2C1->ICR |= I2C_ICR_STOPCF;
        }
        SERIAL_TX(flg_msg[3]);
    }
}

void SERIAL_TX(char *chr)
{
    while(*chr)
    {
        USART1->TDR = *chr++;
        while(!(USART1->ISR & USART_ISR_TC));
        USART1->ISR &= ~USART_ISR_TC;
    }
}

void MEM_WRITE(uint8_t MEM_ADDR, uint8_t MEM_REG, uint8_t ADDR_VALUE)
{
    // ESCRITA
        SERIAL_TX(sys_msg[1]);
        I2C1->CR2 &= 0; // LIMPA REGISTRADOR
//        while((I2C1->ISR & (I2C_ISR_BUSY|~I2C_ISR_TXE))); // ESPERA LIBERAR BARRAMENTO E TRANSFERÊNCIA
        I2C1->CR2 |=     1<<I2C_CR2_AUTOEND_Pos| // STOP CONDITION AUTOMÁTICO APÓS A TRANSF. DE NBYTES
                                    2<<I2C_CR2_NBYTES_Pos| // NÚMERO DE BYTES A SEREM TRANSFERIDOS
                                    MEM_ADDR<<(I2C_CR2_SADD_Pos)| // ENDEREÇO DO DISPOSITIVO
                                    0<<I2C_CR2_RD_WRN_Pos| // REQUISITA ESCRITA
                                    1<<I2C_CR2_START_Pos; // START CONDITION
        while(!(I2C1->CR2 & I2C_CR2_START))
        {
            SERIAL_TX(sys_msg[3]);
            i = 100000;
            while(i--);
            I2C1->CR2 |= I2C_CR2_START;
        }
        SERIAL_TX(com_msg[0]);
       
        SERIAL_TX(com_msg[1]);
        if(!(I2C1->ISR & I2C_ISR_NACKF))
        {
            SERIAL_TX(com_msg[2]);
        }
        while(!(I2C1->ISR & I2C_ISR_TXIS)); // ESPERA LIBERAÇÃO DA ESCRITA DO DADO A SER ENVIADO
        I2C1->TXDR = MEM_REG; // ENDEREÇO DE MEMÓRIA A SER SALVO
        SERIAL_TX(com_msg[3]);

        while(!(I2C1->ISR & I2C_ISR_TXIS)); // ESPERA LIBERAÇÃO DA ESCRITA DO DADO A SER ENVIADO
        I2C1->TXDR = WHO_I_AM_EVAL; // DADO A SER SALVO NO ENDEREÇO ENVIADO
        SERIAL_TX(com_msg[4]);
       
        if(!(I2C1->ISR & I2C_ISR_NACKF))
        {
            SERIAL_TX(com_msg[4]);
        }
}

void MEM_READ(uint8_t MEM_ADDR, uint8_t MEM_REG)
{
    // LEITURA
        I2C1->CR2 &= 0; // LIMPA REGISTRADOR
//        while((I2C1->ISR & (I2C_ISR_BUSY|~I2C_ISR_TXE))); // ESPERA LIBERAR BARRAMENTO E TRANSFERÊNCIA
        I2C1->CR2 |=     0<<I2C_CR2_AUTOEND_Pos| // STOP CONDITION AUTOMÁTICO APÓS A TRANSF. DE NBYTES
                                    1<<I2C_CR2_NBYTES_Pos| // NÚMERO DE BYTES A SEREM TRANSFERIDOS
                                    MEM_ADDR<<(I2C_CR2_SADD_Pos)| // ENDEREÇO DO DISPOSITIVO
                                    0<<I2C_CR2_RD_WRN_Pos| // REQUISITA ESCRITA
                                    1<<I2C_CR2_START_Pos; // START CONDITION
        while(!(I2C1->CR2 & I2C_CR2_START))
        {
            SERIAL_TX(sys_msg[3]);
            i = 100000;
            while(i--);
            I2C1->CR2 |= I2C_CR2_START;
        }
        SERIAL_TX(com_msg[0]);
       
        SERIAL_TX(com_msg[1]);
        if(!(I2C1->ISR & I2C_ISR_NACKF))
        {
            SERIAL_TX(com_msg[2]);
        }
       
        while(!(I2C1->ISR & I2C_ISR_TXIS)); // ESPERA LIBERAÇÃO DA ESCRITA DO DADO A SER ENVIADO
        I2C1->TXDR = 0x00; // ENDEREÇO DE MEMÓRIA A SER SALVO
        SERIAL_TX(com_msg[3]);

        I2C1->CR2 &= 0; // LIMPA REGISTRADOR
//        while((I2C1->ISR & (I2C_ISR_BUSY|~I2C_ISR_TXE))); // ESPERA LIBERAR BARRAMENTO E TRANSFERÊNCIA
        I2C1->CR2 |=     1<<I2C_CR2_AUTOEND_Pos| // STOP CONDITION AUTOMÁTICO APÓS A TRANSF. DE NBYTES
                                    1<<I2C_CR2_NBYTES_Pos| // NÚMERO DE BYTES A SEREM TRANSFERIDOS
                                    MEM_ADDR<<(I2C_CR2_SADD_Pos)| // ENDEREÇO DO DISPOSITIVO
                                    1<<I2C_CR2_RD_WRN_Pos| // REQUISITA LEITURA
                                    1<<I2C_CR2_START_Pos; // START CONDITION
        while(!(I2C1->CR2 & I2C_CR2_START))
        {
            SERIAL_TX(sys_msg[3]);
            i = 100000;
            while(i--);
            I2C1->CR2 |= I2C_CR2_START;
        }
        SERIAL_TX(com_msg[0]);
       
        SERIAL_TX(com_msg[1]);
        if(!(I2C1->ISR & I2C_ISR_NACKF))
        {
            SERIAL_TX(com_msg[2]);
        }
}