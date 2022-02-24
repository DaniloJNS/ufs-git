// Padrão para entrada e saída
#include <stdio.h>
// Padrão de tipos por tamanho
#include <stdint.h>
#include <stdlib.h>

// Estrutura exemplo
typedef struct exemplo {
    // nome
    const char* nome;
    // Idade
    uint8_t idade;
} exemplo;

int main(int argc, char *argv[])
{
    exemplo a = { "Estrutura de Dados", 20 };

    printf("sizeof(a) = %lu\n", sizeof(a)); // 16
    printf("sizeof(a.nome) = %lu\n", sizeof(a.nome)); // 9
    printf("sizeof(a.idade) = %lu\n", sizeof(a.idade)); // 1

    return 0;
}
