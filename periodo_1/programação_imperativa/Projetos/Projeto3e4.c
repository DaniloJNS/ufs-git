#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int fila[20], parametro=-1, verificador=-1, Valido=-1;
struct Corrida{
	char Cliente[20];
	char Motorista[20];
	char Endereco[50];
	char Status;
	float Preco;
	float Distancia;};
typedef struct Corrida TpCorrida;

TpCorrida Dmotorista[20];

struct Motoristas{
	float Faturamento;
	float Kilometragem;
	char Taxista[20];
	char Data[12];
	};
typedef struct Motoristas TpMotorista;

TpMotorista FatuMotorista;

FILE *RgMotorista;

long int Tamanho=32*sizeof(char)+sizeof(float)+sizeof(float);

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
	printf("		posicao: %d  Motorista: %s\n\n",cont+1, Dmotorista[fila[cont]].Motorista);
    printf("		Desejar solicitar qual motorista?\n		Nome: ");
    scanf(" %[^\n]s", Nome);
    cont=0;
	do{
	if(strcmp(Dmotorista[cont].Motorista, Nome)==0 && Dmotorista[cont].Status=='1'){
	ponteiro= cont;
	Dmotorista[cont].Status='2';
	for(T=0; T<=parametro;T++)
	if(fila[T+1]!=-1) 	
	fila[T]=fila[T+1];
	parametro--;
	}
	cont++;}
	while(cont<=19);
	if(ponteiro!=-1){
    printf("\n\n		Qual o nome do cliente?\n		Cliente:  ");
    scanf(" %[^\n]s",Dmotorista[ponteiro].Cliente);
    printf("\n\n		Qual o endereco do cliente?\n		Destino:  ");
    scanf(" %[^\n]s",Dmotorista[ponteiro].Endereco);
    printf("\n\n		O MOTORISTA %s FOI SOLICITADO COM SUCESSO", Dmotorista[ponteiro].Motorista);}
    else
    printf("\n		Motorista nao existe ou invalido\n");
    printf("\n\n 		Deseja inserir novo servico? S|N\n		Opcao:  ");
    scanf(" %c",&Sair);
    Sair=toupper(Sair);}
	while (Sair!='N' && parametro>=0);
	if(parametro<0 && Sair!='N'){
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
	 for(cont=0;cont<=19;cont++)
	if(Dmotorista[cont].Status=='2')
	check=-1;
    if(check==-1){
	printf("		Motorista 'EM TRANSITO': \n\n");	
	for(cont=0;cont<=19;cont++)
	if(Dmotorista[cont].Status=='2')
	printf("			Motorista: %s\n\n", Dmotorista[cont].Motorista);
    printf("		De qual motorista a corrida foi iniciada?\n		Nome: ");
    scanf(" %[^\n]s", Nome);
    for(cont=0;cont<=verificador;cont++){
	if(strcmp(Dmotorista[cont].Motorista, Nome)==0){
	Dmotorista[cont].Status='3';
	check++;
	printf("\n\n		O MOTORISTA %s COMECOU UMA CORRIDA COM SUCESSO\n", Dmotorista[cont].Motorista);}}
	if(check==-1)
	printf("\n\n		ERRO DE DADOS\n\n		Motorista invalido\n\n");}
	else
	printf("		Nao existem motorista em transito\n\n");
	system("\n		pause");
}
 
void CancelarCorrida(){
	int cont, check=-2;
	char Nome[20];
	system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
    for(cont=0;cont<=19;cont++)
	if(Dmotorista[cont].Status=='2')check=-1;
    if(check==-1){
	printf("		Motorista 'EM TRANSITO': \n\n");	
	for(cont=0;cont<=19;cont++)
	if(Dmotorista[cont].Status=='2')
	printf("			Motorista: %s\n\n", Dmotorista[cont].Motorista);
    printf("		Deseja cancelar a corrrida de qual motorista?\n			Nome:");
    scanf(" %[^\n]s", Nome);
    for(cont=0;cont<=19;cont++){
	if(strcmp(Dmotorista[cont].Motorista, Nome)==0){
	check++;
	parametro++;	
	fila[parametro]=cont;
	Dmotorista[cont].Status='1';
	printf("\n\n		O MOTORISTA %s VOLTO PARA FILA COM SUCESSO\n", Dmotorista[cont].Motorista);}}
	if(check==-1)
	printf("\n\n		ERRO DE DADOS\n\n		Motorista invalido\n\n");}
	else
	printf("		Nao existem motoristas em transito\n\n");
	system("		pause");
}
 
void ConsultarMotorista(){
	system("cls");
	printf("\n\n		 >>> Radio-Taxi <<< \n\n");
	if(verificador!=-1){
	for (int Cont=0; Cont<=verificador; Cont++){
    printf("\n 		Motorista: %s", Dmotorista[Cont].Motorista);
    if (Dmotorista[Cont].Status=='1')
    printf("\n 		Status: EM ESPERA");    
    if (Dmotorista[Cont].Status=='2')
    printf("\n 		Status: EM TRANSITO");  
    if (Dmotorista[Cont].Status=='3'){
    printf("\n 		Status: EM CORRIDA");
    printf("\n		Cliente: %s",Dmotorista[Cont].Cliente);
    printf("\n		Endereco(destino): %s",Dmotorista[Cont].Endereco);}
    printf("\n		--------------------- \n");}}
	else
	printf("		Nao existem motoristas trabalhando\n\n");
	system("		pause");    
}
void MotoristaDisponivel(){
	fflush(stdin);
	char Nome[20];
	int cont;
	float valor=-1,distancia=-1;
	system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
    if(verificador!=-1){
    printf("\n		Motoristas trabalhando:\n");
    for (int Cont=0; Cont<=verificador; Cont++){
    printf("\n 		Motorista: %s", Dmotorista[Cont].Motorista);
    if(Dmotorista[Cont].Status=='3')
    printf("  'EM CORRIDA'");}}
	parametro++;
	printf("\n		Qual nome do motorista que esta disponivel?\n		Nome: ");
	scanf(" %[^\n]s",Nome);
	for(cont=0;cont<=19;cont++)
	{
	if(strcmp(Dmotorista[cont].Motorista,Nome)!=0 && Dmotorista[cont].Status=='0'){
	valor++;
	verificador++;
	fila[parametro]=cont;
	strcpy(Dmotorista[cont].Motorista,Nome);
	Dmotorista[cont].Status='1';
	printf("\n\n		O MOTORISTA %s ENTRO NA FILA COM SUCESSO\n\n", Dmotorista[cont].Motorista);
	break;}
	if(strcmp(Dmotorista[cont].Motorista,Nome)==0 && Dmotorista[cont].Status=='3')
	{
	fila[parametro]=cont;
	Dmotorista[cont].Status='1';
	printf("\n\n		Qual o valor da corrida da corrida?\n		Valor: ");
	scanf("%f", &valor);
	printf("\n\n		Qual a distancia percorrida (em KM)?\n		Distancia: ");
	scanf("%f", &distancia);
	Dmotorista[cont].Preco=Dmotorista[cont].Preco + valor;
	Dmotorista[cont].Distancia=Dmotorista[cont].Distancia+distancia;
	Valido++;
	printf("\n\n		O MOTORISTA %s VOLTO PARA FILA COM SUCESSO\n", Dmotorista[cont].Motorista);
	break;}
	if(strcmp(Dmotorista[cont].Motorista,Nome)==0 && Dmotorista[cont].Status!='3'){
	printf("\n\n		ERRO DE DADOS\n\n		Motorista nao esteve em uma corrida\n\n");
	parametro--;
	break;}
	}
	system("\n\n		pause");}

void Consularfaturamento(){
	float FatuTotal=0, KiloTotal=0;
	char Nome[20], opcao;
	fseek(RgMotorista,0,2);
    long int R=ftell(RgMotorista)/Tamanho;
    if(R!=0){
	do{	
	FatuTotal=0; 
	KiloTotal=0;
	system("cls");
	printf("\n\n 		>>> Radio-Taxi <<< \n\n");
	printf("\n\n		Deseja exibir o faturamento de qual motorista?\n		Motorista: ");
	scanf(" %[^\n]s",Nome);
	fclose(RgMotorista);
	RgMotorista=fopen("Motorista.dat","r+b");
	fseek(RgMotorista,0,0);
	do
	{
	fread(&FatuMotorista,Tamanho,1, RgMotorista);
	if(strcmp(Nome,FatuMotorista.Taxista)==0 && !feof(RgMotorista))
	{
	printf("\n\n		motorista %s:", Nome);	
	printf("\n\n		Data: %s", FatuMotorista.Data);
	printf("\n		Faturamento: %.2f", FatuMotorista.Faturamento);
	printf("\n		Kilometragem: %.2f", FatuMotorista.Kilometragem);
	printf("\n	----------------------");
	FatuTotal= FatuTotal+ FatuMotorista.Faturamento;
	KiloTotal=KiloTotal+FatuMotorista.Kilometragem;
	}
	}
	while(!feof(RgMotorista));
	if(FatuTotal!=0 && KiloTotal!=0){
	printf("\n\n		Faturemento Total = %.2f", FatuTotal);
	printf("\n 		Kilometragem Total = %.2f\n\n", KiloTotal);
	}
	else
	printf("\n\n		Registros inexistentes\n");
	fclose(RgMotorista);
	RgMotorista=fopen("Motorista.dat","a+b");
	fflush(stdin);
	printf("\n\n Desejar exibir o faturamento de novo? S/N\n		Opcao: ");
	scanf("%c", &opcao);
	opcao=toupper(opcao);} 
	while(opcao!='N');}
	else
	printf("\n		Arquivo vazio\n");
	system("\n		pause");
}
void EncerrarExpediente(){
	system("cls");
	printf("\n\n 		>>> Radio-Taxi <<< \n\n");
	if(Valido!=-1){
		fflush(stdin);
		float soma=0;
		char Dia[12], Mes[4], Ano[6], Option, Barra[2]={"/"};
		for (int Cont=0; Cont<=verificador; Cont++){
		printf("\n 		Motorista: %s", Dmotorista[Cont].Motorista);
		printf("\n		Valor arrecadado do dia: %.2f", Dmotorista[Cont].Preco);
		printf("\n		Distancia percorrida: %.2f", Dmotorista[Cont].Distancia);
		printf("\n		--------------------- \n");
		soma= Dmotorista[Cont].Preco + soma;}
		printf("\n		Valor total obtido: %.2f\n", soma);
		printf("\n		Deseja salvar os dados? S/N \n		Opcao: ");
		scanf("%c", &Option);
		Option=toupper(Option);
		if(Option=='S'){
			printf("\n\n		Qual data deste expediente?\n		Dia: ");
			scanf(" %[^\n]s",Dia);
			printf("\n		Mes: ");
			scanf(" %[^\n]s",Mes);
			printf("\n		Ano: ");
			scanf(" %[^\n]s",Ano);
			strcat(Dia, Barra);
			strcat(Dia,	Mes);
			strcat(Dia, Barra);
			strcat(Dia, Ano);
			for (int Cont=0; Cont<=verificador; Cont++)
			{
				strcpy(FatuMotorista.Data, Dia);
				strcpy(FatuMotorista.Taxista,Dmotorista[Cont].Motorista);
				FatuMotorista.Faturamento=Dmotorista[Cont].Preco;
				FatuMotorista.Kilometragem=Dmotorista[Cont].Distancia;
				fseek(RgMotorista,0,2);
				fwrite(&FatuMotorista,Tamanho,1, RgMotorista);
			}
			printf("\n\n		DADOS SALVOS COM SUCESSO\n\n");
		}
	}else
	printf("\n\n		Nao existem corridas completas\n		");
	system("\n		pause");
}


int main(){
	RgMotorista=fopen("Motorista.dat","a+b");
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
	printf("		4 - Consultar Motoristas \n");
	printf("		5 - Motorista Disponivel\n");
	printf("		6 - Encerrar Expediente \n");
	printf("		7 - Consultar Faturamento\n");
	printf("		8 - Sair \n\n");
	printf("		Digite a opcao desejada:\n		Opcao: ");
	scanf("%d",&Opcao); 
	switch (Opcao){
		case 1: SolicitarMotorista(); break;
		case 2: IniciarCorrida(); break;
		case 3: CancelarCorrida(); break;
		case 4: ConsultarMotorista(); break;
		case 5: MotoristaDisponivel(); break;
		case 6: EncerrarExpediente(); break;
		case 7: Consularfaturamento(); break;
		case 8: break;        
		default: printf("\n ERRO: Opcao Invalida! \7\n");
	break;}}
	while (Opcao!=8);
	fclose(RgMotorista);
	return 0;
  }
