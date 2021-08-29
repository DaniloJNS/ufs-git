/* Rotação de Array
1. Escreva um programa para rotacionar um array em d elementos. Para o array [1,
2, 3, 4, 5, 6, 7] e d=2, por exemplo, a saída deve ser [3, 4, 5, 6, 7, 1, 2].
2. Efetue diversas execuções, variando-se a quantidade de elementos do array e
guardando o resultado em um arquivo
3. Gere um gráfico que mostre o tempo de processamento com o aumento do número
de elementos */

#include <iostream>
using namespace std;

/*Função que rotaciona à esquerda em 1*/
void leftRotatebyOne(int arr[], int n) {
  int temp = arr[0];
  for (int i = 0; i < n; ++i)
    arr[i] = arr[i + 1];
  arr[n - 1] = temp;
}

/*FunçÃo que rotaciona à esquerda em d posições: chama a anterior*/
void leftRotate(int arr[], int d, int n) {
  int temp[d];
  for (int i = 0; i < d; ++i)
    temp[i] = arr[i];
  for (int i = d; i < n; ++i)
    arr[i - d] = arr[i];
  for (int i = n - d; i < n; ++i)
    arr[i] = temp[i - n + d];
}

/* Função utilitária para imprimir o array */
void printArray(int arr[], int n) {
  for (int i = 0; i < n; i++)
    cout << arr[i] << " ";
}

/* Programa principal para testar as funções anteriores */
int main() {
  int arr[] = {1, 2, 3, 4, 5, 6, 7};
  int n = sizeof(arr) / sizeof(arr[0]);

  // Function calling
  /* leftRotatebyOne(arr, n); */
  leftRotate(arr, 2, n);
  printArray(arr, n);

  return 0;
}
