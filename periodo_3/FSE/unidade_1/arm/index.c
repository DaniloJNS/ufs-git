// Tipos inteiros de tamanho fixo
#include <stdint.h>
// Define valor de true
#define true 1
// Marcro de delay em ms (8 MHz)
#define delay_ms(X)\
    asm volatile(\
    "mov r1, %[counter]\n\t"\
    "loop:\n\t"\
    "   subs r1, r1, 1\n\t"\
    "   bne loop"\
    :\
    : [counter]  "r1"(X * 4000)\
    : "r1"\
    );
// Definição dos registradores de GPIO A/C
#define GPIOA_MOD  (0x40020000)
#define GPIOA_TYP  (0x40020004)
#define GPIOA_SPD  (0x40020008)
#define GPIOA_PUP  (0x4002000C)
#define GPIOA_ODR  (0x4002000C)
// Definições do pino 13 de GPIO C
#define PA3_MOD   (26)
#define PA3_TYP   (13)
#define PA3_SPD   (26)
#define PA3_PUP   (26)
#define PA3_ODR   (13)
// Definição do registrador de RCC para  GPIO
#define	RCC_AHB1ENR	 (0x40023830)

#define RCC_APB1ENR  (0x40023840)
// Definição de campos RCC
#define GPIOC_EN   (0)
// Reset and Clock Control (RCC)
volatile uint32_t* const RCC = (volatile uint32_t*)(RCC_AHB1ENR);
// Porta A
// define in/out direction ---> output pin 3 -> 10 in pos [26, 27]
volatile uint32_t* const PA_MOD = (volatile uint32_t*)(PA3_MOD); 
// configure output type to open_dain
volatile uint32_t* const PA_TYP = (volatile uint32_t*)(PA3_TYP); 
// define speed in output
volatile uint32_t* const PA_SPD = (volatile uint32_t*)(PA3_SPD);
// configure pull_up/pull_down 
volatile uint32_t* const PA_PUP = (volatile uint32_t*)(PA3_PUP);
// define bit output value
volatile uint32_t* const PA_ODR = (volatile uint32_t*)(PA3_ODR);
// Configurar pinos
void cofigurar_pinos() {
    // Habilitando clock do GPIO C
    (*RCC) |= 1 << GPIOC_EN;
    // Ajustando PC13 como saída (2 MHz)
    (*PA_MOD) &= ~(0b11 << PA3_MOD);
    (*PA_MOD) |=  (0b01 << PA3_MOD);
    (*PA_TYP) &= ~(   1 << PA3_TYP);
    (*PA_SPD) &= ~(0b11 << PA3_SPD);
    (*PA_PUP) &= ~(0b11 << PA3_PUP);
}
// Função Principal int main()
int main() {
    cofigurar_pinos();
    // Laço infinto
    /* while (true) { */
        (*PA_ODR) ^= (1 << PA3_ODR);
        (*PA_ODR) ^= (1 << PA3_ODR);
        (*PA_ODR) ^= (1 << PA3_ODR);
        (*PA_ODR) ^= (1 << PA3_ODR);
        /* delay_ms(1000) */
    /* } */
    return 0;
}
