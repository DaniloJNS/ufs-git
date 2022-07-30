// Tipos inteiros de tamanho fixo
#include <stdint.h>
// Definição dos registradores de GPIO A/C
#define	GPIOA_CRL			(0x40010800)
#define	GPIOA_CRH			(0x40010804)
#define	GPIOA_ODR			(0x4001080C)
#define	GPIOC_CRH			(0x40011004)
#define	GPIOC_ODR			(0x4001100C)
// Definição de campos de GPIO A/C
#define	PA0_CNF				(2)
#define	PA0_MODE			(0)
#define	PA9_CNF				(6)
#define	PA9_MODE			(4)
#define	PA10_CNF			(10)
#define	PA10_MODE			(8)
#define	PC13_CNF			(22)
#define	PC13_MODE			(20)
#define	PC13_ODR			(13)
// Definição do registrador de RCC
#define	RCC_APB2ENR			(0x40021018)
// Definição de campos de RCC
#define	RCC_AFIO_EN  		(0)
#define	RCC_GPIOA_EN  		(2)
#define	RCC_GPIOC_EN  		(4)
#define	RCC_USART1_EN  		(14)
// Definição de registradores de SysTick
#define SYST_CSR  			(0xE000E010)
#define SYST_RVR  			(0xE000E014)
// Definição de campos de SYST_CSR
#define	SYST_CSR_CLKSOURCE	(2)
#define	SYST_CSR_TICKINT	(1)
#define	SYST_CSR_ENABLE		(0)
// Definição de registradores de AFIO, EXTI e NVIC
#define	AFIO_EXTICR1		(0x40010008)
#define	EXTI_IMR			(0x40010400)
#define	EXTI_FTSR			(0x4001040C)
#define	EXTI_PR				(0x40010414)
#define	NVIC_ISER0			(0xE000E100)
#define	NVIC_IPR			(0xE000E400)
// Definição de registradores de USART1
#define	USART1_SR			(0x40013800)
#define	USART1_DR			(0x40013804)
#define	USART1_BRR			(0x40013808)
#define	USART1_CR1			(0x4001380C)
#define	USART1_CR2			(0x40013810)
// Definiçaão de campos da USART1
#define	USART1_RXNE			(5)
#define	USART1_TXE			(7)
#define	USART1_UE			(13)
#define	USART1_TE			(3)
#define	USART1_RE			(2)
// Declaração de topo da pilha e de funções
extern char _estack;
int main();
void SysTick();
void botao();
// Tabela de vetor de interrupção
uint32_t (* const vector_table[]) __attribute__ ((section(".text.vector_table"))) = {
	(uint32_t*)(&_estack),	// Topo da pilha
	(uint32_t*)(main),		// Reset
	0,						// NMI
	0,						// Hard fault
	0,						// Memory management fault
	0,						// Bus fault
	0,						// Usage fault
	0, 0, 0, 0,				// Reservado
	0,						// SVCall
	0,						// Debug monitor
	0,						// Reservado
	0,						// PendSV
	(uint32_t*)(SysTick),	// SysTick
	0,						// Window Watchdog interrupt
	0,						// PVD through EXTI Line detection interrupt
	0,						// Tamper interrupt
	0,						// RTC global interrupt
	0,						// Flash global interrupt
	0,						// RCC global interrupt
	(uint32_t*)(botao),		// EXTI Line0 interrupt
	0,						// EXTI Line1 interrupt
	0,						// EXTI Line2 interrupt
	0,						// EXTI Line3 interrupt
	0,						// EXTI Line4 interrupt
};
// Porta A
volatile uint32_t* const PA_CRL = (volatile uint32_t*)(GPIOA_CRL);
volatile uint32_t* const PA_CRH = (volatile uint32_t*)(GPIOA_CRH);
volatile uint32_t* const PA_ODR = (volatile uint32_t*)(GPIOA_ODR);
// Porta C
volatile uint32_t* const PC_CRH = (volatile uint32_t*)(GPIOC_CRH);
volatile uint32_t* const PC_ODR = (volatile uint32_t*)(GPIOC_ODR);
// Reset and Clock Control (RCC)
volatile uint32_t* const APB2ENR = (volatile uint32_t*)(RCC_APB2ENR);
// SysTick
volatile uint32_t* const CSR = (volatile uint32_t*)(SYST_CSR);
volatile uint32_t* const RVR = (volatile uint32_t*)(SYST_RVR);
// AFIO
volatile uint32_t* const EXTICR1 = (volatile uint32_t*)(AFIO_EXTICR1);
// EXTI
volatile uint32_t* const IMR = (volatile uint32_t*)(EXTI_IMR);
volatile uint32_t* const FTSR = (volatile uint32_t*)(EXTI_FTSR);
volatile uint32_t* const PR = (volatile uint32_t*)(EXTI_PR);
// NVIC
volatile uint32_t* const ISER0 = (volatile uint32_t*)(NVIC_ISER0);
volatile uint8_t* const IPR = (volatile uint8_t*)(NVIC_IPR);
// USART1
volatile uint32_t* const U1SR = (volatile uint32_t*)(USART1_SR);
volatile uint32_t* const U1DR = (volatile uint32_t*)(USART1_DR);
volatile uint32_t* const U1BRR = (volatile uint32_t*)(USART1_BRR);
volatile uint32_t* const U1CR1 = (volatile uint32_t*)(USART1_CR1);
volatile uint32_t* const U1CR2 = (volatile uint32_t*)(USART1_CR2);
// Declaração de função print_serial
void print_serial(char* string);
// Função SysTick
void SysTick() {
	// Invertendo valor do pino PC13 (LED)
	(*PC_ODR) ^= (1 << PC13_ODR);
}

// Função de botão
void botao() {
	// Enviando mensagem
	print_serial("BOTAO!\n");
	// Desabilitando SysTick
	(*CSR) &= ~(1 << SYST_CSR_ENABLE);
	// Checando se valor de recarga é 8000000
	if((*RVR) == 8000000) {
		// Ajustando para 800000
		(*RVR) = 800000;
	}
	// Valor de recarga é 800000
	else {
		// Ajustando para 8000000
		(*RVR) = 8000000;
	}
	// Habilitando SysTick
	(*CSR) |= (1 << SYST_CSR_ENABLE);
	// Checando registrador de pendência
	if((*PR) & 1) {
		// Limpando pendência no bit 0
		(*PR) = 1;
	}
}
// Configurar pinos
void configurar_pinos() {
	// Habilitando clock do AFIO, GPIO A/C e USART1
	(*APB2ENR) |= (1<< RCC_AFIO_EN) | (1 << RCC_GPIOA_EN) | (1 << RCC_GPIOC_EN) | (1 << RCC_USART1_EN);
	// Ajustando PA0 como entrada
	(*PA_CRL) &= ~((0b11 << PA0_CNF) | (0b11 << PA0_MODE));
	(*PA_CRL) |= (0b10 << PA0_CNF);
	// Ajustando PA9 (TX) e PA10 (RX) como função alternativa
	(*PA_CRH) &= ~((0b11 << PA9_CNF) | (0b11 << PA9_MODE) | (0b11 << PA10_CNF) | (0b11 << PA10_MODE));
	(*PA_CRH) |= (0b10 << PA9_CNF) | (0b10 << PA9_MODE) | (0b10 << PA10_CNF);
	// Ajustando PC13 como saída (2 MHz)
	(*PC_CRH) &= ~((0b11 << PC13_CNF) | (0b00 << PC13_MODE));
	(*PC_CRH) |= (0b10 << PC13_MODE);
}
// Configurar SysTick
void configurar_SysTick() {
	// Configuração do SysTick para 1 segundo (8 Mhz)
	(*RVR) = 8000000;
	// Habilitando SysTick com clock interno e interrupção
	(*CSR) |= (1 << SYST_CSR_CLKSOURCE) | (1 << SYST_CSR_TICKINT) | (1 << SYST_CSR_ENABLE);
}
// Configurar pino PA0 para interrupção em EXTI0
void configurar_PA0_EXTI0() {
	// EXTI0 <-> GPIO A
	(*EXTICR1) &= 0xFFF0;
	// Interrupção por gatilho de borda negativa
	(*FTSR) |= 1;
	// Desativar mascaramento
	(*IMR) |= 1;
	// Habilitando interrupção
	(*ISER0) |= (1 << 6);
	// Ajustando prioridade para nível 13
	IPR[6] = 0xD0;
}
// Configurar USART1
void configurar_USART1() {
	// Habilitando USART (1 start bit + 8 bits de dados + 1 stop bit)
	(*U1CR1) |= (1 << USART1_UE);
	// Ajustando baud rate para 9600 bps (52,083 @ 8 MHz)
	(*U1BRR) = 0x341;
	// Habilitando recepção e transmissão
	(*U1CR1) |= (1 << USART1_RE) | (1 << USART1_TE);
}
// Enviar dado pela USART1
void enviar_USART1(uint8_t dado) {
	// Escrevendo dado
	(*U1DR) = dado;
	// Esperando envio do dado
	while(!((*U1SR) & (1 << USART1_TXE)));
}
// Receber dado pela USART1
uint8_t receber_USART1() {
	// Aguardando recebimento do dado
	while(!((*U1SR) & (1 << USART1_RXNE)));
	// Retornando dado
	return (*U1DR);
}
// Versão simplificada do printf
void print_serial(char* string) {
	// Índice da string
	uint32_t i = 0;
	// Repete até encontrar caractere nulo
	while(string[i]) {
		// Enviando caractere via USART1
		enviar_USART1(string[i++]);
	}
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
// Função principal
int main() {
	// Caracteres de texto
	char texto[100];
	// Configuração de pinos, SysTick, interrupção
	configurar_pinos();
	configurar_SysTick();
	configurar_PA0_EXTI0();
	configurar_USART1();
	// Mensagem de inicialização
	print_serial("RX2TX4EVER\n");
	// Laço infinito
	while(1) {
		// Recebendo e enviando dados
		scan_serial(texto);
		print_serial(texto);
	}
}
