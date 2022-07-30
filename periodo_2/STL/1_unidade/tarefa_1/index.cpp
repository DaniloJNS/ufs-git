/* Aluno: Danilo José Nascimento da Silva */
#include "ArrayStack.h"
#include <iostream>

using namespace std;

int main() {
  int testes, tamanhoNumero, entrada, temp;
  bool verifica;
  ods::ArrayStack<int> numeros, maiorProximo, maiorProximoAux;
  cin >> testes;

  while (testes--) {
    verifica = true;
    cin >> tamanhoNumero;

    while (tamanhoNumero--) {
      cin >> entrada;
      numeros.push(entrada);
    }

    temp = numeros.pop();

    while (numeros.size() > 0) {

      if (temp > numeros.top() && verifica) {

        while (maiorProximo.size() > 0) {
          if (numeros.top() > maiorProximo.top())
            maiorProximoAux.push(maiorProximo.pop());
          else {
            maiorProximo.push(numeros.pop());
            break;
          }
        }

        if (maiorProximo.size() == 0)
          maiorProximo.push(numeros.pop());

        while (maiorProximoAux.size() > 0)
          maiorProximo.push(maiorProximoAux.pop());

        maiorProximo.push(temp);
        verifica = false;
      } else
        maiorProximo.push(temp);

      if (numeros.size() > 0)
        temp = numeros.pop();
    }

    maiorProximo.push(temp);

    if (verifica)
      cout << -1 << endl;
    else
      while (maiorProximo.size() > 0)
        cout << maiorProximo.pop();
    cout << endl;

    numeros.clear();
    maiorProximo.clear();
    maiorProximoAux.clear();
  }
}
/* 
 pior caso: n
 melhor caso: n²

*/
