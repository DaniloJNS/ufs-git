// Padrão para entrada e saída
#include <stdio.h>
// Padrão de tipos por tamanho
#include <stdint.h>

// Função principal
int main(int argc, char *argv[]) {
    // args -> argv   
    char**args = argv;

    // pargs -> args -> argv
    char*** pargs = &args;

    // Imprimindo parâmetros da main
    printf("main:(%i, %p)\n", argc, argv);
    printf("main:(%i, %p)\n", argc, args);
    printf("main:(%i, %p)\n", argc, (*pargs));

    // returno do main zero
    return 0;
}
