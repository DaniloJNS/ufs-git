#include <stdio.h>
int main ()
{
	int A, cont, U;
	long long int  fib[61];
	scanf("%d", &A);
	fib[1]= 1;
	fib[0]= 0;
		for(cont=2; cont<=60; cont ++)
	{
		fib[cont] = fib[cont-2] + fib[cont-1];
		printf("%lld fibonCI\n", fib[cont]);
	}
		for(cont=1; cont<=A; cont++)
	{
		scanf("%d", &U);
		printf("Fib(%d) = %lld\n", U, fib[U]);
	}
	return 0;
}
		
