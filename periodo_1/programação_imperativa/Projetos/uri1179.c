#include <stdio.h>
int main()
{
	int	V[5], U[5], cont=-1, i=-1, j, N, t;
	for(t=1; t<=15; t++)
	{
	scanf("%d", &N);
	if(N%2==0)
	{
		cont++;
		V[cont]=N;
		if(cont==4 || t==15)
		{
			for(j=0; j<=cont; j++)
			printf("par[%d] = %d\n", j, V[j]);
			cont=-1;
		}
	}
	else
	{
		i++;
		U[i]=N;
		if(i==4 || t==15)
		{
			for(j=0; j<=i; j++)
			printf("impar[%d] = %d\n", j, U[j]);
			i=-1;
		}
	}
}
	return 0; 
}	
