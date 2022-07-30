#include <stdio.h>
int main ()
{
int  unidade, dezena, centena, Unidade_de_Milhar, Dezena_de_Milhar, dezena1, centena1, Umilhar, Numero;
char  Alternativa;

printf(" Digite um numero de 0 a 99.999: ");
scanf("%d", &Numero);

Dezena_de_Milhar = Numero / 10000;
Umilhar = Numero % 10000;
Unidade_de_Milhar = Umilhar /1000;
centena1 = Umilhar % 1000;
centena = centena1 / 100;
dezena1 = centena1 %100;
dezena = dezena1 /10;
unidade = dezena1 % 10;

printf("Escolha uma das alternativas abaixo:\n");
printf(" U - Unidade\n");
printf(" D - Dezena\n");
printf(" C - Centena\n");
printf(" M - Unidade de Milhar\n");
printf(" Z - Dezena de Milhar\n");
printf("alternativa: ");
scanf("  %c",  &Alternativa);

switch  
(Alternativa)
{
case 'u':	
case  'U':  printf("Unidade: %d  \n", unidade);  break;
case 'd':
case  'D':  printf("Dezena: %d \n", dezena);  break;
case 'c':
case  'C':  printf("Centena: %d \n",centena);  break;
case 'm':
case  'M':  printf("Unidade de Milhar: %d  \n", Unidade_de_Milhar);  break;
case 'z':
case  'Z':  printf("Dezena de Milhar: %d  \n", Dezena_de_Milhar);  break;
default:  printf("Alternativa invalida  \n");  
}

return 0;
}



 
