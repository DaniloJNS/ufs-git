#include <stdio.h>

float proximaParcela(float s, int n, float j, int *corrente, float *p) {
  if (*corrente == 0) // Primeira Parcela
    *p = s;
  else if (*corrente <= n)
    *p = *p + (*p * (j / 100));
  else
    *p = 0; // não há mais parcelas

  (*corrente)++;
  return *p;
}

int main() {
  float s = 200;
  int n = 5;
  float j = 1;

  int corrente = 0; // Controla parcela corrente
  float p = s;      // controla valor da parcela corrrente

  for (int i = 0; i <= n; i++) {
    printf("O valor da parcela %d eh %3.2f \n", i, p);
    p = proximaParcela(s, n, j, &corrente, &p);
  }
  return 0;
}
