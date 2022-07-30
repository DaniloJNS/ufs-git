#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <stdlib.h> 
int main(){  
	char Nome[21], Sobrenome[21];  
	printf("Nome: ");  
	gets(Nome);  
	printf("Sobrenome: ");  
	gets(Sobrenome); 
	int L;  
	for (L=1; L<strlen(Nome); L++)    
	Nome[L]=tolower(Nome[L]);   
	Nome[0]=toupper(Nome[0]);  
	for (L=0; L<strlen(Sobrenome); L++)    
	Sobrenome[L]=toupper(Sobrenome[L]);
	char Resultado[41]="";  
	strcat(Resultado,Sobrenome);  
	strcat(Resultado,", ");  
	strcat(Resultado,Nome);
	printf("Resultado: %s.\n",Resultado); 
	return 0;
	}
