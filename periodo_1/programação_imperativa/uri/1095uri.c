#include <stdio.h>
int main()
{
	int A, i,j;
	i=0;
	j=65;
	for(A=1; A<=13; A++)
	{
		i= i + 3;
		j= j - 5;
		if(i<=36)
		printf("I=%d j=%d\n", i, j);
		else
		printf("I=? J=0");
		
	}
	return 0;
}
