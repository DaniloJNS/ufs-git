// Padrão para entrada e saída
#include <stdio.h>
// Padrão de tipos por tamanho
#include <stdint.h>

// Função f com referencia protegida
/* Beneficios: 
 *              Conseguir ler o conteúdo da variavel
 *              acessando-o diretamente na memoria sem 
 *              precisar copia-la em outra variavel
 *              localmente
 */
void f(const uint32_t* x) {
    // Imprimindo o parâmetro x
  printf("f: x = %u\n", *x);

  // Modificando o parâmetro x
  /* *x = 1; // Erro de assignment */
}

// Função principal
int main(int argc, char *argv[]) {
    // Inteiro sem sinal x inicializado com 11
    uint32_t x  = 7;

    // Chamando a função f com parâmetro x
    f(&x);
    // Imprimindo o valor de x
    printf("main: x = %u ", x);

    // returno do main zero
    return 0;
}
