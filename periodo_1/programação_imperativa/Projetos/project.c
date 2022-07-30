#include <stdio.h>
#include <string.h>
#include <stdlib.h>
	int Custos[40], Vendas[40];
	int Escolha,i, cont=0,Excluir;
	char A[30][20];
void IncluirFilial()
{
do
{

			printf("qual o nome da filial?\n");
			scanf("%s", A[cont]);
			printf("Custo operacional:\n");
			scanf("%d", &Custos[cont]);
			printf("Montante de Vendas\n");
			scanf("%d", &Vendas[cont]);
			printf("Inserir nova filial: 1-sim 2-nao\n");
			scanf("%d", &Escolha);
			cont++; 
			}
			while(Escolha==1);
		}
void ExcluirFilial()
{
	
	printf("qual filial voce deseja excluir?\n");
	for(i=0; i<=cont; i++)
	printf("%d - fillial %s\n", i, A[i]);
	scanf("%d", &Excluir);
int main ()
{
	char opcao;
	do{
	system("cls");
	printf("escolha uma das opcoes abaxio:\n");
	printf("1 - Incluir uma nova filial\n");
	printf("2 - Excluir uma filial\n");
	printf("3 - Alterar uma filial\n");
	printf("4 - Melhores fililais\n");
	printf("5 - sair\n");
	printf("opcao: ");
	scanf("%c", &opcao);
	switch(opcao)
	{
		case 'a': IncluirFilial();break;
		case 'b': ExcluirFilial();break;
	}
	printf("%d\n", cont);
	system("pause");
}
	while(opcao!=5);
	return 0;
}
