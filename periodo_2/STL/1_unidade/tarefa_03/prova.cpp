#include <iostream>

int hashT(int x) { return (3 * x) >> (2); }
using namespace std;
int main() {
  int input;
  while (true) {
    cin >> input;
    cout << "hash: " << hashT(input) << endl;
  }
}
