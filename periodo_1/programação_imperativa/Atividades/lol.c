

#include <stdio.h>

int main(int argc, char **argv)
{int A, B, C;
	printf("digite um numero\n");
	scanf("%d", &A);
	printf("digite um divisor\n");
	scanf("%d", &B);
	printf("digite outro divisor\n");
	scanf("%d", &C);
	if ( (A % B==0 || A % C==0) && ( A % B != A % C))
	printf(" atende as condições");
	else
	printf(" não atende as condições");
	
	
	
	
	return 0;
}

