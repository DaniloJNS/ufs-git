#include <stdio.h>
int main()
{
	int dia, D1, D2;
	float IC, ICT, Media;
	printf("dia inicial da pesquisa:\n");
	scanf("%d", &D1);
	printf("dia final da pesquisa:\n");
	scanf("%d", &D2);
	if(D2>=D1)
	{
	for(dia= D1; dia<=D2; dia++)
	{
	printf("indice de chuva:\n");
	scanf("%f", &IC);
	ICT = IC + ICT;
	}
	Media= ICT / (D2 - D1);
	printf("media dos indice de chuva: %.1f\n", Media);
	}
	else 
	printf ("erro");
	return 0; 
}
	
	
