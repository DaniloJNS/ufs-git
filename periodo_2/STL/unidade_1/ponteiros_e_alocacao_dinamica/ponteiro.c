// Padrão para entrada e saída
#include <stdio.h>
// Padrão de tipos por tamanho
#include <stdint.h>

// Função f
void f(uint32_t x) {
    // Imprimindo o parâmetro x
  printf("f: x = %u\n", x);

  // Modificando o parâmetro x
  x = 1;

  // Imprimindo o parâmetro x
  printf("f: x = %u\n", x);
}
// Função principal
int main(int argc, char *argv[])
{
    // Inteiro sem sinal inicializado com 7
    uint32_t x  = 7;

    // Ponteiro px inicializado como nulo
    uint32_t* px = NULL;
    uint32_t** ppx = NULL;

    // Ponteiro px recebe endereço da variável x
    px = &x;

    // Imprimindo informações
    printf("main: x = %u @ %p\n", *px, px);

    // Atualizando valor de x
    *px = 3;
    // returno do main zero
    return 0;
}
