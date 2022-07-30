#include <stdio.h> 
#include <math.h> 
int main()
{ 
	int L1, L2, L3; 
	printf("Quais os lados? "); 
	scanf("%d%d%d",&L1,&L2,&L3); 
	if ((L1<(L2+L3)) && (L2<(L1+L3)) && (L3<(L1+L2))) ;
	if (pow(L1,2)==pow(L2,2)+pow(L3,2)) 
	printf("Lado 1: %d.",L1);
else if (pow(L2,2)==pow(L1,2)+pow(L3,2)) 
printf("Lado 2: %d.",L2);
else if (pow(L3,2)==pow(L1,2)+pow(L2,2)) 
printf("Lado 3: %d.",L3);
else printf("nao atende\n");
return 0;}
