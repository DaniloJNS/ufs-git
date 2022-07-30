#include <stdio.h>
int main ()
{
	int A, B, i,C=0;
	scanf("%d", &A);
	scanf("%d", &B);
	if(A>B)
	{
	for(i=B; i<=A; i++)
	{
		if(i%13!=0)
		C=C+i;
	}
	printf("%d\n", C);
	}
		if(B>A)
	{
	for(i=A; i<=B; i++)
	{
		if(i%13!=0)
		C=C+i;
	}
	printf("%d\n", C);
	}

return 0;
}
