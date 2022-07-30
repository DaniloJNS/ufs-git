// Tipos inteiros de tamanho fixo
#include <stdint.h>
// Define valor de true
#define true 1
// Macro for variables IO
#define __IO volatile
// Base address defition for peripherals
#define PERIPH_BASE         (0x40000000U)
// Declaração de topo da pilha e de funções
extern char _estack;
int main();
void tim2_int();
void usart1_int();
// Tabela de vetor de interrupção
uint32_t (* const vector_table[]) __attribute__ ((section(".text.vector_table"))) = {
        (uint32_t*)(&_estack),	                        // Topo da pilha
        (uint32_t*)(main),              		// Reset
        0,						// NMI
        0,						// Hard fault
        0,						// Memory management fault
        0,						// Bus fault
        0,						// Usage fault
        0, 0,     				        // Reservado
        0,						// SVCall
        (uint32_t*)(&_estack),				// Debug monitor
        0,						// Reservado
        0,						// PendSV
        0,                   	                        // SysTick
        0,						// Window Watchdog interrupt
        0,						// PVD through EXTI Line detection interrupt
        0,						// Tamper interrupt
        0,						// RTC global interrupt
        0,						// Flash global interrupt
        0,						// RCC global interrupt
        0,		                                // EXTI Line0 interrupt
        0,						// EXTI Line1 interrupt
        0,						// EXTI Line2 interrupt
        0,						// EXTI Line3 interrupt
        0,						// EXTI Line4 interrupt
        0, 0, 0, 0, 0, 0, 0,                          	// DMA Stream global interrupt
        0,					        // ADC1, ADC2 and ADC3 global interrupt
        0, 						// CAN1 TX interrupt
        0, 						// CAN1 RX0 interrupt
        0, 						// CAN1 RX1 interrupt
        0, 						// CAN1 SCE interrupt
        0, 						// EXTI line [9:5] interrupts
        0, 						// TIM1 Break interrupt and TIM9 global interrupt
        0, 						// TIM1 Update interrupt and TIM10 global interrupt
        0, 						// TIM1 Trigger and Commutation interrupts and TIM11 global interrupt
        0, 						// TIM1 Capture Compare interrupt
        (uint32_t*)(tim2_int),                          // TIM2 global interrupt
        0, 						// TIM3 global interrupt
        0, 						// TIM4 global interrupt
        0, 						// I2C1 event interrupt
        0, 						// I2C1 error interrupt
        0, 						// I2C2 event interrupt
        0, 						// I2C2 error interrupt
        0, 						// SPI1 global interrupt
        0, 						// SPI2 global interrupt
        (uint32_t*)(usart1_int),	        	// USART1 global interrupt
};
// Mapping address
#define MMIO32(addr)        (__IO uint32_t *)(addr)
// Definição dos registradores de GPIO A/C
typedef struct _GPIO_TypeDef{
  __IO uint32_t MODER;        /*!< GPIO port mode register,                Address offset: 0x00      */
  __IO uint32_t OTYPER;       /*!< GPIO port output type register,         Address offset: 0x04      */
  __IO uint32_t OSPEEDR;      /*!< GPIO port output speed register,        Address offset: 0x08      */
  __IO uint32_t PUPDR;        /*!< GPIO port pull-up/pull-down register,   Address offset: 0x0C      */
  __IO uint32_t IDR;          /*!< GPIO port input data register,          Address offset: 0x10      */
  __IO uint32_t ODR;          /*!< GPIO port output data register,         Address offset: 0x14      */
  __IO uint32_t BSRR;         /*!< GPIO port bit set/reset register,       Address offset: 0x1A      */
  __IO uint32_t LCKR;         /*!< GPIO port configuration lock register,  Address offset: 0x1C      */
  __IO uint32_t AFR[2];       /*!< GPIO alternate function registers,      Address offset: 0x20-0x24 */
  __IO uint32_t BRR;          /*!< GPIO bit reset register,                Address offset: 0x28      */
} GPIO_TypeDef;
/*! \struct _USART_TypeDef 
 *  \brief Brief struct description
 *
 *  Detailed description
 */
typedef struct _USART_TypeDef {
    __IO uint32_t SR;        /*!< USART status register,                    Address offset: 0x00      */
    __IO uint32_t DR;        /*!< USART data register,                      Address offset: 0x04      */
    __IO uint32_t BR;        /*!< USART baud rate register,                 Address offset: 0x08      */
    __IO uint32_t CR[3];     /*!< USART control register,                   Address offset: 0x0C-0x14 */
    __IO uint32_t GTPR;      /*!< USART guard time and prescaler register   Address offset: 0x18      */
} USART_TypeDef;

typedef struct _TIMER_TypeDef {
    __IO uint32_t CR[2];      /*!< TIMER control register,                   Address offset: 0x00-0x04 */
    __IO uint32_t SMCR;       /*!< TIMER slave controler register,           Address offset: 0x08      */
    __IO uint32_t DIER;       /*!< TIMER DMA/interrup enable resgister,      Address offset: 0x0C      */
    __IO uint32_t SR;         /*!< TIMER status register,                    Address offset: 0x10      */
    __IO uint32_t EGR;        /*!< TIMER event generation register,          Address offset: 0x14      */
    __IO uint32_t CCMR[2];    /*!< TIMER capture/compare mode register 1,    Address offset: 0x18      */
    __IO uint32_t CCER;       /*!< TIMER capture/compare enable resgister 2, Address offset: 0x20      */
    __IO uint32_t CNT;        /*!< TIMER counter,                            Address offset: 0x24      */
    __IO uint32_t PSC;        /*!< TIMER prescaler,                          Address offset: 0x28      */
    __IO uint32_t ARR;        /*!< TIMER auto-load register,                 Address offset: 0x30      */
    __IO uint32_t CCR[4];     /*!< TIMER capture/compare register 1,         Address offset: 0x34      */
    __IO uint32_t DCR;        /*!< TIMER DMA control register,               Address offset: 0x48      */
    __IO uint32_t DMAR;       /*!< TIMER DMA address full tranfer,           Address offset: 0x4C      */
    __IO uint32_t OR;         /*!< TIMER option tranfer,                     Address offset: 0x4C      */
} TIMER_TypeDef;

typedef struct _NVIC_TypeDef {
    __IO uint32_t ISER[2];    /*!< NVIC interrupt set-enable registers,      Address offset: 0x00-0xB  */
} NVIC_TypeDef;
// address for registers
#define	NVIC_ISER0	    (0xE000E100)
#define GPIOA_BASE          (PERIPH_BASE + 0x20000)
#define USART1_BASE         (PERIPH_BASE + 0x11000)
#define TIMER3_BASE         (PERIPH_BASE)
// define registers
#define GPIOA               (GPIO_TypeDef  *)(GPIOA_BASE)
#define USART1              (USART_TypeDef *)(USART1_BASE)
#define TIMER3              (TIMER_TypeDef *)(TIMER3_BASE)
#define NVIC                (NVIC_TypeDef  *)(NVIC_ISER0)
// Definições do pino 13 de GPIO A
#define PA3_MOD   (6)
#define PA3_TYP   (3)
#define PA3_SPD   (6)
#define PA3_PUP   (6)
#define PA3_ODR   (3)
#define PA3_AFL   (12)
// Fields defition for USART
#define	USART1_RXNE			(5)
#define	USART1_TXE			(7)
#define	USART1_UE			(13) // enable USART
#define	USART1_TE			(3)  // enable transmission
#define	USART1_RE			(2)  // enable receive
#define	USART1_RXNEIE			(5)  // RXNE interrupt  enable
// Fiealds definition for TIMER
#define TIMER_CKD                       (7)  // define counter direction (up(1)/down(0))
#define TIMER_COUNTER                   (0)  // counter enable
#define TIMER_UIE                       (0)  // enable interrupt update
// Fields definition for NVIC
#define NVIC_TIMER1                     (28)
#define NVIC_USART1                     (5)
// Definition address base for RCC
#define RCC_BASE          (PERIPH_BASE + 0x23800)
// Definição do registrador de RCC para  GPIO
#define	AHB1ENR	          MMIO32(RCC_BASE + 0x30) 
// Definição do registrador de RCC para  USART
#define APB1ENR           MMIO32(RCC_BASE + 0x40)
// Definição de campos RCC
#define GPIOC_EN   (0)
#define USART1_EN  (4)

// Configurar pinos
void pin_setup() {
    // Habilitando clock do GPIO C
    (*AHB1ENR ) |= 1 << GPIOC_EN;
    (*APB1ENR ) |= 1 << USART1_EN;
    // Ajustando PC13 como saída (2 MHz)
    (*GPIOA).MODER       &= ~(0b100 << PA3_MOD);
    (*GPIOA).MODER       |=  (0b01 << PA3_MOD);
    (*GPIOA).OTYPER      &= ~(   1 << PA3_TYP);
    (*GPIOA).OSPEEDR     &= ~(0b11 << PA3_SPD);
    (*GPIOA).ODR         &= ~(0b11 << PA3_PUP);
    (*GPIOA).AFR[0]      &= ~(0b1111 << PA3_AFL);
    (*GPIOA).AFR[0]      &= ~(0b1000 << PA3_AFL);
}
// Configure timer3
void timer_setup() {
    // define arr to buffered
    (*TIMER3).CR[0] |= (1 << TIMER_CKD); 
    // active counter
    (*TIMER3).CR[0] |= (1 << TIMER_COUNTER); 
    // define prescaler -> clk / 1000
    (*TIMER3).PSC   |= 0x3E8;
    // define auto-reload 
    (*TIMER3).ARR   |= 0xF4240;
    // Enable interrupt update
    (*TIMER3).DIER  |= (1 << TIMER_UIE);
}
// Configure registers for alternate function USART
void usart1_setup() {
    (*USART1).CR[0] = (1 << USART1_UE);
    // Ajustando baud rate para 9600 bps (52,083 @ 8 MHz)
    (*USART1).BR = 0x341;
    // Habilitando recepção, transmissão e interrupção
    (*USART1).CR[0] |= (1 << USART1_RE) | (1 << USART1_TE) | (1 << USART1_RXNEIE);
}

void nvic_setup() {
    // Enable timer1 interrupt
    (*NVIC).ISER[0] |= (1 << NVIC_TIMER1);
    // Enable USART1 interrupt
    (*NVIC).ISER[1] |= (1 << NVIC_USART1);
}
void enviar_USART1(uint8_t dado) {
        // Escrevendo dado
        (*USART1).DR = dado;
        // Esperando envio do dado
        /* while(!((*USART1).SR & (1 << USART1_TXE))); */
}
void print_serial(char* string) {
        // Índice da string
        uint32_t i = 0;
        // Repete até encontrar caractere nulo
        while(string[i]) {
                // Enviando caractere via USART1
                enviar_USART1(string[i++]);
        }
}
uint8_t receber_USART1() {
        // Aguardando recebimento do dado
        while(!((*USART1).SR & (1 << USART1_RXNE)));
        // Retornando dado
        return (*USART1).DR;
}
// Versão simplificada do scanf
void scan_serial(char* string) {
        // Índice da string
        uint32_t i = 0;
        // Recebendo dado da USART1
        do string[i] = receber_USART1();
        // Repete até receber quebra de linha
        while(string[i++] != '\n');
        // Inserindo caractere nulo
        string[i] = 0;
}

void tim2_int(void) {
    print_serial("S\n");
}
char texto[100] = { 0 };

void usart1_int(void) {
    scan_serial(texto);
    /* print_serial("V\n"); */
}
// Função Principal
int main() {
    usart1_setup();
    timer_setup();
    nvic_setup();
    while (true) {
        /* scan_serial(texto); */
        /* print_serial(texto); */
    }
    return 0;
}
