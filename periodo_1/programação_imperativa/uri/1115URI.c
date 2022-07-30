

#include <stdio.h>

int main()
{int X, Y;
	do
	{
	scanf("%d", &X);
	scanf("%d", &Y);
	if(X>=1 && Y>=1)
	printf("primeiro\n");
	if(X>=1 && Y<=-1)
	printf("quarto\n");
	if(X<=-1 && Y<=-1)
	printf("terceiro\n");
	if(X<=-1 && Y>=1)
	printf("segundo\n");
}
	while(X!=0 && Y!=0);
	return 0;
}

