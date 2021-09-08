#include <iostream>
#include <string>

using namespace std;

typedef struct notas {
  float notaU1;
  float notaU2;
  float notaP;
} notas;

class Alunos {
private:
  notas notasAluno[3];
  string Nome;
  int notaF;

  float notaUnidade(int unidade) {
    return notasAluno[unidade].notaU1 * 0.2 + notasAluno[unidade].notaU2 * 0.2 +
           notasAluno[unidade].notaP * 0.6;
  }

public:
  Alunos(string nome, float notasU1[], float notasU2[], float notasP[])
      : Nome(nome) {
    for (int i = 0; i < 3; ++i) {
      notasAluno[i].notaU1 = notasU1[i];
      notasAluno[i].notaU2 = notasU2[i];
      notasAluno[i].notaP = notasP[i];
    }
    notaF = 0;
  };

  float notaFinal() {
    if (notaF == 0) {
      for (int i = 0; i < 3; ++i)
        notaF += notaUnidade(i);
      notaF /= 3;
    }
    return notaF;
  }
  string getNome() { return Nome; }
};

int main() {
  float notasU1[3] = {
      1.2, 1.3, 1.4}; // nota da primeira guia de laboratorio de cada unidade
  float notaU2[3] = {2, 2,
                     2}; // nota da segunda guia de laboratorio de cada unidade
  float notaP[3] = {5, 5, 5};
  string nome = "danilo";

  Alunos umALuno = Alunos(nome, notasU1, notaU2, notaP);
  cout << "nome: " << umALuno.getNome()
       << ", nota final: " << umALuno.notaFinal() << endl;
}
