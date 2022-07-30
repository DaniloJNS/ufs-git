#include <stdio.h>

int main() {
  float s = 200;
  int n = 5;
  float j = 1;

  float p = s;

  for (int i = 0; i <= n; ++i) {
    printf("O valor da parcela %d eh %3.2f \n", i, p);
    p = p + (p * (j / 100));
  }
  return 0;
}
