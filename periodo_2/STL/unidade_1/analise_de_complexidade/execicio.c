#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void exemplo(uint32_t n) {
    int a[] = (int*)(malloc((n*n) * sizeof(int)));
    for (int i = 0; i < 10; ++i)
        a[i] = 1;
    for (int i = 0; i < n; ++i) {
        int b = 3;
        for (int j = 0; j < n; j++) {
            a[i][j] = b * a[i][j];
            for (int k = 0; k < 10; ++k) {
                a[i][k] = a[i][k] * k;
            }
        }
    }
    for (int i = n; i < n * n; i++)
        a[i] = a[i] + 2;
}
