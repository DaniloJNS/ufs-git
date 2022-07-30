#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main ()
{
	char S[21];
	gets(S);
	 S[0]=toupper(S[0]); 
	printf("%d %s", strlen(S), S);
	return 0;
}

