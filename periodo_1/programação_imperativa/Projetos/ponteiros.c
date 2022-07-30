#include <stdio.h>
#include <stdlib.h>

 int main(){ 
	 struct Tipo{      
		 int Letra;      
		 int Ok;    };   
		 typedef struct Tipo TipoVD;   
		 TipoVD *P;
  printf("Letra: ");  
  P = (TipoVD *) malloc(sizeof(TipoVD));  
  for(int i=0; i<=3; i++){
  scanf(" %d",&P->Letra);  
  P->Ok=10;     
  printf("Letra: %d\n",P->Letra);  
  printf("Letra: %d\n",P->Ok); }
    for(int i=0; i<=3; i++){
     printf("%d Letra: %d\n",i,P->Letra);  
  printf("%d Letra: %d\n",i,P->Ok);}
  free(P);  
  P = NULL;  
  return 0;}
