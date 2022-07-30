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

#define DeslocZ 21
#define DeslocX 16
#define DeslocY 11

#define sum 2
#define sub 3

#define max_memory 32




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

int Rverificador;



void type_instruction_U()
{




}

void type_instruction_F(){





}

void type_instruction_S(){
    char FREE[28];


}

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

    //printf("\nInicializando procura...\n");
    if(Rverificador != 0){
        Rinstruction = Rbegin;
        while (Rinstruction != NULL)
        {
            //printf("R[%d] = %X\n", i, Rinstruction->Number);
            if(Rinstruction->Number == ((instruction->DATA & Registers) >> Desloc))
            {
                //printf("Registro achado!!!!\n");
                //printf("Fim da busca...\n\n");
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

void maneger_registers(int signal,  int ZN){

    if (Rverificador == 0)
    {
        Rinstruction = (Tpmemory_register *) malloc(sizeof(Tpmemory_register));

        atribute_register(signal, ZN);
        Rinstruction->Number = (instruction->DATA & 0x03E00000) >> 21;
        Rinstruction->prox = NULL; 

        Rbegin=Rinstruction;
        Rbegin->prev = NULL;

        Rultimate=Rinstruction;
        Rverificador++;

    }

    else
    {
 
        if (seach_registers(Z, DeslocZ) == false)
        {
            Rinstruction = (Tpmemory_register *) malloc(sizeof(Tpmemory_register));
            atribute_register(signal, ZN);
            Rinstruction->Number = (instruction->DATA & 0x02E00000) >> 21;
            Rinstruction->prev=ultimate;
            Rultimate->prox = Rinstruction;
            Rultimate = Rinstruction;
            Rultimate->prox = NULL;        
        }

    }

}

void operetions(int op)
{
    int Rx[max_memory],  Ry[max_memory], Rz[max_memory], Desloc = 0x00000000;
    int Carry = 0x00000000, aux;;
    double num;

    //printf("\nInicializando modulo de operacoes...\n");
    //printf("Decodificando Rx...\n");
    if(seach_registers(X, DeslocX) == true)
        for (int i = 0x00000000; i < max_memory; i++)
        {
            aux = pow(2,i);
            Rx[i] = (Rinstruction->DATA & (Desloc+ aux)) >>i;
            //printf("Rx[%d] = %X | Desloc = %X\n", i, Rx[i], Desloc+aux);
        }
        
        
    //printf("\nDecodificado Ry...\n");

    //printf(" Ry = %d\n", (instruction->DATA & Y) >> DeslocY);
    if(seach_registers(Y, DeslocY) == true)
    {
        for (int i = 0; i < max_memory; i++)
        {
            aux = pow(2,i);
            Ry[i] = (Rinstruction->DATA & (Desloc+ aux)) >>i;
            //printf("Ry[%d] = %X | Desloc = %X\n", i, Ry[i], Desloc+aux);
        }
    }
    if( seach_registers(Z, DeslocZ) == false)
        maneger_registers(false, true);
    
    Rinstruction->DATA = 0x00000000;

    //printf("\nProcessando Rz...\n");

    int temp;
    for (int i = 0; i < max_memory; i++)
    {
        Rz[i] = (Rx[i] ^ Ry[i]) ^ Carry;
        temp =Carry;
        Carry = (Rx[i] & Ry[i]) | (Rx[i] & Carry) | (Ry[i] &Carry);
        if(op == sum)
            Rinstruction->DATA = Rinstruction->DATA  | (Rz[i] << i);
        if(op == sub)
            Rinstruction->DATA = Rinstruction->DATA  | (!Rz[i] << i);
        //printf(" Rz[%d]= %X + %X + %X = %X | Carry = %X | Data = 0x%08X\n", i,Rx[i], Ry[i], temp,Rz[i], Carry, Rinstruction->DATA);
    }
    if(op == sub)
    {
            
        Rinstruction->DATA += 0x00000001;
    }
    if (Rz[31] == 1)
        Rinstruction->SN = 1;
    else    
        Rinstruction->SN = 0;
    if(Rinstruction->DATA == 0x00000000)
        Rinstruction->ZN = 0;
    else
        Rinstruction->ZN = 1;
    if (Rx[31] == (Ry[31] & Rz[31]))
        Rinstruction->OV = 1;
    else
        Rinstruction->OV = 0;
    if(Carry ==1)
        Rinstruction->CY = 1;
    else
        Rinstruction->CY = 0;
    
    
    //printf("Fim do modulo de operações...\n");
        
}

void decoding(FILE *output, int k){
    int decode = (instruction->DATA & 0x00000700) >> 8;

    switch (decode)
    {
    case 0x0:
        fprintf(output, "0x%08X:\t%-25s\t\n", k,"mul ru,rl,rx,ry");
        printf("0x%08X:\t%-25s\t\n", k,"mul ru,rl,rx,ry");                                        
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
            int response, Rx, Ry;
            unsigned int opcode = (instruction->DATA & 0xFC000000) >> 26;
            printf("\n opcode = %X || DATA = 0x%08X ==>   ", opcode, instruction->DATA);
            switch(opcode) {
                // OP = 000000
                case 0x00:
                    maneger_registers(false, false);
                    fprintf(output, "0x%08X:\t%-25s%d,%d\t%s=0x%08X\n", k, "mov r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);
                    printf("0x%08X:\t%-25s%d,%d\t%s%d=0x%08X\n", k, "mov r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);
                    break;
                case 0x01:
                    maneger_registers(true, false);
                    fprintf(output, "0x%08X:\t%-25s%d,%d\t%s=0x%08X\n", k, "movs r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);
                    printf("0x%08X:\t%-25s%d,%d\t%s%d=0x%08X\n", k, "movs r",Rinstruction->Number, Rinstruction->DATA, "R", Rinstruction->Number, Rinstruction->DATA);     
                    break;  
                case 0x02:
                     Rx = (instruction->DATA & X) >> DeslocX;
                     Ry = (instruction->DATA & Y) >> DeslocY;
                     operetions(sum);
                    fprintf(output, "0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d+%s%d=0x%08X", k,"add r",Rinstruction->Number,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);
                    printf("0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d+%s%d=0x%08X", k,"add r",Rinstruction->Number,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);                                        
                    break;
                case 0x03:
                    Rx = (instruction->DATA & X) >> DeslocX;
                    Ry = (instruction->DATA & Y) >> DeslocY;
                    operetions(sub);
                    fprintf(output, "0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d-%s%d=0x%08X", k,"sub r",Rinstruction->Number,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);
                    printf("0x%08X:\t%s%d,%s%d,%s%d\t\n %s%d=%s%d-%s%d=0x%08X", k,"sub r",Rinstruction->Number,"r",Rx,"r",Ry, "R",Rinstruction->Number,"R",Rx,"R",Ry, Rinstruction->DATA);                                        
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