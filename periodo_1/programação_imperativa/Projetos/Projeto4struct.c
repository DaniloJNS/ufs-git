#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 int fila[20], parametro=-1, verificador=-1;
struct Corrida{
    char Cliente[20];
    char Motorista[20];
    char Endereco[50];
    char Status;
    float Preco;};
typedef struct Corrida TpCorrida;
TpCorrida Dmotorista[20];
void SolicitarMotorista(){
	fflush(stdin);
  char Sair='S';
  char Nome[20];
  int ponteiro=-1, cont, T;
  if(parametro>=0){
  do{
    system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
    printf("		Motoristas disponiveis:\n\n");
      for(cont=0;cont<=parametro;cont++)
		if(Dmotorista[fila[cont]].Status=='1')
			{
						printf("		posicao: %d  Motorista: %s\n\n",cont+1, Dmotorista[fila[cont]].Motorista);
			}
    printf("		Qual o nome motorista?\n		Nome: ");
    scanf(" %[^\n]s", Nome);
    cont=0;
do{
	
		if(strcmp(Dmotorista[cont].Motorista, Nome)==0 && Dmotorista[cont].Status=='1')
		{
			ponteiro = cont;
			Dmotorista[cont].Status='2';
			for(T=0; T<=parametro;T++)
			if(fila[T+1]!=-1) 	
			fila[T]=fila[T+1];
			parametro--;
		}
			cont++;
		}
		while(cont<=19);
		if(ponteiro!=-1){
    printf("\n\n		Qual o nome do cliente?\n		Cliente:  ");
    scanf(" %[^\n]s",Dmotorista[ponteiro].Cliente);
    printf("\n\n		Qual o endereco do cliente?\n		Destino:  ");
    scanf(" %[^\n]s",Dmotorista[ponteiro].Endereco);}
        else
    printf("\n		Motorista nao existe ou invalido\n");
    printf("\n\n 		Deseja inserir novo servico? S|N\n		Opcao:  ");
    scanf(" %c",&Sair);
    Sair=toupper(Sair);}
  while (Sair!='N' && parametro>=0);
  if(parametro<0){
	  printf("\n\n		Nao existem motoristas disponiveis para corridas\n\n");
	  system("		pause");}}
	  else{
	  printf("\n\n		Nao existem motoristas disponiveis para corridas\n\n");
	  system("		pause");}
	
}
 
void IniciarCorrida(){
	int cont, check=-2;
	char Nome[20];
	system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
	for(cont=0;cont<=verificador;cont++)
		if(Dmotorista[cont].Status=='2')
			{
						check=-1;
						printf("		Motorista: %s\n\n", Dmotorista[cont].Motorista);
			}
			if(check==-1)
			{
    printf("		Qual o nome do motorista?\n		Nome: ");
    scanf(" %[^\n]s", Nome);
     for(cont=0;cont<=verificador;cont++)
     {
		if(strcmp(Dmotorista[cont].Motorista, Nome)==0)
			{
			Dmotorista[cont].Status='3';
			check++;
			}
		}
			if(check==-1)
				printf("\n\n		ERRO DE DADOS\n\n		Motorista invalido\n\n");}
				else
				printf("		Nao existem motorista em transito\n\n");
		system("		pause");
}
 
void CancelarCorrida(){
	int cont, check=-2;
	char Nome[20];
	system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
	for(cont=0;cont<=19;cont++)
		if(Dmotorista[cont].Status=='2')
			{
						check=-1;
						printf("			Motorista: %s\n\n", Dmotorista[cont].Motorista);
			}
	if(check==-1)
	{
    printf("		Qual o nome do motorista?\n			Nome:");
    scanf(" %[^\n]s", Nome);
     for(cont=0;cont<=19;cont++)
    {
		if(strcmp(Dmotorista[cont].Motorista, Nome)==0)
			{
			check++;
			parametro++;	
			fila[parametro]=cont;
			Dmotorista[cont].Status='1';
			}
		}
		if(check==-1)
				printf("\n\n		ERRO DE DADOS\n\n		Motorista invalido\n\n");
			}
			else
			printf("		Nao existem motoristas em transito\n\n");
		system("		pause");
}
 
void ConsultarMotorista(){
  system("cls");
  printf("\n\n		 >>> Radio-Taxi <<< \n\n");
  if(verificador!=-1)
  {
  for (int Cont=0; Cont<=verificador; Cont++){
    printf("\n 		Motorista: %s", Dmotorista[Cont].Motorista);
    if (Dmotorista[Cont].Status=='1')
      printf("\n 		Status: EM ESPERA");    
    if (Dmotorista[Cont].Status=='2')
      printf("\n 		Status: EM TRANSITO");  
     if (Dmotorista[Cont].Status=='3')
     {
           printf("\n 		Status: EM CORRIDA");
           printf("\n		Cliente: %s",Dmotorista[Cont].Cliente);
           printf("\n		Endereco(destino): %s",Dmotorista[Cont].Endereco);
	 }
    printf("\n		--------------------- \n");}
}
else
printf("		Nao existem motoristas trabalhando\n\n");
  system("		pause");    
}
void MotoristaDisponivel(){
	fflush(stdin);
	char Nome[20];
	int cont;
	float valor=-1;
	system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
    if(verificador!=-1){
    printf("\n		Motoristas trabalhando:\n");
    for (int Cont=0; Cont<=verificador; Cont++)
    printf("\n 		Motorista: %s", Dmotorista[Cont].Motorista);}
	parametro++;
	printf("\n		Qual nome do motorista?\n		Nome: ");
	scanf(" %[^\n]s",Nome);
	for(cont=0;cont<=19;cont++)
	{
	if(strcmp(Dmotorista[cont].Motorista,Nome)!=0 && Dmotorista[cont].Status=='0')
		{
		valor++;
		verificador++;
		fila[parametro]=cont;
		strcpy(Dmotorista[cont].Motorista,Nome);
		Dmotorista[cont].Status='1';
		break;
		}
	if(strcmp(Dmotorista[cont].Motorista,Nome)==0 && Dmotorista[cont].Status=='3')
	{
			fila[parametro]=cont;
			Dmotorista[cont].Status='1';
			printf("\n\n		Qual o valor da corrida da corrida?\n		Valor: ");
			scanf("%f", &valor);
			Dmotorista[cont].Preco=Dmotorista[cont].Preco + valor;
			break;
	}
	if(strcmp(Dmotorista[cont].Motorista,Nome)==0 && Dmotorista[cont].Status!='3')
	{
		printf("\n\n		ERRO DE DADOS\n\n		Motorista nao esteve em uma corrida\n\n");
		parametro--;
		break;
	}
}
system("\n\n		pause");
 }
void EncerrarExpediente(){
	float soma=0;
	for (int Cont=0; Cont<=verificador; Cont++){
    printf("\n 		Motorista: %s", Dmotorista[Cont].Motorista);
	printf("\n		Valor arrecadado do dia: %.2f", Dmotorista[Cont].Preco);
    printf("\n		--------------------- \n");
    soma= Dmotorista[Cont].Preco + soma;}
    printf("\n		Valor total obtido: %.2f\n", soma);
    system("\n		pause");
}
 
int main(){
	fflush(stdin);
  int Opcao, cont;
  for(cont=0; cont<=19; cont++)
  {
  Dmotorista[cont].Status='0';
  Dmotorista[cont].Preco=0;
  fila[cont]=-1;
}
  system("color F0");
  do{
    system("cls");
    printf("\n\n		 >>> Radio-Taxi <<< \n\n");
    printf("		1 - Solicitar Motorista \n");
    printf("		2 - Iniciar Corrida \n");
    printf("		3 - Cancelar Corrida \n");
    printf("		4 - Consultar Motorista \n");
    printf("		5 - Motorista Disponivel\n");
    printf("		6 - Encerrar Expediente \n");
    printf("		7 - Sair \n\n");
    printf("		Digite a opcao desejada:\n		Opcao: ");
    scanf("%d",&Opcao); 
    switch (Opcao){
      case 1: SolicitarMotorista(); break;
      case 2: IniciarCorrida(); break;
      case 3: CancelarCorrida(); break;
      case 4: ConsultarMotorista(); break;
      case 5: MotoristaDisponivel(); break;
      case 6: EncerrarExpediente(); break;
      case 7: break;        
      default: printf("\n ERRO: Opcao Invalida! \7\n");
               break;}}
  while (Opcao!=7);
  return 0;
  }
