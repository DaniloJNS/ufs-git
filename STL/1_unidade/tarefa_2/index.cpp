#include "SLList.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main() {
  string numero1, numero2;
  cin >> numero1 >> numero2;
  ods::SLList<int> num1, num2;
  for (size_t i = 0; i < numero2.size(); ++i) {
    num1.push(atoi(numero1));
  }
  return 0;
}
