#include <stdio.h>
int main ()
{
int cont=0, B=1, N, I=1;
scanf("%d", &N);
do
	{	
		if(cont <=2)
			{
				B = B * I;
				printf(" %d", B);
				cont++;
			}
		if (cont >=3)
			{
				printf("\n");
				I++;
				cont=0;
				B=1;
			}
	}
while(N>=I); 
return 0;
}

