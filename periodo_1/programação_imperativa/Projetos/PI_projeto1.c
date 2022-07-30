#include <string.h>
#include <stdlib.h>
#include <ctype.h>

float Custos[10], Vendas[10];
char NomeFiliais[10][15];

void IncluirFilial()
{
	int Escolha, cont=0;
	
	do
	{
		
		for(cont=0;cont<=9;cont++)
			if(Vendas[cont]<=-1)
			{          
				fflush(stdin);
				printf("\n\n	Qual o nome da filial?\n");
				printf("	Nome: ");
				gets(NomeFiliais[cont]);
				printf("\n	Custo operacional:\n	Valor: R$ ");
				scanf("%f", &Custos[cont]);
				printf("\n	Montante de Vendas:\n	Valor: R$ ");
				scanf("%f", &Vendas[cont]);
				break;
			}
		
		printf("\n\n	Inserir nova filial: 1-sim 2-nao\n");
		printf("	Escolha: "); 
		scanf("%d", &Escolha);
		 
	}
	while(Escolha==1);
}

void ExcluirFilial()
{
	int Verificador=0, Excluir, cont=0;
	
	for(cont=0; cont<=10; cont++)
		if(Vendas[cont]>-1)
			Verificador++;
			
	if(Verificador==0)
		printf("\n\n	Nao existem filiais cadastradas\n\n");
		
	else
	{
		Verificador=0;
		printf("\n\n	Qual filial voce deseja excluir?\n\n");
		
		for(cont=0; cont<=10; cont++)
			if(Vendas[cont]>1)
			{
				Verificador++;
				printf("	%d - Fillial %s\n\n", cont, NomeFiliais[cont]);
			}
			
		printf("	Opcao: ");	
		scanf("%d", &Excluir); 
		
		if(Excluir>Verificador-1 || Excluir<0)
			printf("\n\n	OPCAO INVALIDA\n\n");
			
		else
			{
				Vendas[Excluir]=-1;
				printf("\n\n	Filial %s excluida com SUCESSO\n\n", NomeFiliais[Excluir]);
			} 
	}
}	

void AlterarFilial()
	{
		
		int Alterar,Verificador=0,cont=0;
		
		for(cont=0; cont<=10; cont++)
			if(Vendas[cont]>-1)
				Verificador++;
			
		if(Verificador==0)
			printf("\n\n	Nao existem filiais cadastradas\n\n");
				
		else
			{
				Verificador=0;
				printf("\n\n	Qual filial voce deseja alterar?\n\n");
				
				for(cont=0;cont<=9;cont++)
					if(Vendas[cont]>-1)
					{
						Verificador++;
						printf("	%d - Fillial %s\n\n", cont, NomeFiliais[cont]);
					}
					
				printf("	Opcao: ");
				scanf("%d", &Alterar);
				
				if(Alterar>Verificador-1 || Alterar<0)
					printf("\n\n	OPCAO INVALIDA\n\n");
					
				else
				{	
					printf("\n\n	Custo operacional:\n	Valor: R$ ");
					scanf("%f", &Custos[Alterar]);
					printf("\n\n	Montante de Vendas:\n	Valor: R$ ");
					scanf("%f", &Vendas[Alterar]);
					printf("\n\n	Dados Alterados com SUCESSO\n\n");
				 }
				 
			 }
	}

void MelhoresFiliais()
	{
		
		int MediadeLucro=0,Soma=0,Divisor=0,cont=0,Verificador=0;
		
		for(cont=0; cont<=10; cont++)
			if(Vendas[cont]>-1)
				Verificador++;
			
		if(Verificador==0)
			printf("\n\n	Nao existem filiais cadastradas\n\n");
				
		else
		{
			for(cont=0;cont<=9;cont++)
					if(Vendas[cont]>-1)
						{
							Divisor++;
							Soma=Vendas[cont]-Custos[cont]+Soma;
						}
				
			MediadeLucro = Soma / Divisor;
			printf("\n\n	As melhores filiais sao:\n\n");
					
			for(cont=0;cont<=9;cont++)
					if(Vendas[cont]>-1)
							if(Vendas[cont]-Custos[cont]>MediadeLucro)
								printf("	%s com LUCRO de R$ %.2f\n\n",NomeFiliais[cont],Vendas[cont]-Custos[cont]);
		}
	}			

int main()
	{
		char opcao;
		int cont=0;

		system(" color F0");
		
		for(cont=0;cont<=9;cont++)
			Vendas[cont]=-1;
			
		do
		{	
		
			system("cls");
			puts("\a");
			fflush(stdin);
			
			printf("*******GERENCIADOR DE LUCRO DAS FILIAIS*******\n\n\n"); 	
			printf("	Escolha uma das opcoes abaxio:\n\n");
			printf("	A - Incluir uma nova filial\n");
			printf("	B - Excluir uma filial\n");
			printf("	C - Alterar uma filial\n");
			printf("	D - Melhores fililais\n");
			printf("	E - sair\n\n");
			printf("	Opcao: ");
			scanf("%c", &opcao);
			 
			opcao=toupper(opcao);
			
			switch(opcao)
			{
				case 'A': IncluirFilial();system("pause");break;
				case 'B': ExcluirFilial();system("pause");break;
				case 'C': AlterarFilial();system("pause");break;
				case 'D': MelhoresFiliais();system("pause");break;
				case 'E': printf("\n\n	Programa Encerrado\n\n");break;
				default: printf("\n\nOpcao invalida\n\n");system("pause");
			}
		}
		while(opcao!='E');
		return 0;
	}

Projeto01(Filiais).c
Exibindo Projeto01(Filiais).c.
