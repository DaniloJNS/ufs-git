#include <stdio.h>
int main ()
{
	int cont=0, i=1, A=8;
		for(cont=0; cont<=3; cont++)
		{	
		if (cont >=3 && i<=7)
		{
		i=i+2;
		A=A+5;
		cont=0;
		}
		if(cont <=2)
		{
		A=A-1;
		printf("I=%d J=%d\n", i, A);
	}
		}
	return 0;
}
