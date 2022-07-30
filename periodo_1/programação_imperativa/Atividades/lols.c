#include <stdio.h>
int fatorial(int N)
{  int R=1;  int i;  
	for (i=1; i<=N; i++)    
	R=R*i;    
	return R; }
	int main() 
	{  
		int Numero;  
		printf(">>> Cálculo de Fatorial <<<\n\n");  
		printf("De qual numero deseja calcular o fatorial? ");  
		scanf("%d",&Numero);  
		if (Numero<0)    
		printf("Nao existe fatorial deste numero.\n");  
		else{    
			int F;    
			F=fatorial(Numero);    
			printf("Fatorial: %d\n",F);}     
			return 0; }

