 // Tipos ineiros de tamanho fixo
 #include <stdint.h>
 // Macro de delay em ms (8 MHz)
 #define delay_ms(X)\
      asm volatile(\
      "mov r1, %[counter]\n\t"
      "loop:\n\t"\
      "   subs r1, r1, 1\n\t"\
      "   bne loop"\
      :\
      : [counter] "r"(X * 4000)\
      : "r1"\
      )
 // Definição de habilitação de GPIO C
 #define GPOC_EN   (4)
 // Definições do pino 13 do GPIO C
 #define PC13_CNF  (22)
 #define PC13_MODE (20)
 #define PC13_ODR  (13)
 // Reset and Clock Control (RCC)
 volatile uint32_t* const RCC_APB2ENR = (volatile uint32_t*)(0x040011004);
 // Porta C
 volatile uint32_t* const GPIOCODH = (volatile uint32_t*)(0x40011004);
 volatile uint32_t* const GPIOCODR = (volatile uint32_t*)(0x4001100C);
// Função principal
int main() {
    // Habilitando clock do GPIO C
    (*RCC_APB2ENR) |= 1 << GPIOC_EN;
    // Ajustando PC13 como saída (2 MHz)
    // Laço infinito
    while (1)
    {
        // Invertendo valor do pino PC13 por 1 segundo
        (*GPIOCODR) ^= (1 << PC13_ODR);
        delay_ms(1000);
    }
    
}