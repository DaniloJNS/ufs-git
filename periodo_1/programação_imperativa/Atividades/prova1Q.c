#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main()
{
	char Q[10],P[10],A[3]={" ,"},C[10];
	int i=0,j;
	gets(Q);
do
{
	fflush(stdin);
	if(i==0)
	P[i]=toupper(Q[i]);
	if(i>0)
	{
		P[i]= tolower(Q[i]);
	}
	i++;
	}
	while(Q[i]!=' ');
	for(j=i+1;j<=strlen(Q);j++)
	C[i]=toupper(Q[i]);
	strcat(C,A);
	strcat(C,P);
	printf("%s", C);
	return 0;
}
