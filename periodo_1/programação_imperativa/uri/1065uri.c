#include <stdio.h>
int main ()
{
	int i, N, T;
	T=0;
	for(i=1; i<=5; i++)
	{
	scanf("%d", &N);
	if(N%2==0)
	T = T + 1;
	}
	printf("%d valores pares\n", T); 
	return 0;
}
