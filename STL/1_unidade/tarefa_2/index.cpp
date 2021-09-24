/* Danilo José Nascimento da Silva */

#include "SLList.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int main() {
  ods::SLList<int> numList1, numList2;
  string entrada1, entrada2, saida;
  int numero1, numero2, resultado = 0;
  char aux[2];

  cin >> entrada1 >> entrada2;

  for (size_t i = 0; i < entrada1.size(); ++i) {
    aux[0] = entrada1[i];
    numList1.push(atoi(aux));
  }
  for (size_t i = 0; i < entrada2.size(); ++i) {
    aux[0] = entrada2[i];
    numList2.push(atoi(aux));
  }

  while (numList1.size() > 0 || numList2.size() > 0) {
    numero1 = numList1.size() > 0 ? numList1.remove() : 0;
    numero2 = numList2.size() > 0 ? numList2.remove() : 0;
    resultado += numero1 + numero2;
    saida = to_string(resultado).back() + saida;
    resultado /= 10;
  }

  if (resultado > 0)
    saida = to_string(resultado) + saida;

  cout << saida << endl;
  return 0;
}
/*
    O código que eu desenvolvir para resolver este problema consistir
   basicamente, de trẽs estruturas de repetição, sendo que, cada uma
   tem complexidade linear (O(n)) e as operações nas listas ultiliza-
   das dentro dessas estruturas são constatante, podemos concluir que
   este códigode complexidade predominante linear.

   Melhor caso e Pior caso:
      como a quantidade de operações necessarias para resolver o problema
    depende unicamente da tamanho da entrada, como o codigo tem completxiade
    linear o tempo de execução aumenta proporcionalmente a medida que a
    entrada cresce.
    Logo,
    Ω(n) = O(n) => Θ(n)
 )


*/
