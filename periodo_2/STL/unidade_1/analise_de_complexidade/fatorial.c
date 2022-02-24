#include <stdio.h>
#include <stdint.h>

int fatorial(uint32_t n) {
    if(n == 0) 
      return 1;
    return n * fatorial(n -1);
}

int main(int argc, char *argv[])
{
    printf("fatorial(%d) = %d", 5, fatorial(5));   
    return 0;
}
