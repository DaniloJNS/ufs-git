#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int parametro=-1,verificador=-1, Valido=-1;

struct Corrida{
	char Cliente[20];
	char Motorista[20];
	char Endereco[50];
	char Status;
	float Preco;
	float Distancia;
	void *Auxiliar;
	void *Fim;};
typedef struct Corrida TpCorrida;

TpCorrida *Dmotorista,*Inicio, *Ultimo;
TpCorrida *Fila, *Primeiro, *MulherDoPadre;

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

void SolicitarMotorista()
 {
	fflush (stdin);
	
	char Sair='S';
	char Nome[20];
	int cont=0;
	
	if (parametro>=0)
		{
			do
				{
					system("cls");
					
					printf("\n\n 		>>> Radio-Taxi <<< \n\n");
					printf("		Motoristas disponiveis:\n\n");
					
					Fila=Primeiro;
					cont=0;
					
					while (Fila != NULL)
						{ 
							if (Fila->Status=='1')
							{
							cont++;
							printf("		posicao: %d  Motorista: %s\n\n",cont, Fila->Motorista);
							}
							Fila=Fila->Fim;
						}
						
					printf("		Desejar solicitar qual motorista?\n		Nome: ");
					scanf(" %[^\n]s", Nome);
					
					cont=0;
					Dmotorista=Inicio;
					
					do
						{
							if (strcmp(Dmotorista->Motorista, Nome)==0 && Dmotorista->Status=='1')
								{
									Dmotorista->Status='2';
									printf("\n\n		Qual o nome do cliente?\n		Cliente:  ");
									scanf(" %[^\n]s",Dmotorista->Cliente);
									printf("\n\n		Qual o endereço do destino?\n		Destino:  ");
									scanf(" %[^\n]s",Dmotorista->Endereco);
									printf("\n\n		O MOTORISTA %s FOI SOLICITADO COM SUCESSO", Dmotorista->Motorista);
									parametro--;	
									Primeiro=Primeiro->Fim;
									cont++;
								}
							Dmotorista=Dmotorista->Auxiliar;
						}
					while (Dmotorista!=NULL);
					
					if (cont==0)
						printf("\n		Motorista nao existe ou invalido\n");
						
					printf("\n\n 		Deseja inserir novo servico? S|N\n		Opcao:  ");
					scanf(" %c",&Sair);
					Sair=toupper(Sair);
					
				}
			while (Sair!='N' && parametro>=0);
			
			if (parametro<0 && Sair!='N')
				{
					printf("\n\n		Nao existem motoristas disponiveis para corridas\n\n");
					system("		pause");
				}
				
		}
		
	else
		{
			printf("\n\n		Nao existem motoristas disponiveis para corridas\n\n");
			system("		pause");
		}
	
}

void IniciarCorrida(){
	
	int check=-2;
	char Nome[20];
	
	system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
    
    Dmotorista=Inicio;
    
	while (Dmotorista!=NULL)
		{
			if(Dmotorista->Status=='2')
				check=-1;
			Dmotorista=Dmotorista->Auxiliar;
		}
		
    if (check==-1)
		{
			
			printf("		Motorista 'EM TRANSITO': \n\n");	
			
			Dmotorista=Inicio;
			
			while (Dmotorista!=NULL)
				{
					if(Dmotorista->Status=='2')
						printf("			Motorista: %s\n\n", Dmotorista->Motorista);
						
					Dmotorista=Dmotorista->Auxiliar;
				}
				
			printf("		De qual motorista a corrida foi iniciada?\n		Nome: ");
			scanf(" %[^\n]s", Nome);
			
			Dmotorista=Inicio;
			
			while (Dmotorista!=NULL)
				{
					if (strcmp(Dmotorista->Motorista, Nome)==0)
						{
							Dmotorista->Status='3';
							check++;
							printf("\n\n		O MOTORISTA %s COMECOU UMA CORRIDA COM SUCESSO\n", Dmotorista->Motorista);
						}
						
					Dmotorista=Dmotorista->Auxiliar;
				}
				
			if (check==-1)
				printf("\n\n		ERRO DE DADOS\n\n		Motorista invalido\n\n");
				
		}
		
	else
	printf("		Nao existem motorista em transito\n\n");
	
	system("		pause");
}

void CancelarCorrida(){
	
	int check=-2;
	char Nome[20];
	
	system("cls");
	
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
    
    Dmotorista=Inicio;
    
	while (Dmotorista!=NULL)
	{
		if (Dmotorista->Status=='2')
		check=-1;
		Dmotorista=Dmotorista->Auxiliar;
	}
	
    if (check==-1){
	printf("		Motorista 'EM TRANSITO': \n\n");	
	
	Dmotorista=Inicio;
	while (Dmotorista!=NULL)
		{
			
			if (Dmotorista->Status=='2')
				printf("			Motorista: %s\n\n", Dmotorista->Motorista);
				
					Dmotorista=Dmotorista->Auxiliar;
					
		}
		
    printf("		Deseja cancelar a corrrida de qual motorista?\n			Nome:");
    scanf(" %[^\n]s", Nome);
    
    Dmotorista=Inicio;
    
	while (Dmotorista!=NULL)
		{
				if (strcmp(Dmotorista->Motorista, Nome)==0)
					{
						check++;
						parametro++;	
						Dmotorista->Status='1';
						Fila=Dmotorista;
						
					if (parametro==0) 
						{
							Fila->Fim=NULL;
							Primeiro=Fila;
							MulherDoPadre=Fila;					
							printf("p");
						}
						
					else
						{
							MulherDoPadre->Fim=Fila;
							MulherDoPadre=Fila;
							Fila->Fim=NULL;
							printf("		\n\nfoi\n\n");
						}
						
						printf("\n\n		O MOTORISTA %s VOLTO PARA FILA COM SUCESSO\n", Dmotorista->Motorista);
						
					}
				Dmotorista=Dmotorista->Auxiliar;
		}
		
	if (check==-1)
	printf("\n\n		ERRO DE DADOS\n\n		Motorista invalido\n\n");
	
	}
	
	else
	printf("		Nao existem motoristas em transito\n\n");
	
	system("		pause");
	
}

void ConsultarMotorista(){
	system("cls");
	
	printf("\n\n		 >>> Radio-Taxi <<< \n\n");
	
	if (verificador!=-1)
		{
			
			Dmotorista=Inicio;
			
			while (Dmotorista!=NULL)
				{
					printf("\n 		Motorista: %s", Dmotorista->Motorista);
					
					if (Dmotorista->Status=='1')
						printf("\n 		Status: EM ESPERA");    
						
					if (Dmotorista->Status=='2')
						printf("\n 		Status: EM TRANSITO");  
						
					if (Dmotorista->Status=='3')
						{
							printf("\n 		Status: EM CORRIDA");
							printf("\n		Cliente: %s",Dmotorista->Cliente);
							printf("\n		Endereco(destino): %s",Dmotorista->Endereco);
						}
						
					printf("\n		--------------------- \n");
					
						Dmotorista=Dmotorista->Auxiliar;
						
				}
		}
	else
		printf("		Nao existem motoristas trabalhando\n\n");
		
	system("		pause");    
}

void MotoristaDisponivel(){
	fflush(stdin);
	
	char Nome[20];
	float valor=-1,distancia=-1;
	
	system("cls");
    printf("\n\n 		>>> Radio-Taxi <<< \n\n");
    
    if (verificador!=-1)
		{
			printf("\n		Motoristas trabalhando:\n");
			Dmotorista=Inicio;
			while (Dmotorista!=NULL)
				{
					printf("\n 		Motorista: %s", Dmotorista->Motorista);
					if(Dmotorista->Status=='3')
						printf("  'EM CORRIDA'");
					Dmotorista=Dmotorista->Auxiliar;
				}
		}
		
	parametro++;
	
	printf("\n		Qual nome do motorista que esta disponivel?\n		Nome: ");
	scanf(" %[^\n]s",Nome);
	
	Dmotorista=Inicio;
	
	while (Dmotorista!=NULL)
		{
			if (strcmp(Dmotorista->Motorista,Nome)==0 && Dmotorista->Status=='3')
				{
					Dmotorista->Status='1';
					
					printf("\n\n		Qual o valor da corrida da corrida?\n		Valor: ");
					scanf("%f", &valor);
					
					printf("\n\n		Qual a distancia percorrida (em KM)?\n		Distancia: ");
					scanf("%f", &distancia);
					
					Dmotorista->Preco=Dmotorista->Preco + valor;
					Dmotorista->Distancia=Dmotorista->Distancia+distancia;
					
					Valido++;
					
					Fila=Dmotorista;
					
					if (parametro==0) 
						{
							Fila->Fim=NULL;
							Primeiro=Fila;
							MulherDoPadre=Fila;					
						}
					else
						{
							MulherDoPadre->Fim=Fila;
							MulherDoPadre=Fila;
							Fila->Fim=NULL;
						}
						
							printf("\n\n		O MOTORISTA %s VOLTO PARA FILA COM SUCESSO\n", Dmotorista->Motorista);
							break;
							
						}
						
					if (strcmp(Dmotorista->Motorista,Nome)==0 && Dmotorista->Status!='3')
						{
							printf("\n\n		ERRO DE DADOS\n\n		Motorista nao esteve em uma corrida\n\n");
							parametro--;
							valor++;
							break;
						}
			
			Dmotorista=Dmotorista->Auxiliar;
			
		}
		
	if (Dmotorista==NULL && valor==-1)
		{
			verificador++;
			
			Dmotorista =(TpCorrida *) malloc(sizeof(TpCorrida));
			
			strcpy(Dmotorista->Motorista,Nome);
			Dmotorista->Status='1';
			Dmotorista->Auxiliar=NULL;
			Dmotorista->Preco=0;
			Dmotorista->Distancia=0;
			
			Fila=Dmotorista;
			
			if(verificador==0) 
				{
					Dmotorista->Auxiliar=NULL;
					Inicio=Dmotorista;
					Ultimo=Dmotorista;					
				}
			else
				{
					Ultimo->Auxiliar=Dmotorista;
					Ultimo=Dmotorista;
					printf("		\n\nfoi\n\n");
				}
				
			if (parametro==0) 
				{
					Fila->Fim=NULL;
					Primeiro=Fila;
					MulherDoPadre=Fila;					
				}
			else
				{
					MulherDoPadre->Fim=Fila;
					MulherDoPadre=Fila;
					Fila->Fim=NULL;
					printf("		\n\nfoi\n\n");
				}
				
					printf("\n\n		O MOTORISTA %s ENTRO NA FILA COM SUCESSO\n\n", Dmotorista->Motorista);
				}
				
	system("\n\n		pause");
	
	}

void EncerrarExpediente(){
	system("cls");
	
	printf("\n\n 		>>> Radio-Taxi <<< \n\n");
	
	if (Valido!=-1)
		{
			fflush(stdin);
			
			float soma=0;
			char Dia[12], Mes[4], Ano[6], Option, Barra[2]={"/"};
			
			Dmotorista=Inicio;
			
			while (Dmotorista!=NULL)
				{
					printf("\n 		Motorista: %s", Dmotorista->Motorista);
					printf("\n		Valor arrecadado do dia: %.2f", Dmotorista->Preco);
					printf("\n		Distancia percorrida: %.2f", Dmotorista->Distancia);
					printf("\n		--------------------- \n");
					
					soma= Dmotorista->Preco + soma;
					Dmotorista=Dmotorista->Auxiliar;
					
				}
				
			printf("\n		Valor total obtido: %.2f\n", soma);
			
			printf("\n		Deseja salvar os dados? S/N \n		Opcao: ");
			scanf("%c", &Option);
			Option=toupper(Option);
			
			if (Option=='S')
				{
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
					Dmotorista=Inicio;
					
					while(Dmotorista!=NULL)
						{
							
							strcpy(FatuMotorista.Data, Dia);
							strcpy(FatuMotorista.Taxista,Dmotorista->Motorista);
							FatuMotorista.Faturamento=Dmotorista->Preco;
							FatuMotorista.Kilometragem=Dmotorista->Distancia;
							
							fseek(RgMotorista,0,2);
							fwrite(&FatuMotorista,Tamanho,1, RgMotorista);
							
							Dmotorista=Dmotorista->Auxiliar;
							
						}
						
					printf("\n\n		DADOS SALVOS COM SUCESSO\n\n");
					
				}
		}
	else
	printf("\n\n		Nao existem corridas completas\n		");
	
	system("\n		pause");
}

void Consularfaturamento(){
	
	float FatuTotal=0, KiloTotal=0;
	char Nome[20], opcao;
	
	fseek(RgMotorista,0,2);
    long int R=ftell(RgMotorista)/Tamanho;
    
    if (R!=0)
		{
			do
				{	
					
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
							
							if (strcmp(Nome,FatuMotorista.Taxista)==0 && !feof(RgMotorista))
								{
									
									printf("\n\n		motorista %s:", Nome);	
									printf("\n\n		Data: %s", FatuMotorista.Data);
									printf("\n		Faturamento: %.2f", FatuMotorista.Faturamento);
									printf("\n		Kilometragem: %.2f", FatuMotorista.Kilometragem);
									printf("\n		----------------------");
									
									FatuTotal= FatuTotal+ FatuMotorista.Faturamento;
									KiloTotal=KiloTotal+FatuMotorista.Kilometragem;
									
								}
						}
					while(!feof(RgMotorista));
					
					if (FatuTotal!=0 && KiloTotal!=0)
						{
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
					opcao=toupper(opcao);
					
				} 
			while (opcao!='N');
		}
	else
	printf("\n		Arquivo vazio\n");
	
	system("\n		pause");
}

int main(){
	RgMotorista=fopen ("Motorista.dat","a+b");
	
	fflush(stdin);
	
	int Opcao;
	
	system("color F0");
	
	Inicio=NULL;
	
	do
		{
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
			
			switch (Opcao)
				{
					case 1: SolicitarMotorista(); break;
					case 2: IniciarCorrida(); break;
					case 3: CancelarCorrida(); break;
					case 4: ConsultarMotorista(); break;
					case 5: MotoristaDisponivel(); break;
					case 6: EncerrarExpediente(); break;
					case 7: Consularfaturamento(); break;
					case 8: break;        
					default: printf("\n ERRO: Opcao Invalida! \7\n");
					break;
				}
				
		}
	while (Opcao!=8);
	
	fclose (RgMotorista);
	
	return 0;
  }

