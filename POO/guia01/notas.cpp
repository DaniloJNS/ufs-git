// Nota final: (U1*p1 + U2*p2 + U3*p3) / (p1+p2+p3)
// U1:Guias de Laboratório da Unidade 1 (20%)
// U2:Guias de Laboratório da Unidade 2 (20%)
// PF: Apresentação Projeto Final (60%)

#include <stdio.h>
#include <string.h>
/*! \struct notas_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */
typedef struct Aluno {
  char nome[20];
  float notaU1;
  float notaU2;
  float notaP;
  float (*notaFinal)(struct Aluno *);
} Aluno; /* optional variable list */
;

float notaFinal(struct Aluno *);

Aluno novoAluno(char nome[20], float notaU1, float notaU2, float notaP) {
  Aluno umAluno;
  strcpy(umAluno.nome, nome);
  umAluno.notaU1 = notaU1;
  umAluno.notaU2 = notaU2;
  umAluno.notaP = notaP;
  return umAluno;
}

float notaFinal(struct Aluno *umAluno) {
  float nota =
      (umAluno->notaP * 0.6 + umAluno->notaU1 * 0.2 + umAluno->notaU2 * 0.2);
  return nota;
};

int main(int argc, char *argv[]) {
  Aluno umAluno;
  char nome[20] = "danilo";
  umAluno = novoAluno(nome, 5, 5, 5);
  float nota = umAluno.notaFinal(&umAluno);
  nota = 2;
  printf("%f \n", nota);
  return 0;
}
