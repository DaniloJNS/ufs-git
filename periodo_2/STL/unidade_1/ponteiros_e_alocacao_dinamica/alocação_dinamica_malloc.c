// Padrão para entrada e saída
#include <stdio.h>
// Padrão de tipos por tamanho
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint32_t *vetor  = NULL;
    
    vetor =(uint32_t *)(malloc(100 * sizeof(uint32_t)));

    // Checagem da alocação
    if (vetor == NULL) 
      printf("Falha na alocação!\n");
    else 
      printf("Sucesso na alocação!\n");

    // Liberando espaço da memoria
    free(vetor);
    return 0;
}
