#include <stdio.h>
#include <stdint.h>

// alias para o tipo de dado (struct elemento) -> (elemento)
typedef struct elemento elemento;

// Estrutura do elemento
struct elemento {
    // Valor
    uint32_t E;

    // Ponteiro
    elemento* P;
};

// Estrutura da lista
typedef struct lista {
    // Ponteiro
    elemento* L;
} lista;

/******************************************************************************
* Function:         insertion
* Description:      Faz a inserção de um elemento no final da lista
* Where:            -----
* Return:          as one integer
* Error:            -----
*****************************************************************************/
void insertion(elemento * lista, elemento element){
}
int main(int argc, char *argv[])
{
    
    elemento list;
    list.P;

    return 0;
}
