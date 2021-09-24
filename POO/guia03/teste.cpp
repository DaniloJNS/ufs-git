#include <iostream>
#include <string>
using namespace std;
class Teste {
public:
  string nome;
  Teste(){};
  void setNome(string nome) { this->nome = nome; }

private:
  /* data */
};

using namespace std;

int main(int argc, char const *argv[]) {
  Teste teste;
  string nome = "danilo";
  teste.nome = "carlos";
  teste.setNome(nome);
  cout << teste.nome << endl;
  return 0;
}
