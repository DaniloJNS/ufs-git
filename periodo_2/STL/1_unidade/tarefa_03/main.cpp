/* Danilo José Nascimento da Silva */
#include "ChainedHashTable.h"
#include "array.h"
#include <iostream>
#include <string>

using namespace ods;
using namespace std;

int main() {
  int m, N;
  string command, data;
  cin >> m;
  cin >> N;

  ChainedHashTable<string> strHash = ChainedHashTable<string>(m);

  while (N--) {
    cin >> command >> data;
    switch (command[0]) {
    case 'a':
      strHash.add(data);
      break;
    case 'c':
      strHash.check(stoi(data));
      break;
    case 'd':
      strHash.remove(data);
      break;
    case 'f':
      string res = strHash.find(data) == data ? "yes" : "no";
      std::cout << res << std::endl;
      break;
    }
  }
}
