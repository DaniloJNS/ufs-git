#include <iostream>
#include <stdlib.h>

using namespace std;
class lampada {
  typedef struct rede_eletrica {
    bool estado;
    int tensao;
  } rede_eletrica;

private:
  int voltagem;
  rede_eletrica conecção;
  bool funcionando;

public:
  lampada();
  lampada(int v);

  void ligar_desligar();
  void ligar_desligar(int tensao);

  bool get_estado() { return conecção.estado; };
  int get_voltagem() { return voltagem; };
  void set_voltagem(int Voltagem) { voltagem = Voltagem; }
  void imprime();
};

lampada::lampada() {
  voltagem = 110;
  conecção.estado = false;
  conecção.tensao = 0;
  funcionando = true;
}

lampada::lampada(int v) {
  if (v == 110 || v == 220)
    voltagem = v;
  else
    voltagem = 110;
  conecção.estado = false;
  conecção.tensao = 0;
  funcionando = true;
}
void lampada::ligar_desligar() { conecção.estado = false; }

void lampada::ligar_desligar(int tensao) {
  if (tensao > voltagem) {
    voltagem = 0;
    funcionando = false;
  }
  if (funcionando && tensao > 0)
    conecção.estado = true;
  conecção.tensao = tensao;
}
void lampada::imprime() {
  string estado_str =
      conecção.estado ? "a lampada está ligada" : "a lampada está desligada";
  string funcionando_str =
      funcionando ? "a lampada está funcionando" : "a lampada está queimada";
  std::cout << estado_str << std::endl;
  std::cout << "voltagem = " << voltagem << std::endl;
  std::cout << funcionando_str << std::endl;
}
void options() {
  std::cout << "Opções: " << std::endl;
  std::cout << "1 - ligar ou desligar lampada" << std::endl;
  std::cout << "2 - informar que está queimada" << std::endl;
}
int main() {
  lampada sala = lampada(), cozinha = lampada(), escritorio = lampada();
  int option = -1;
  sala.ligar_desligar(110);
  cozinha.ligar_desligar();
  escritorio.ligar_desligar();
  while (option != 0) {
    std::cout << "Gerenciador de lampada" << std::endl;

    std::cout << "1 - cozinha" << std::endl;
    std::cout << "2 - sala " << std::endl;
    std::cout << "3 - escritorio" << std::endl;

    std::cout << "Escolha: ";
    cin >> option;

    switch (option) {
    case 1:
      std::cout << "Estado da lampada : " << std::endl;
      sala.imprime();
      options();
      break;
    case 2:
      std::cout << "Estado da lampada : " << std::endl;
      sala.imprime();
      options();
      break;
    case 3:
      std::cout << "Estado da lampada : " << std::endl;
      escritorio.imprime();
      options();
    }
  }
