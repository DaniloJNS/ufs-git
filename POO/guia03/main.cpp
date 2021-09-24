#include <iostream>
#include <string>

using namespace std;

class Avaliacao {
  double u1_guialab1;
  double u1_guialab2;
  double u2_guialab1;
  double u2_guialab2;
  double projetofinal;
  double p1;
  double p2;
  double p3;
  double unidade1;
  double unidade2;
  double notafinal;
  string matricula;
  string nomeAluno;

public:
  Avaliacao() {
    p1 = 0.2;
    p2 = 0.2;
    p3 = 0.6;
  }

  double AvaliacaoUnidade1() {
    unidade1 = (u1_guialab1 + u1_guialab2) / 2;
    return unidade1;
  }

  double AvaliacaoUnidade2() {
    unidade2 = (u2_guialab1 + u2_guialab2) / 2;
    return unidade2;
  }

  void NotaFinal() {
    notafinal = (AvaliacaoUnidade1() * p1 + AvaliacaoUnidade2() * p2 +
                 projetofinal * p3) /
                (p1 + p2 + p3);
  }

  void MostrarNotaFinal() { cout << notafinal << endl; }

  void EntradaNotas(double notas[]) {
    this->u1_guialab1 = notas[0];
    this->u1_guialab2 = notas[1];
    this->u2_guialab1 = notas[2];
    this->u2_guialab2 = notas[3];
    this->projetofinal = notas[4];
  }

  void setNomeAluno(string NomeAluno) { this->nomeAluno = NomeAluno; };
  void setMatricula(string Matricula) { this->matricula = Matricula; };
  double getNotaFinal() { return this->notafinal; };
};

class Turma {
  Avaliacao avaliaAluno1, avaliaAluno2, avaliaAluno3;
  double mediaTurma;
  double maior;
  double menor;
  string nomeTurma;
  string codigoTurma;

public:
  Turma(string nomeAluno1, string matriculaAluno1, string nomeAluno2,
        string matriculaAluno2, string nomeAluno3, string matriculaAluno3) {

    avaliaAluno1.setNomeAluno(nomeAluno1);
    avaliaAluno1.setMatricula(matriculaAluno1);

    avaliaAluno2.setNomeAluno(nomeAluno2);
    avaliaAluno2.setMatricula(matriculaAluno2);

    avaliaAluno2.setNomeAluno(nomeAluno3);
    avaliaAluno3.setMatricula(matriculaAluno3);
  }
  void setNomeTurma(string NomeTurma) { this->nomeTurma = NomeTurma; };
  void setCodigoTurma(string CodigoTurma) { this->codigoTurma = CodigoTurma; };
  void registraEntradaNotasDosAlunos(double notasAluno1[], double notasAluno2[],
                                     double notasAluno3[]) {
    avaliaAluno1.EntradaNotas(notasAluno1);
    avaliaAluno2.EntradaNotas(notasAluno2);
    avaliaAluno3.EntradaNotas(notasAluno3);

    avaliaAluno1.NotaFinal();
    avaliaAluno2.NotaFinal();
    avaliaAluno3.NotaFinal();
  };
  void calculaMediaDaTurma() {
    mediaTurma = avaliaAluno1.getNotaFinal() + avaliaAluno2.getNotaFinal() +
                 avaliaAluno3.getNotaFinal();
    mediaTurma /= 3;
  };
  void calculaNotaMaiorTurma() {

    double notas[2] = {avaliaAluno2.getNotaFinal(),
                       avaliaAluno3.getNotaFinal()};
    maior = avaliaAluno1.getNotaFinal();
    for (int nota : notas)
      if (nota > maior)
        maior = nota;
  }
  void calculaNotaMenorTurma() {
    double notas[2] = {avaliaAluno2.getNotaFinal(),
                       avaliaAluno3.getNotaFinal()};
    menor = avaliaAluno1.getNotaFinal();
    for (int nota : notas)
      if (nota < menor)
        menor = nota;
  };
  double getMediaDaTurma() { return mediaTurma; };
  double getNotaMaiorTurma() { return maior; };
  double getNotaMenorTurma() { return menor; };
};

int main(int argc, char const *argv[]) {
  string nomeTurma = "POO";
  string codigoTurma = "T02";

  string nomeAluno1 = "Danilo";
  string matriculaAluno1 = "20200137681";
  double notasAluno1[5] = {
      6, 7, 7, 10,
      7}; // [u1_guialab1, u1_guialab2, u2_guialab1, u2_guialab2, projetofinal]

  string nomeAluno2 = "Leonador";
  string matriculaAluno2 = "20190145129";
  double notasAluno2[5] = {4, 4, 5, 5, 7};

  string nomeAluno3 = "Caio";
  string matriculaAluno3 = "20200146354";
  double notasAluno3[5] = {2, 4, 2, 4, 2};

  Turma turmaPOO(nomeAluno1, matriculaAluno1, nomeAluno2, matriculaAluno2,
                 nomeAluno3, matriculaAluno3);

  turmaPOO.setNomeTurma(nomeTurma);
  turmaPOO.setCodigoTurma(codigoTurma);
  turmaPOO.registraEntradaNotasDosAlunos(notasAluno1, notasAluno2, notasAluno3);
  turmaPOO.calculaMediaDaTurma();
  turmaPOO.calculaNotaMaiorTurma();
  turmaPOO.calculaNotaMenorTurma();
  cout << turmaPOO.getMediaDaTurma() << endl;
  cout << turmaPOO.getNotaMaiorTurma() << endl;
  cout << turmaPOO.getNotaMenorTurma() << endl;

  return 0;
}
