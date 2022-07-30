#include <stdio.h>
#include <math.h>
int main ()
{
		double n, cont, X=0;
	scanf("%lf", &n);
	for(cont=1;cont <=n; cont++)
	{
		X= pow(cont,2)*1/pow(n,3) + X;
		printf("parcela: %lf\n", X);
	}
	printf("resultado: %lf\n", X);
	return 0;
	}
