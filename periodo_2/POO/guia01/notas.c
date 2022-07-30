/* Aluno: Danilo José Nascimento da Silva */

// Nota final: (U1*p1 + U2*p2 + U3*p3) / (p1+p2+p3)
// U1:Guias de Laboratório da Unidade 1 (20%)
// U2:Guias de Laboratório da Unidade 2 (20%)
// PF: Apresentação Projeto Final (60%)

#include <stdio.h>
#include <string.h>

typedef struct notas{
  float notaU1;
  float notaU2;
  float notaP;
}notas;

typedef struct Aluno {
  char nome[20];
  notas notas[3];
  float notaF;
  float (*notaFinal)(struct Aluno *);
} Aluno; 


float notaFinal(struct Aluno *);

Aluno novoAluno(char nome[20], float notaU1[3], float notaU2[3], float notaP[3]) {
  Aluno umAluno;
  strcpy(umAluno.nome, nome);
  for (int i = 0; i < 3; ++i) {
    umAluno.notas[i].notaU1 = notaU1[i];
    umAluno.notas[i].notaU2 = notaU2[i];
    umAluno.notas[i].notaP = notaP[i];
  }
  umAluno.notaF = 0;
  umAluno.notaFinal = notaFinal;
  return umAluno;
}

float notaUnidade(struct Aluno *umAluno, int unidade) {
  return umAluno->notas[unidade].notaU1 * 0.2 + umAluno->notas[unidade].notaU2 * 0.2 + umAluno->notas[unidade].notaP * 0.6;
}

float notaFinal(struct Aluno *umAluno) {
  if(umAluno->notaF == 0) {
    for (int i = 0; i < 3; ++i) 
      umAluno->notaF += notaUnidade(umAluno, i);
    umAluno->notaF /= 3;
  }
    return umAluno->notaF;
}

int main() {
  Aluno umAluno;
  // Dados do Aluno
  float notasU1[3] = {1.2,1.3,1.4};// nota da primeira guia de laboratorio de cada unidade
  float notaU2[3] = {2,2,2};// nota da segunda guia de laboratorio de cada unidade
  float notaP[3] = {5,5,5};// nota do projeto de cada unidade
  char nome[20] = "danilo";

  umAluno = novoAluno(nome, notasU1, notaU2, notaP);

  printf("Aluno: %s - nota final: %2.2f \n",umAluno.nome, umAluno.notaFinal(&umAluno));

  return 0;
}
