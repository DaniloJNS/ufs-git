/* Danilo José Nascimento da Silva */
#include "ChainedHashTable.h"
#include "array.h"
#include <iostream>
#include <string>
using namespace ods;
using namespace std;

void display(ods::ArrayStack<string> data) {
  std::cout << data.size() << std::endl;
  if (data.size() > 0)
    for (int i = 0; i < (int)data.size(); i++)
      std::cout << data.get(i) << std::endl;
  else
    std::cout << std::endl;
}

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
      display(strHash.check(stoi(data)));
      break;
    case 'd':
      std::cout << strHash.remove(data) << std::endl;
      break;
    case 'f':
      string res = strHash.find(data) == data ? "yes" : "no";
      std::cout << res << std::endl;
      break;
    }
  }
}
