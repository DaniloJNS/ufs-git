#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
	int Custos[40], Vendas[40];
	int cont=0;
	char NomeFiliais[30][20];
void IncluirFilial()
{
	int Escolha;
	do
	{
		for(cont=0;cont<=39;cont++)
		{
			if(Vendas[cont]<=-1)
			{          
				fflush(stdin);
				printf("qual o nome da filial?\n");
				gets(NomeFiliais[cont]);
				printf("Custo operacional:\n");
				scanf("%d", &Custos[cont]);
				printf("Montante de Vendas\n");
				scanf("%d", &Vendas[cont]);
				break;
			}
		}
		printf("Inserir nova filial: 1-sim 2-nao\n");
		scanf("%d", &Escolha); 
	}
	while(Escolha==1);
}
void ExcluirFilial()
{
	int Verificador=0, Excluir;
		for(cont=0; cont<=39; cont++)
	{
		if(Vendas[cont]>-1)
		{
			Verificador++;
		}
	}
	if(Verificador==0)
		printf("Nao existem filiais cadastradas\n");
	else
	{
		printf("qual filial voce deseja excluir?\n");
	for(cont=0; cont<=39; cont++)
	{
		if(Vendas[cont]>1)
		{
			printf("%d - fillial %s\n", cont, NomeFiliais[cont]);
		}
	}
		scanf("%d", &Excluir); 
		Vendas[Excluir]=-1; 
	}
}	
void AlterarFilial()
{
	int Alterar,Verificador=0;
			for(cont=0; cont<=39; cont++)
	{
		if(Vendas[cont]>-1)
		{
			Verificador++;
		}
	}
	
	if(Verificador==0)
			printf("Nao existem filiais cadastradas\n");
			else
			{
	printf("qual filial voce deseja alterar?\n");
	for(cont=0;cont<=39;cont++)
	{
		if(Vendas[cont]>-1)
		printf("%d - fillial %s\n", cont, NomeFiliais[cont]);
	}
	scanf("%d", &Alterar);
	printf("Custo operacional:\n");
	scanf("%d", &Custos[Alterar]);
	printf("Montante de Vendas\n");
	scanf("%d", &Vendas[Alterar]); 
}
}
void MelhoresFiliais()
{
	int MediadeLucro=0, Soma=0, Divisor=0;
	for(cont=0;cont<=39;cont++)
	{
		if(Vendas[cont]>-1)
		{
			Divisor++;
			Soma=Vendas[cont]-Custos[cont]+Soma;
			printf("filial %s\n", NomeFiliais[cont]);
		}
	}
		MediadeLucro= Soma / Divisor;
	for(cont=0;cont<=39;cont++)
	{
		if(Vendas[cont]>-1)
		{
			if(Vendas[cont]-Custos[cont]>MediadeLucro)
			{	
			printf("%s com Saldo de R$ %d\n",NomeFiliais[cont],Vendas[cont]-Custos[cont]);
			}
		}
	}

}
		
int main ()
{
	char opcao;
	system(" color F0");
	for(cont=0;cont<=39;cont++)
		Vendas[cont]=-1;
	do
	{		
		system("cls");
		printf("escolha uma das opcoes abaxio:\n");
		printf("A - Incluir uma nova filial\n");
		printf("B - Excluir uma filial\n");
		printf("C - Alterar uam filial\n");
		printf("D - Melhores fililais\n");
		printf("E - sair\n");
		printf("opcao: ");
		scanf("%c", &opcao); 
		opcao=toupper(opcao);
		switch(opcao)
		{
			case 'A': IncluirFilial();break;
			case 'B': ExcluirFilial();break;
			case 'C': AlterarFilial();break;
			case 'D': MelhoresFiliais();break;
			default: printf("Opcao invalida\n");
		}
		system("pause");
	}
	while(opcao!='E');
	return 0;
}
