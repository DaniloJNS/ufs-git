#include <stdio.h>
int main()
{
	float N[100];
	int U, i;
	scanf("%d", &U);
	N[0]=U;
	for(i=1;i<=99;i++)
	{
		N[i]=N[i-1]/2;
		printf("N[%d] = %.4f\n", i, N[i]);
	}
		return 0;
	}
		
