#include <string>

using namespace std;

class notas {
  string Nome;
  int Nota1;
  int Nota2;

public:
  notas(string nome, int nota1, int nota2)
      : Nome(nome), Nota1(nota1), Nota2(nota2){};
  /* virtual ~notas(); */
  int get() { return Nota1; }
};
int main() { notas Aluno = notas("danilo", 5, 6); }
