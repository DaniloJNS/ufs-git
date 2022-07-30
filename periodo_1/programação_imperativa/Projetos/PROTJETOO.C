#include <stdio.h>
#include <ctype.h>
int main ()
{ 
	int num, dezena_de_milhar, unidade_de_milhar, umilhar, centena1, centena, dezena1, dezena, unidade;
	char alternativa;
	printf("Digite um numero de 1 a 99999\n");
	scanf("%d", &num);
	dezena_de_milhar= num / 10000;
	umilhar= num % 10000;
	unidade_de_milhar= umilhar / 1000;
	centena1= umilhar % 1000;
	centena= centena1 / 100;
	dezena1= centena % 100;
	dezena= dezena1 / 10;
	unidade= dezena1 % 10;
	printf("Escolhar uma das alternativas abaixo");
	printf(" Z - Dezena de milhar\n");
	printf(" M- Unidade de milhar\n");
	printf(" C - Centena\n");
	printf(" D - Dezena\n");
	printf(" U - Unidade\n");
	printf("Alternativa:");
	scanf(" %c", &alternativa);
	alternativa= toupper(alternativa);
	switch(alternativa)
	{
		case 'Z': printf("Dezena de milhar: %d\n", dezena_de_milhar); break;
		case 'M':printf(" Unidade de milhar: %d\n", unidade_de_milhar); break;
		case 'C': printf("Centena: %d\n", centena); break;
		case 'D': printf(" Dezena: %d\n", dezena ); break;
		case 'U': printf("Unidade: %d\n", unidade); break;
		default: printf("erro\n");
	}
	return 0;
	}
