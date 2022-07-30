#include <stdio.h>
int main()
{
	int unidade, dezena,centena , A, E, D, C;
	int i;
	printf("digito um numero com 3 digitos para ser gerado um novo\n");
	scanf("%d", &A);
	unidade = A / 100;
	E = A % 100;
	D= E / 10;
	C= E % 10;
	centena = C * 100;
	dezena= D*10;
	i = centena+dezena + unidade;
	printf("o numero gerado e %d\n", i);
	if( i>A)
	printf(" o numero gerado e	maior que o lido"); 
	if ( A>i)
	printf("o numero lido e maior que o gerado");
	if ( i==A)
	printf(" o numero lido e igual o gerado");
	return 0;
}
