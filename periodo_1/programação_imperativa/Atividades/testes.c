#include <stdio.h>
#include <stdlib.h> //usado malloc
#include <string.h> //usado strcpy 
#include <ctype.h>


int main (void) {
   typedef struct {
      int dia, mes, ano, d, e;
   } data;
   printf ("sizeof (data) = %d\n", 
            sizeof (data));
int valor = 27;
int *ptr;
ptr = &valor;
printf("Utilizando ponteiros\n\n");
printf ("Conteudo da variavel valor: %d\n", valor);

printf ("Conteudo da variavel ponteiro ptr: %x", ptr);
   return EXIT_SUCCESS;
}
	   
