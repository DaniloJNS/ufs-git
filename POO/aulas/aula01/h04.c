#include <stdio.h>

typedef struct {
  float s;
  int n;
  float j;
  int corrente;
  float p;
} Emprestimo;

void novoEmprestimo(Emprestimo *umEmprestimo) {
  umEmprestimo->corrente = 1;
  umEmprestimo->p = umEmprestimo->s;

}

float proximaParcela(Emprestimo *umEmprestimo) {
  float retorno = umEmprestimo->p;
  if (umEmprestimo->corrente < umEmprestimo->n) 
    umEmprestimo->p += umEmprestimo->p * (umEmprestimo->j/100);
  else
    umEmprestimo->p = 0;
  
  umEmprestimo->corrente++;

  return retorno;
}

int main() {
  Emprestimo umEmprestimo;
  
  umEmprestimo.s = 200;
  umEmprestimo.n = 5;
  umEmprestimo.j = 1;

  novoEmprestimo(&umEmprestimo);
  int i = 1;
  float p = proximaParcela(&umEmprestimo);
  while (p > 0) {
    printf("O valor da parcela %d eh %3.2f \n", i, p);
    p = proximaParcela(&umEmprestimo);
    i++;
  }
  return 0;
}
