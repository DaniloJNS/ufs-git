#include <stdio.h>
int main ()
{
	int N, A;
	scanf("%d", &N);
	if (N<=10000)
	{
		for(A=1; A<=10000; A++)
		{
			if(A%N==2)
			printf("%d\n", A);
		}
	}
	return 0;
}
	
