// Padrão para entrada e saída
#include <stdio.h>
// Padrão de tipos por tamanho
#include <stdint.h>

// Função vetorial
uint64_t fatorial(uint32_t n) {
    // resultado
    uint64_t r = 1;

    // Interações de 2 -> n
    for (uint32_t i = 2; i <= n; ++i)
      // Multiplicação de resultado por i
      r = r * i;
    
    return r;
}

// Função principal
int main(int argc, char *argv[])
{
    uint64_t (*pf)(uint32_t) = NULL;

    pf  = &fatorial;

    printf("Fatorial(5) = %lu\n", (*pf)(5));
    return 0;
}
