// Padrão para entrada e saída
#include <stdio.h>
// Padrão de tipos por tamanho
#include <stdint.h>

// Função principal
int main(int argc, char *argv[])
{
    int a = 10;

    int * pa;
    
    pa = &a;

    int **ppa;

    ppa = &pa;

    // Imprimindo parâmetros da main
    printf("main:(%p -> %d)\n", &a, a);
    printf("main:(%p -> %d)\n", pa, *pa);
    printf("main:(%p -> %d)\n", *ppa, **ppa);

    **ppa = 30;
    printf("\n\n");
    // Imprimindo parâmetros atualizados da main
    printf("main:(                                    %p -> %d)\n", &a, a);
    printf("main:(                  %p -> %p -> %d)\n",&pa, pa, *pa);
    printf("main:(%p -> %p -> %p -> %d)\n",&ppa, ppa, *ppa, **ppa);
    // returno do main zero
    return 0;
}
