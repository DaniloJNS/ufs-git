#include <stdio.h>

float es;
int en;
float ej;
int corrente;
float p;

void novoEmprestimo(float s, int n, float j) {
  es = s;
  en = n;
  ej = j;
  corrente = 1;
  p = s;
}

float proximaParcela() {
  float retorno = p;
  corrente++;

  if (corrente <= en)
    p = p + (p * (ej / 100));
  else
    p = 0; // não há mais parcelas

  return retorno;
}

int main() {
  novoEmprestimo(200, 5, 1);

  int i = 1;
  float p = proximaParcela();
  while (p > 0) {
    printf("O valor da parcela %d eh %3.2f \n", i, p);
    i++;
    p = proximaParcela();
  }
  return 0;
}
