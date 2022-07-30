#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ARQUIVO_INICIO 0
#define ARQUIVO_FIM 2

#define true 1
#define false 0

#define Z 0x03E00000
#define X 0x001F0000
#define Y 0x0000F800
#define I 0x0000001F

#define DeslocZ 21
#define DeslocX 16
#define DeslocY 11
#define DeslocI 0

#define Register1 5
#define Register2 6
#define Register3 7
#define Register4 8

#define sum 30
#define sub 31
#define mul 32
#define sll 33
#define muls 34
#define sla 35
#define div 36
#define srl 37
#define divs 38
#define sra 39
#define cmp 40

#define max_memory 32
#define double_max_memory 64




struct memory_flash
{
    int DATA;
    void *prox;
    void *prev;
};

typedef struct memory_flash Tpmemory_flash;

Tpmemory_flash *begin, *instruction, *ultimate;

struct memory_register
{
    int DATA;
    int Number;
    int ZN;
    int SN;
    int OV;
    int CY;
    void *prox;
    void *prev;
};

typedef struct memory_register Tpmemory_register;

Tpmemory_register *Rbegin, *Rinstruction, *Rultimate;

int Rverificador, R1[max_memory],  R2[max_memory], R3[max_memory], R4[max_memory], R5[2*max_memory], R6[2*max_memory];




void atribute_register(int signal, int ZN){

    if(ZN == true)
    {
        Rinstruction->DATA = 0x00000000;
        Rinstruction->SN = 0;
        Rinstruction->ZN = 1;
    }
    else
    {
        if(signal == true)
        {
            if((instruction->DATA & 0x00100000) >> 20 == 0x00000001)
                Rinstruction->DATA = (instruction->DATA | 0xFFE00000);
            else
                Rinstruction->DATA = (instruction->DATA & 0x001FFFFF);
            Rinstruction->SN = 1;
            Rinstruction->ZN = 0;
            Rinstruction->CY = 0;
            Rinstruction->OV = 0;
        }
        else
        {
            Rinstruction->SN = 0;
            Rinstruction->DATA = (instruction->DATA & 0x001FFFFF);
        }
    }

}

int seach_registers(int Registers, int Desloc){
    int i = 1;

    printf("\nInicializando procura...\n");
    if(Rverificador != 0){
        Rinstruction = Rbegin;
        while (Rinstruction != NULL)
        {
            printf("R[%d] = %X\n", i, Rinstruction->Number);
            if(Rinstruction->Number == ((instruction->DATA & Registers) >> Desloc))
            {
                printf("Registro achado!!!!\n");
                printf("Fim da busca...\n\n");
                    return true;
            }
            Rinstruction = Rinstruction->prox;
            i++;
        }
        return false;
    }
    else
        return false;
    
}

int maneger_registers(int signal,  int ZN, int Register, int Desloc)
{

    if (Rverificador == 0)
    {
        Rinstruction = (Tpmemory_register *) malloc(sizeof(Tpmemory_register));

        atribute_register(signal, ZN);
        Rinstruction->Number = (instruction->DATA & Register) >> Desloc;
        Rinstruction->prox = NULL; 

        Rbegin=Rinstruction;
        Rbegin->prev = NULL;

        Rultimate=Rinstruction;
        Rverificador++;

        return true;

    }

    else
    {
 
        if (seach_registers(Register, Desloc) == false)
        {
            Rinstruction = (Tpmemory_register *) malloc(sizeof(Tpmemory_register));
            atribute_register(signal, ZN);
            Rinstruction->Number = (instruction->DATA & Register) >> Desloc;
            Rinstruction->prev=ultimate;
            Rultimate->prox = Rinstruction;
            Rultimate = Rinstruction;
            Rultimate->prox = NULL;    
            return true;    
        }
        else
            return false;

    }

}

int convertion_register( int Nregister)
{
    int aux, Desloc = 0x00000000;
    printf("---------------------------------\n\n");
    switch (Nregister)
    {
    case Register1:
        if(maneger_registers(true, true, X, DeslocX) == false)
        {
            for (int i = 0x00000000; i < max_memory; i++)
            {
                aux = pow(2,i);
                R1[i] = (Rinstruction->DATA & (Desloc+ aux)) >>i;
                printf("R1[%d] = %X | Desloc = %X\n", i, R1[i], Desloc+aux);
            }
            return true;
        }
        else
            return false;
        break;
    case Register2:
        if(maneger_registers(true, true, Y, DeslocY) == false)
        {
            for (int i = 0x00000000; i < max_memory; i++)
            {
                aux = pow(2,i);
                R2[i] = (Rinstruction->DATA & (Desloc+ aux)) >>i;
                printf("R2[%d] = %X | Desloc = %X\n", i, R2[i], Desloc+aux);
            }
            return true;
        }
        else
            return false;
    case Register3:
        if(maneger_registers(true, true, Z, DeslocZ) == false)
        {
            for (int i = 0x00000000; i < max_memory; i++)
            {
                aux = pow(2,i);
                R3[i] = (Rinstruction->DATA & (Desloc+ aux)) >>i;
                printf("R2[%d] = %X | Desloc = %X\n", i, R2[i], Desloc+aux);
            }
            return true;
        }
        else
            return false;
        break;               
    case Register4:
        if(maneger_registers(true, true, I, DeslocI) == false)
        {
            for (int i = 0x00000000; i < max_memory; i++)
            {
                aux = pow(2,i);
                R4[i] = (Rinstruction->DATA & (Desloc+ aux)) >>i;
                printf("R4[%d] = %X | Desloc = %X\n", i, R4[i], Desloc+aux);
            }
            return true;
        }
        else
            return false;
        break;         
    default:
        break;
    }

}



void operetions(int op)
{
    printf("Inicializando modulo operetions...\n");
    int Desloc = 0x00000000;
    int Carry = 0x00000000, aux, NumRegister, TpOperetion;
    double num;
    switch (op)
    {
    case sum:
        NumRegister = 3;
        TpOperetion =0;
        break;
    case sub:
        NumRegister = 3;
        TpOperetion =0;
        break;    
    case mul:
        NumRegister = 4;
        TpOperetion = 1;
        break;            
    case sll:
        NumRegister = 3;
        TpOperetion =2;
        break;
    case muls:
        NumRegister = 4;
        TpOperetion = 1;
        break;
    case sla:
        NumRegister = 3;
        TpOperetion = 2;
        break;           
    case div:
        NumRegister = 4;
        TpOperetion = 0;
        break;     
    case srl:
        NumRegister = 3;
        TpOperetion = 2;
        break;
    case divs:
        NumRegister = 4;
        TpOperetion = 0;
        break;  
    case sra:
        NumRegister = 3;
        TpOperetion = 2;
        break;                               
    
    default:
        break;
    }
    //printf("\nInicializando modulo de operacoes...\n");
    //printf("Decodificando Rx...\n");
    printf(" Inicializando a conversaode registro 1...\n");
    if (Register1 <= NumRegister)
    {
            if(convertion_register(Register1) == true);
            
            else
            maneger_registers(false, true, X, DeslocX);

    }
    printf("Fim da conversão de registros1... \n");
    printf(" Inicializando a conversao de registro 2...\n");
    if (Register2 <= NumRegister)
    {
            if(convertion_register(Register2) == true);            
            else
            maneger_registers(false, true, Y, DeslocY);
    }
    printf("Fim da conversão de registros2... \n");
    printf(" Inicializando a conversao de registro 3...\n");
    if (Register3 <= NumRegister)
    {
            if(convertion_register(Register3) == true);            
            else
            maneger_registers(false, true, Z, DeslocZ);
    }    
    printf("Fim da conversão de registros3... \n");
    printf(" Inicializando a conversao de registro 4...\n");
    if (Register4 <= NumRegister)
    {
            if(convertion_register(Register4) == true);            
            else
            maneger_registers(false, true, I, DeslocI);
    }                                  
    printf("Fim da conversão de registros4... \n");
    if(TpOperetion == 1)
    {
            for (int i = 0x00000000; i < max_memory; i++)
            {
                R5[i+max_memory] = R4[i];
                R5[i] = R3[i];
            }
    }
    else if(TpOperetion == 2)
    {
        
        for (int i = 0x00000000; i < max_memory; i++)
            {
                R6[i+max_memory] = R3[i];
                R6[i] = R1[i];

                R5[i+max_memory] = R3[i];
                R6[i] = R2[i];
            }
    }
    printf("Fim da conversão de registros... \n");
    //printf("\nDecodificado Ry...\n");

    //printf(" Ry = %d\n", (instruction->DATA & Y) >> DeslocY);
    
    //printf("Fim do modulo de operações...\n");
        
}


void SumAndSub(int op)
{
    //operetions(op);
    Rinstruction->DATA = 0x00000000;

    //printf("\nProcessando Rz...\n");
    printf("\n\n Processando a soma... \n\n");
    int temp, Carry = 0x00000000;
    for (int i = 0; i < max_memory; i++)
    {
        R3[i] = (R2[i] ^ R1[i]) ^ Carry;
        temp =Carry;
        Carry = (R1[i] & R2[i]) | (R1[i] & Carry) | (R2[i] &Carry);

        if(op == sum)
            Rinstruction->DATA = Rinstruction->DATA  | (R3[i] << i);

        if(op == sub)
            Rinstruction->DATA = Rinstruction->DATA  | (!R3[i] << i);
        printf(" Rz[%d]= %X + %X + %X = %X | Carry = %X | Data = 0x%08X\n", i,R1[i], R2[i], temp,R3[i], Carry, Rinstruction->DATA);
    }
    if(op == sub)    
        Rinstruction->DATA += 0x00000001;

        
}

void desloc(int op)
{
    Rinstruction->DATA = 0x00000000;

    operetions(op);
    int temp, Carry = 0x00000000;
    for (int i = 0; i < 2*max_memory; i++)
        R5[i]=0x0;
    
    for (int i = 0; i < max_memory; i++)
    {
        if(R2[i] == 1)
        for (int k = 0; k < max_memory; k++)
        {
            R5[k+i] = (R5[k+i] ^ R1[k]) ^ Carry;
            temp =Carry;
            Carry = (R1[i] & R5[i]) | (R1[i] & Carry) | (R5[i] & Carry);
        }
        //printf(" Rz[%d]= %X + %X + %X = %X | Carry = %X | Data = 0x%08X\n", i,Rx[i], Ry[i], temp,Rz[i], Carry, Rinstruction->DATA);
    }
    seach_registers(Z, DeslocZ);
    for (int i = 0x00000000; i < max_memory; i++)
        Rinstruction->DATA = Rinstruction->DATA  | (R5[i] << i);
    seach_registers(I, DeslocI);
    for (int i = 0x00000000; i < max_memory; i++)
        Rinstruction->DATA = Rinstruction->DATA  | (R5[max_memory+i] << i);
}
void decoding(FILE *output, int k){
    int decode = (instruction->DATA & 0x00000700) >> 8;
    int Rx, Ry, Ri, Rz, RzData, RiData;
    switch (decode)
    {
    case 0x0:
        desloc(mul);
        Rx = (instruction->DATA & X) >> DeslocX;
        Ry = (instruction->DATA & Y) >> DeslocY;
        Ri = (instruction->DATA & I) >> DeslocI;
        Rz = (instruction->DATA & Z) >> DeslocZ;

        if(seach_registers(Z, DeslocZ) == true)
        RzData = Rinstruction->DATA;
        
        if(seach_registers(I, DeslocI) == true)
        RiData = Rinstruction->DATA;

        printf("0x%08X:\t%s%d,%s%d,%s%d,%s%d\t\n %s%d:%s%d=%s%d*%s%d=0x%08X%08X", k,"mul r",Ri,"r",Rz,"r",Rx,"r",Ry,"R",Ri,"R",Rz,"R",Rx,"R",Ry, RiData, RzData);
        fprintf(output, "0x%08X:\t%s%d,%s%d,%s%d,%s%d\t\n %s%d:%s%d=%s%d*%s%d=0x%08X%08X", k,"mul r",Ri,"r",Rz,"r",Rx,"r",Ry,"R",Ri,"R",Rz,"R",Rx,"R",Ry, RiData, RzData);
        //printf("0x%08X:\t%-25s\t\n", k,"mul ru,rl,rx,ry");            
                                    
        break;
    case 0x1:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"sll rz,rx,ry,u");
        printf("0x%08X:\t%-25s\t\n", k,"sll rz,rx,ry,u");                                            
        break;        
    case 0x2:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"muls ru,rl,rx,ry");
        printf("0x%08X:\t%-25s\t\n", k,"muls ru,rl,rx,ry");                                        
        break;    
    case 0x3:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"sla rz,rx,ry,u");
        printf("0x%08X:\t%-25s\t\n", k,"sla rz,rx,ry,u");                                        
        break;    
    case 0x4:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"div rr,rz,rx,ry");
        printf("0x%08X:\t%-25s\t\n", k,"div rr,rz,rx,ry");                                        
        break;            
    case 0x5:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"srl rz,rx,ry,u");
        printf("0x%08X:\t%-25s\t\n", k,"srl rz,rx,ry,u");                                        
        break;    
    case 0x6:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"divs rr,rz,rx,ry");
        printf("0x%08X:\t%-25s\t\n", k,"divs rr,rz,rx,ry");                                        
        break;        
    case 0x7:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"sra rz,rx,ry,u");
        printf("0x%08X:\t%-25s\t\n", k,"sra rz,rx,ry,u");                                        
        break;               
	case 0x1F:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"cmp rx,sr");
        printf("0x%08X:\t%-25s\t\n", k,"cmp rx,sr");                                        
		break;        
    default:
        break;
    }

}



int main( int argc, char** argv ){

    if( argc > 1 )
    {
        // Declaramos um ponteiro(link para o endereço da memória) para o arquivo de nome: 'pf'

        for(int cont=0; cont < argc; cont++)
            printf("%d Parametro: %s\n", cont,argv[cont]);
        
        FILE* input = fopen(argv[1], "r");
	    FILE* output = fopen(argv[2], "w");

        unsigned int CODE = 0, verificador = 0, k=0;

        fseek(input,0,ARQUIVO_INICIO);

        begin=NULL;
        
        while(!feof(input))
        {
        
            fscanf(input,"%X", &CODE);

            instruction = (Tpmemory_flash *) malloc(sizeof(Tpmemory_flash));

            if (verificador == 0)
            {
                instruction->DATA = CODE;
                instruction->prox = NULL; 
                begin=instruction;
                begin->prev = NULL;
                ultimate=instruction;
                verificador++;
            }

            else
            {
                instruction->DATA = CODE;
                instruction->prev=ultimate;
                ultimate->prox = instruction;
                ultimate = instruction;
                ultimate->prox = NULL;
            }

            printf("0x%08X\n", instruction->DATA);

        }

        fclose(input);
    
        instruction = begin;
        Rverificador=0;

        while (instruction != NULL)
        {
            int response, Rx, Ry, Rz;
            unsigned int opcode = (instruction->DATA & 0xFC000000) >> 26;
            printf("\n opcode = %X || DATA = 0x%08X ==>   ", opcode, instruction->DATA);
            switch(opcode) {
                // OP = 000000
                case 0x00:
                    if(maneger_registers(false, false, Z, DeslocZ) == true)
                    {
                    fprintf(output, "0x%08X:\t%-25s%d,%d\t%s=0x%08X\n", k, "mov r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);
                    printf("0x%08X:\t%-25s%d,%d\t%s%d=0x%08X\n", k, "mov r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);
                    }
                    break;
                case 0x01:
                    if(maneger_registers(true, false, Z, DeslocZ) == true)
                    {
                    fprintf(output, "0x%08X:\t%-25s%d,%d\t%s=0x%08X\n", k, "movs r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);
                    printf("0x%08X:\t%-25s%d,%d\t%s%d=0x%08X\n", k, "movs r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);     
                    }
                    break;  
                case 0x02:
                     Rx = (instruction->DATA & X) >> DeslocX;
                     Ry = (instruction->DATA & Y) >> DeslocY;
                     Rz = (instruction->DATA & Z) >> DeslocZ;
                    SumAndSub(sum);
                    response = seach_registers(Z, DeslocZ);
                    fprintf(output, "0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d+%s%d=0x%08X", k,"add r",Rz,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);
                    printf("0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d+%s%d=0x%08X", k,"add r",Rz,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);                                        
                    break;
                case 0x03:
                    Rx = (instruction->DATA & X) >> DeslocX;
                    Ry = (instruction->DATA & Y) >> DeslocY;
                    Rz = (instruction->DATA & Z) >> DeslocZ;
                    SumAndSub(sub);
                    printf("\n\nVoltando a main... \n\n");
                    response = seach_registers(Z, DeslocZ);
                    fprintf(output, "0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d-%s%d=0x%08X", k,"sub r",Rz,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);
                    printf("0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d-%s%d=0x%08X", k,"sub r",Rz,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);                                        
                    break;
                case 0x04:
                    decoding(output, k);
                    break;                         
                case 0x05:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"cmp rx,sr");
                    printf("0x%08X:\t%-25s\t\n", k,"cmp rx,sr");               
                    break;               
                case 0x06:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"and rz,rx,ry");
                    printf("0x%08X:\t%-25s\t\n", k,"and rz,rx,ry");                     
                    break;     
                case 0x07:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"or rz,rx,ry");
                    printf("0x%08X:\t%-25s\t\n", k,"or rz,rx,ry");                    
                    break;    
                case 0x08:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"not rz,rx");
                    printf("0x%08X:\t%-25s\tX\n", k,"not rz,rx");                    
                    break;               
                case 0x09:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"xor rz,rx,ry");
                    printf("0x%08X:\t%-25s\t\n", k,"xor rz,rx,ry");                    
                    break;         
                // mudança de 0 para 1 nos primeiros quatros bits
                // e de tipo U para o F
                // operaçoes imediatas    
                case 0x12:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"addi rz,rx,s");
                    printf("0x%08X:\t%-25s\t\n", k,"addi rz,rx,s");                    
                    break;      
                case 0x13:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"subi rz,rx,s");
                    printf("0x%08X:\t%-25s\t\n", k,"subi rz,rx,s");                    
                    break;           
                case 0x14:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"muli rz,rx,s");
                    printf("0x%08X:\t%-25s\t\n", k,"muli rz,rx,s");                    
                    break;      
                case 0x15:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"divi rz,rx,s");
                    printf("0x%08X:\t%-25s\t\n", k,"divi rz,rx,s");                    
                    break;                  
                case 0x16:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"modi rz,rx,s");
                    printf("0x%08X:\t%-25s\t\n", k,"modi rz,rx,s");                    
                    break;    
                case 0x17:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"cmpi rx,s");
                    printf("0x%08X:\t%-25s\t\n", k,"cmpi rx,s");                    
                    break;      
                //operações de leituras
                case 0x18:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"l8 rz,[rx+-s]");
                    printf("0x%08X:\t%-25s\t\n", k,"l8 rz,[rx+-s]");                    
                    break;                   
                case 0x19:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"l16 rz,[rx+-s]");
                    printf("0x%08X:\t%-25s\t\n", k,"l16 rz,[rx+-s]");                    
                    break;     
                case 0x1A:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"l32 rz,[rx+-s]");
                    printf("0x%08X:\t%-25s\t\n", k,"l32 rz,[rx+-s]");                    
                    break;     
                case 0x1B:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"s8 [rx+-s],rz");
                    printf("0x%08X:\t%-25s\t\n", k,"s8 [rx+-s],rz");                    
                    break;    
                case 0x1C:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"s16 [rx+-s],rz");
                    printf("0x%08X:\t%-25s\t\n", k,"s16 [rx+-s],rz");                    
                    break;     
                case 0x1D:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"s32 [rx+-s],rz");
                    printf("0x%08X:\t%-25s\t\n", k,"s32 [rx+-s],rz");                    
                    break;      
                // mudança de 1 para 2 do primeiros bits em HEX
                // e do tipo F para o tipo S
                // operações de controle de fluxo
                case 0x2A:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bae s");
                    printf("0x%08X:\t%-25s\t\n", k,"bae s");                    
                    break;         
                case 0x2B:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bat s");
                    printf("0x%08X:\t%-25s\t\n", k,"bat s");                    
                    break;     
                case 0x2C:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bbe s");
                    printf("0x%08X:\t%-25s\t\n", k,"bbe s");                    
                    break;                              
                case 0x2D:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bbt s");
                    printf("0x%08X:\t%-25s\t\n", k,"bbt s");                    
                    break;     
                case 0x2E:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"beq s");
                    printf("0x%08X:\t%-25s\t\n", k,"beq s");                    
                    break;     
                case 0x2F:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bge s");
                    printf("0x%08X:\t%-25s\t\n", k,"bge s");                    
                    break;       
                case 0x30:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bgt s");
                    printf("0x%08X:\t%-25s\t\n", k,"bgt s");                    
                    break;     
                case 0x31:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"biv s");
                    printf("0x%08X:\t%-25s\t\n", k,"biv s");                    
                    break;     
                case 0x32:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"ble s");
                    printf("0x%08X:\t%-25s\t\n", k,"ble s");                    
                    break;     
                case 0x33:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"blt s");
                    printf("0x%08X:\t%-25s\t\n", k,"blt s");                    
                    break;     
                case 0x34:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bne s");
                    printf("0x%08X:\t%-25s\t\n", k,"bne s");                    
                    break;     
                case 0x35:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bni s");
                    printf("0x%08X:\t%-25s\t\n", k,"bni s");                    
                    break;     
                case 0x36:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bnz s");
                    printf("0x%08X:\t%-25s\t\n", k,"bnz s");                    
                    break;      
                case 0x37:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bun s");
                    printf("0x%08X:\t%-25s\t\n", k,"bun s");            
                    break;     
                case 0x38:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"bzd s");
                    printf("0x%08X:\t%-25s\t\n", k,"bzd s");                    
                    break;                                                                                                                                                                                                                                                                                                                        
                // OP = 011111
                case 0x1F:
                    fprintf(output, "0x%08X:\t%-25s\t\n", k,"int u ");
                    printf("0x%08X:\t%-25s\t\n", k,"int u ");                    
                    break;
                // OP = Invalido
                default:
                    printf("Codigo invalido!\n");
            }
            k+=4;
            instruction = instruction->prox;
        }

        fclose(output);
    }
    else
    {
        printf("Informe o arquivo. Ex.: ./program arquivo.txt\n");
    }

    return(0);
}