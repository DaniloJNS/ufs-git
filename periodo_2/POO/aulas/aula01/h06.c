#include <stdio.h>

typedef struct Emprestimo{
  float s;
  int n;
  float j;
  int corrente;
  float p;
  float (*proximaParcela)(struct Emprestimo*);
} Emprestimo;

float proximaParcela(struct Emprestimo *);

Emprestimo novoEmprestimo(float s, int n, float j) {
  Emprestimo umEmprestimo;
  umEmprestimo.s = s;
  umEmprestimo.n = n;
  umEmprestimo.j = j;
  umEmprestimo.corrente = 1;
  umEmprestimo.p = s;
  umEmprestimo.proximaParcela = proximaParcela;
  return umEmprestimo;
}

float proximaParcela(struct Emprestimo *umEmprestimo) {
  float retorno = umEmprestimo->p;
  if (umEmprestimo->corrente < umEmprestimo->n) 
    umEmprestimo->p += umEmprestimo->p * (umEmprestimo->j/100);
  else
    umEmprestimo->p = 0;
  
  umEmprestimo->corrente++;

  return retorno;
}

int main() {
  Emprestimo emprestimo1, emprestimo2;

  emprestimo1 = novoEmprestimo(200, 5, 1);
  emprestimo2 = novoEmprestimo(500, 7, 2);

  float p1 = emprestimo1.proximaParcela(&emprestimo1);
  float p2 = emprestimo2.proximaParcela(&emprestimo2);

  int i = 1;
  while (p1 > 0 || p2 > 0) {
    if(p1 > 0)
      printf("Emprestimo 1: parcela %d eh %3.2f \n", i, p1);
    if(p2 > 0)
      printf("Emprestimo 2: parcela %d eh %3.2f \n", i, p2);
    p1 = emprestimo1.proximaParcela(&emprestimo1);
    p2 = emprestimo2.proximaParcela(&emprestimo2);
    i++;
  }
  return 0;
}
