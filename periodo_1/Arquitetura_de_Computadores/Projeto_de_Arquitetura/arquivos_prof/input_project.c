#include <stdio.h>
#include <ctype.h> /* devido ao isprint() */
#include <stdlib.h> /* devido ao exit() */
#define TAMANHO 128
char buf [TAMANHO];
void display (int numlido);
int main( int argc, char** argv ){
 FILE *fp;
 int setor, numlido;
 /* verifica num de argumentos */
 if (argc != 2)
              {
              printf("use: dump nome do arquivo\n");
              exit(1);
              }
 if ((fp=fopen(argv[1], "rb")) == NULL)
              {
              printf("o arquivo nao pode ser aberto \n");
              exit (1);
              }
 for(;;)
    {
    printf("informe o setor (-1 para terminar): ");
    scanf("%ld", &setor);
    if(setor < 0) break;
    if(fseek(fp, setor*TAMANHO, SEEK_SET))
       printf("erro de busca\n");
    if((numlido=fread(buf, 1, TAMANHO, fp)) != TAMANHO)
                               printf("EOF encontrado \n");
    display(numlido);
    }
 return 0;
}
/* funcao para exibir arquivo  */
void display(int numlido)
{
   int i, j;
   for(i=0; i< numlido/16; i++)
      {
       for (j=0; j<16; j++) printf("%3X", buf [i*16+j]);
       printf("|"); 

      for (j=0; j<16; j++)
         {
           if (isprint (buf [i*16 + j])) printf("%c",buf[i*16+j]);
           else printf(".");
         }
         
      printf("\n");
   }
}



