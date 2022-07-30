#include <stdio.h>
int main ()
{ int A, B, C, D, Sum;
	do
	{ 
	scanf("%d", &A);
	scanf("%d", &B);
	if (A>=1 && B>=1)
	{
	if(A>=B)
	{	
		D=B;
		C=A;
	}
	if(B>=A)
	{
		D=A;
		C=B;
	}
	Sum=0;
	do
	{
	printf("%d ", D);
	Sum=Sum+D;
	D++;
	}
	while(D<=C);
	printf("Sum=%d\n", Sum);
}
}
while(A>=1 && B>=1);
	return 0;
}
