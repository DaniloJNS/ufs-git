#include <iostream>
class Avaliacao {
public:
  Avaliacao(double u1_guialab1, double u1_guialab2, double u2_guialab1,
            double u2_guialab2, double projetoFinal)
      : u1_guialab1(u1_guialab1), u1_guialab2(u1_guialab2),
        u2_guialab1(u2_guialab1), u2_guialab2(u2_guialab2),
        projetoFinal(projetoFinal), p1(0.2), p2(0.2), p3(0.6){};
  /* virtual ~Avaliacao(); */
  void NotaFinal() {
    notaFinal = (unidade1 * p1 + unidade2 * p2 + projetoFinal) / (p1 + p2 + p3);
  }
  void MostrarNotaFinal() { std::cout << notaFinal << std::endl; }

private:
  double u1_guialab1;
  double u1_guialab2;
  double u2_guialab1;
  double u2_guialab2;
  double notaFinal;
  double projetoFinal;
  double unidade1;
  double unidade2;
  double p1;
  double p2;
  double p3;
  void AvaliaUnidade1() { unidade1 = (u1_guialab1 + u1_guialab2) / 2; }
  void AvaliaUnidade2() { unidade2 = (u2_guialab1 + u2_guialab2) / 2; }
};
int main() {
  Avaliacao avaliaAluno(6, 4, 5, 10, 6);

  avaliaAluno.NotaFinal();
  avaliaAluno.MostrarNotaFinal();
}
