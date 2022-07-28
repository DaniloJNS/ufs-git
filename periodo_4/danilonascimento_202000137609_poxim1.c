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

#define left 100
#define right 101

#define max_memory 32
#define double_max_memory 64
#define lenght_register 26

#define IR 28
#define PC 29
#define SP 30
#define SR 31

struct memory_flash
{
    unsigned int DATA;
    void *prox;
    void *prev;
};

typedef struct memory_flash Tpmemory_flash;

Tpmemory_flash *begin, *instruction, *ultimate;

struct memory_register
{
   long int DATA;
    unsigned int SDATA;
};

typedef struct memory_register Tpmemory_register;

Tpmemory_register Register[max_memory];


unsigned int Rverificador, R1[max_memory],  R2[max_memory], R3[max_memory], R4[max_memory], R5[2*max_memory], R6[2*max_memory];

void init(){
    for (int i = 0; i < 32; i++)
        Register[i].DATA=0;
    
}
int opcode_variable (int decode, int desloc)
{
    return (instruction->DATA & decode) >> desloc; 
}

void setup_SR(int ZN/*zero*/, int ZD/*disião A/(B=0)*/, int SN/*signal*/, int OV/*overflow*/, int IV/*instrucao invalida*/, int CY/*carry*/)
{
    Register[SR].DATA =0X00000000;

    if(ZN == true)
        Register[SR].DATA = Register[SR].DATA | (ZN << 6);
    if(ZD == true)
        Register[SR].DATA = Register[SR].DATA | (ZD << 5);
    if(SN == true)
        Register[SR].DATA = Register[SR].DATA | (SN << 4);
    if(OV == true)
        Register[SR].DATA = Register[SR].DATA | (OV << 3);
    if(IV == true)
        Register[SR].DATA = Register[SR].DATA | (IV << 2);
    if(CY == true)
        Register[SR].DATA = Register[SR].DATA | (CY << 0);                                
}

void atribute_register(int signal, int ZN, int adress){

    if(ZN == true)
    {
        Register[adress].DATA = 0x00000000;
        /*Register[adress].SN = 0;
        Register[adress].ZN = 1;*/
    }
    else
    {
        if(signal == true)
        {
            if((instruction->DATA & 0x00100000) >> 20 == 0x00000001)
            {
                Register[adress].DATA = (instruction->DATA | 0xFFE00000);
            }
            else
                Register[adress].DATA = (instruction->DATA & 0x001FFFFF);
            /*Register[adress].SN = 1;
            Register[adress].ZN = 0;
            Register[adress].CY = 0;
            Register[adress].OV = 0;*/
        }
        else
        {
            /*Register[adress].SN = 0;*/
            Register[adress].DATA = (instruction->DATA & 0x001FFFFF);
        }
    }

}

int convertion_register( int Nregister)
{
    int aux, Desloc = 0x00000000, NumRegister = Register1;
    int adress;
    //printf("---------------------------------\n\n");
    while(Nregister--)
    {
        
        switch (NumRegister)
        {
            case Register1:
                adress = opcode_variable(X, DeslocX);
                    for (int i = 0x00000000; i < max_memory; i++)
                    {
                        aux = pow(2,i);
                        R1[i] = (Register[adress].DATA & (Desloc+ aux)) >> i;
                        R1[i] = R1[i] & 0x00000001;
                        //printf("R1[%d] = %08X | Desloc = %X\n", i, R1[i], Desloc+aux);
                    }
                break;
            case Register2:
                adress = opcode_variable(Y, DeslocY);;
                    for (int i = 0x00000000; i < max_memory; i++)
                    {
                        aux = pow(2,i);
                        R2[i] = (Register[adress].DATA & (Desloc+ aux)) >>i;
                        R2[i] = R2[i] & 0x00000001;
                        //printf("R2[%d] = %d | Desloc = %X\n", i, R2[i], Desloc+aux);
                    }
            case Register3:
                adress = opcode_variable(Z, DeslocZ);
                    for (int i = 0x00000000; i < max_memory; i++)
                    {
                        aux = pow(2,i);
                        R3[i] = (Register[adress].DATA & (Desloc+ aux)) >>i;
                        R3[i] = R3[i] & 0x00000001;
                        //printf("R3[%d] = %d | Desloc = %X\n", i, R3[i], Desloc+aux);
                    }
                break;               
            case Register4:
                adress = opcode_variable(I, DeslocI);
                    for (int i = 0; i < max_memory; i++)
                    {
                        aux = pow(2,i);
                        R4[i] = (Register[adress].DATA & (Desloc+ aux)) >> i;
                        R4[i] = R4[i] & 0x00000001;
                        //printf("R4[%d] = %d | Desloc = %X\n", i, R4[i], Desloc+aux);
                    }
                break;         
            default:
                break;
        }

        NumRegister++;
    }

}

void operetions(int TpOperetion)
{                  
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
        
}

void SumAndSub(int op)
{
    int adress = opcode_variable(Z, DeslocZ), R31;
    Register[adress].DATA = 0x00000000;

    int temp, Carry = 0x00000000;

    if(op == sum)
    {
        for (int i = 0; i < max_memory; i++)
        {
            R3[i] = (R2[i] ^ R1[i]) ^ Carry;

            temp =Carry;

            Carry = (R1[i] & R2[i]) | (R1[i] & Carry) | (R2[i] & Carry);

            Register[adress].DATA = Register[adress].DATA  | (R3[i] << i);

            //printf(" Rz[%d]= %X + %X + %X = %X | Carry = %X | Data = 0x%08X\n", i,R1[i], R2[i], temp,R3[i], Carry, Register[adress].DATA);
        }
        
        setup_SR(Register[adress].DATA == 0 ? true : false,false, R3[31] == 1 ? true : false, (R1[31] == R2[31]) && (R3[31] != R1[31]) ? true : false, false, Carry == 1 ? true : false);
        
    }

    if(op == sub)
    {
        if((R1[31] != R2[31]) && R1[31] == 1)
        {
            temp = 1;

            for (int i = 0; i < max_memory; i++)
            {
                R1[i] = !R1[i] ^ temp;
                temp = !R1[i] & temp;

            }

            Register[adress].DATA = 0x00000000;

            R1[31] = 0x00000001;

            for (int i = 0; i < max_memory; i++)
            {
                R3[i] = (R1[i] ^ R2[i]) ^ Carry;

                temp =Carry;

                Carry = (R1[i] & R2[i]) | (R1[i] & Carry) | (R2[i] & Carry);

                Register[adress].DATA = Register[adress].DATA  | ((!R3[i]&0x00000001) << i);

                //printf("\n Rz[%d]= %X + %X + %X = %X | Carry = %X | Data = 0x%08X\n", i,R1[i], R2[i], temp,R3[i], Carry, Register[adress].DATA);

            }
            Register[adress].DATA = Register[adress].DATA +1;

            R31 =(Register[adress].DATA >> 31) & 0x00000001;

            setup_SR(Register[adress].DATA == 0 ? true : false,false, R31 == 0x1 ? true : false, (R1[31] != R2[31]) && (R31 != R1[31]) ? true : false, false, Carry == 1 ? true : false);

        }

        else
        {
            temp = 1;

            for (int i = 0; i < max_memory; i++)
            {
                R2[i] = !R2[i] ^ temp;
                temp = !R2[i] & temp;

            }

            Register[adress].DATA = 0x00000000;

            for (int i = 0; i < max_memory; i++)
            {
                R3[i] = (R1[i] ^ R2[i]) ^ Carry;

                temp =Carry;

                Carry = (R1[i] & R2[i]) | (R1[i] & Carry) | (R2[i] & Carry);

                Register[adress].DATA = Register[adress].DATA  | (R3[i] << i);

                //printf("\n Rz[%d]= %X + %X + %X = %X | Carry = %X | Data = 0x%08X\n", i,R1[i], R2[i], temp,R3[i], Carry, Register[adress].DATA);

            }
            Register[adress].DATA = Register[adress].DATA +1;

            R31 =(Register[adress].DATA >> 31) & 0x00000001;

            setup_SR(Register[adress].DATA == 0 ? true : false,false, R31 == 0x1 ? true : false, (R1[31] != R2[31]) && (R31 != R1[31]) ? true : false, false, Carry == 1 ? true : false);
        }
    }
        

        
}

int multi(int SN,  int Ri, int Rz, int Rx, int Ry)
{
    unsigned long long int MEN =0x0;
    unsigned long long int filter;
    int aux,temp[max_memory];
    

    for (int i = 0; i < max_memory; i++)
        if(R2[i] == 1)
            MEN += Register[Rx].DATA << i;

    Register[Rz].DATA = (MEN);


    if(SN == false)
    {
        filter = (MEN ^ 0XFFFFFFFF00000000ULL) >> 32;
        
        filter += 0x1;

        Register[Ri].DATA = (!filter);
    }
    else
    {
        Register[Ri].DATA = MEN >> 32;
    }

    setup_SR((Register[Rz].DATA == 0) && (Register[Ri].DATA == 0) ? true : false, false, false, false ,false, Register[Ri].DATA != 0 ? true: false );

}

int divi(int SN,  int Ri, int Rz, int Rx, int Ry)
{
    if(Register[Ry].DATA != 0)
    {
    Register[Ri].DATA = Register[Rx].DATA % Register[Ry].DATA;
    Register[Rz].DATA = Register[Rx].DATA / Register[Ry].DATA;
    }
    else
     {
    Register[Ri].DATA = 0x0;
    Register[Rz].DATA = 0x0;
    }
}

void deslocing(int SN, int side, int Rz, int Ri,int Rx,int Ry)
{
    unsigned long long MEN = ((Register[Rz].DATA & 0xFFFFFFFF00000000) << max_memory) + ((Register[Ry].DATA & 0x00000000FFFFFFFF));
    
    if(side == left)
    MEN = MEN << (Ri+1);

    else
    {
        MEN = MEN >> (Ri+1);
    }

    Register[Rz].DATA = (MEN) >> 32;
    Register[Rx].DATA = MEN;

}

void instru_opcode(FILE *output,int adress, int k){
    int decode = (instruction->DATA & 0x00000700) >> 8;
    int Rx, Ry, Ri, Rz, RiData, RzData;
    switch (decode)
    {
    case 0x0:
        convertion_register(4); //X

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        multi(false, Ri, Rz, Rx, Ry);

        printf("0x%08X: %s%d,%s%d,%s%d,%s%-13d %s%d:%s%d=%s%d*%s%d=0x%08X%08X,SR=%08X\n", k,"mul r",Ri,"r",Rz,"r",Rx,"r",Ry,"R",Ri,"R",Rz,"R",Rx,"R",Ry, Register[Ri].DATA, Register[Rz].DATA, Register[SR].DATA);
        fprintf(output, "0x%08X: %s%d,%s%d,%s%d,%s%-13d %s%d:%s%d=%s%d*%s%d=0x%08X%08X,SR=%08X\n", k,"mul r",Ri,"r",Rz,"r",Rx,"r",Ry,"R",Ri,"R",Rz,"R",Rx,"R",Ry, Register[Ri].DATA, Register[Rz].DATA, Register[SR].DATA);
        //printf("0x%08X: % s \n", k,"mul ru,rl,rx,ry");            
                                    
        break;
    case 0x1:
        convertion_register(3);

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        deslocing(false, left, Rz, Ri, Rx, Ry);
        //sll rz,rx,ry,u
        fprintf(output, "0x%08X: sll r%d,r%d,r%d,%-14d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);
        printf("0x%08X: sll r%d,r%d,r%d,%-14d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);                                            
        break;        
    case 0x2:
        convertion_register(3);

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        multi(true, Ri, Rz, Rx, Ry);

        printf("0x%08X: %s%d,%s%d,%s%d,%s%-12d %s%d:%s%d=%s%d*%s%d=0x%08X%08X,SR=%08X\n", k,"muls r",Ri,"r",Rz,"r",Rx,"r",Ry,"R",Ri,"R",Rz,"R",Rx,"R",Ry, Register[Ri].DATA, Register[Rz].DATA, Register[SR].DATA);
        fprintf(output, "0x%08X: %s%d,%s%d,%s%d,%s%-12d %s%d:%s%d=%s%d*%s%d=0x%08X%08X,SR=%08X\n", k,"muls r",Ri,"r",Rz,"r",Rx,"r",Ry,"R",Ri,"R",Rz,"R",Rx,"R",Ry, Register[Ri].DATA, Register[Rz].DATA, Register[SR].DATA);                                
        break;    
    case 0x3:
        convertion_register(3);

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        deslocing(true, left, Rz, Ri, Rx, Ry);

        fprintf(output, "0x%08X: sla r%d,r%d,r%d,%-14d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);
        printf("0x%08X: sla r%d,r%d,r%d,%-14d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);                                        
        break;    
    case 0x4:
        convertion_register(3);

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        //divi(false, Ri, Rz, Rx, Ry);
        //div rr,rz,rx,ry
        fprintf(output, "0x%08X: div r%d,r%d,r%d,r%-13d R%d=R%d%sR%d=0x%08X,R%d=R%d/R%d=0x%08X\n", k, Ri, Rz, Rx, Ry, Ri, Rx, "%", Ry, Register[Ri].DATA, Rz, Rx, Ry, Register[Rz].DATA);
        printf("0x%08X: div r%d,r%d,r%d,r%-13d R%d=R%d%sR%d=0x%08X,R%d=R%d/R%d=0x%08X\n", k, Ri, Rz, Rx, Ry, Ri, Rx,"%", Ry, Register[Ri].DATA, Rz, Rx, Ry, Register[Rz].DATA);                                        
        break;            
    case 0x5:
        convertion_register(3);

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        deslocing(false, right, Rz, Ri, Rx, Ry);    
        fprintf(output, "0x%08X: srl r%d,r%d,r%d,%-13d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);
        printf("0x%08X: srl r%d,r%d,r%d,%-13d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);                                        
        break;    
    case 0x6:
        convertion_register(3);

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        divi(true, Ri, Rz, Rx, Ry);
        //div rr,rz,rx,ry
        fprintf(output, "0x%08X: divs r%d,r%d,r%d,r%-10d R%d=R%d%sR%d=0x%08X,R%d=R%d/R%d=0x%08X\n", k, Ri, Rz, Rx, Ry, Ri, Rx, "%", Ry, Register[Ri].DATA, Rz, Rx, Ry, Register[Rz].DATA);
        printf("0x%08X: divs r%d,r%d,r%d,r%-10d R%d=R%d%sR%d=0x%08X,R%d=R%d/R%d=0x%08X\n", k, Ri, Rz, Rx, Ry, Ri, Rx,"%", Ry, Register[Ri].DATA, Rz, Rx, Ry, Register[Rz].DATA);                                        
        break;        
    case 0x7:
        convertion_register(3);

        Rx = opcode_variable(X, DeslocX);
        Ry = opcode_variable(Y, DeslocY);
        Ri = opcode_variable(I, DeslocI);
        Rz = opcode_variable(Z, DeslocZ);

        deslocing(true, right, Rz, Ri, Rx, Ry);    
        fprintf(output, "0x%08X: sra r%d,r%d,r%d,%-11d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);
        printf("0x%08X: sra r%d,r%d,r%d,%-11d R%d:R%d=R%d:R%d<<%d=0x%08X%08X \n", k,Rz, Rx, Ry,Ri,Rz, Rx, Rz, Ry, Ri+1, Register[Rz].DATA, Register[Rx].DATA);       
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

        int CODE = 0, verificador = 0, k=0;

        fseek(input,0,ARQUIVO_INICIO);

        begin=NULL;
        
        while(!feof(input))
        {
        
            fscanf(input,"%X", &CODE);
            int adress;
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

        fprintf(output, "[START OF SIMULATION]\n");
        init();
        while (instruction != NULL)
        {
            int response, Rx, Ry, Rz, Ri, R31;

            unsigned int opcode = (instruction->DATA & 0xFC000000) >> 26, adress = opcode_variable(Z, DeslocZ);

            Register[IR].DATA = opcode;
            Register[PC].DATA = k;
            Register[0].DATA = 0;

            printf("\n opcode = %X || DATA = 0x%08X ==>   ", opcode, instruction->DATA);
            switch(opcode) {
                // OP = 000000
                case 0x00:
                    atribute_register(false, false, adress);
                    fprintf(output, "0x%08X: %s%d,%-20d %s%d=0x%08X\n", k, "mov r",adress, Register[adress].DATA, "R", adress, Register[adress].DATA);
                    printf("0x%08X: %s%d,%-20d %s%d=0x%08X\n", k, "mov r",adress, Register[adress].DATA, "R", adress, Register[adress].DATA);

                    break;
                case 0x01:
                    atribute_register(true, false, adress);
                    fprintf(output, "0x%08X: %s%d,%-19d %s%d=0x%08X\n", k, "movs r",adress, Register[adress].DATA, "R", adress, Register[adress].DATA);
                    printf("0x%08X: %s%d,%-19d %s%d=0x%08X\n", k, "movs r",adress, Register[adress].DATA, "R", adress, Register[adress].DATA);     

                    break;  
                case 0x02:
                    Rx = opcode_variable(X, DeslocX);
                    Ry = opcode_variable(Y, DeslocY);
                    Rz = opcode_variable(Z, DeslocZ);

                    convertion_register(3);

                    SumAndSub(sum);

                    fprintf(output, "0x%08X: %s%d,%s%d,%s%-16d %s%d=%s%d+%s%d=0x%08X,SR=%08X\n", k,"add r",Rz,"r",Rx,"r",Ry, "R",adress,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);
                    printf("0x%08X: %s%d,%s%d,%s%-16d %s%d=%s%d+%s%d=0x%08X,SR=%08X\n", k,"add r",Rz,"r",Rx,"r",Ry, "R",adress,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);    
                    //printf("\nRz = %d + %d = %X\n\n", Register[adress-2].DATA, Register[adress-1].DATA, Register[adress].DATA);                                    
                    break;
                case 0x03:
                    Rx = opcode_variable(X, DeslocX);
                    Ry = opcode_variable(Y, DeslocY);
                    Rz = opcode_variable(Z, DeslocZ);

                    convertion_register(3);

                    SumAndSub(sub);
                    //Register[Rz].DATA = Register[Rx].DATA - Register[Ry].DATA;
                    fprintf(output, "0x%08X: %s%d,%s%d,%s%-16d %s%d=%s%d-%s%d=0x%08X,SR=%08X\n", k,"sub r",Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);
                    printf("0x%08X: %s%d,%s%d,%s%-16d %s%d=%s%d-%s%d=0x%08X,SR=%08X\n", k,"sub r",Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA); 
                    break;
                case 0x04:
                    instru_opcode(output, adress, k);
                    break;                         
                case 0x05:
                    Rx = opcode_variable(X, DeslocX);
                    Ry = opcode_variable(Y, DeslocY);
                    Rz = opcode_variable(Z, DeslocZ);

                    convertion_register(3);

                    SumAndSub(sub);
                    fprintf(output, "0x%08X: cmp ir,pc\t%-14s\tSR=0x%08X \n", k," ", Register[SR].DATA);
                    printf("0x%08X: cmp ir,sr\t%-14s\tSR=0x%08X \n", k," ", Register[SR].DATA);               
                    break;               
                case 0x06:
                    Rx = opcode_variable(X, DeslocX);
                    Ry = opcode_variable(Y, DeslocY);
                    Rz = opcode_variable(Z, DeslocZ);

                    Register[Rz].DATA = Register[Rx].DATA & Register[Ry].DATA;

                    R31 = (Register[Rz].DATA >>31) & 0x00000001;

                    setup_SR(Register[Rz].DATA == 0 ? true: false, false, R31 == 1 ? true : false, false, false, false);

                    fprintf(output, "0x%08X: and r%d,%s%d,%s%-15d %s%d=%s%d&%s%d=0x%08X,SR=%08X\n", k,Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);
                    printf("0x%08X: and r%d,%s%d,%s%-15d %s%d=%s%d&%s%d=0x%08X,SR=%08X\n", k,Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);                     
                    break;     
                case 0x07:
                    Rx = opcode_variable(X, DeslocX);
                    Ry = opcode_variable(Y, DeslocY);
                    Rz = opcode_variable(Z, DeslocZ);

                    Register[Rz].DATA = Register[Rx].DATA | Register[Ry].DATA;

                    R31 = (Register[Rz].DATA >>31) & 0x00000001;

                    setup_SR(Register[Rz].DATA == 0 ? true: false, false, R31 == 1 ? true : false, false, false, false);

                    fprintf(output, "0x%08X: or r%d,%s%d,%s%-16d %s%d=%s%d|%s%d=0x%08X,SR=%08X\n", k,Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);
                    printf("0x%08X: or r%d,%s%d,%s%-16d %s%d=%s%d|%s%d=0x%08X,SR=%08X\n", k,Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);                  
                    break;    
                case 0x08:
                    Rx = opcode_variable(X, DeslocX);
                    Rz = opcode_variable(Z, DeslocZ);   

                    Register[Rz].DATA = ~Register[Rx].DATA;            
                    
                    setup_SR(Register[Rz].DATA == 0 ? true: false, false, R31 == 1 ? true : false, false, false, false);

                    fprintf(output, "0x%08X: not r%d,r%-18d R%d=~R%d=0x%08X,SR=0x%08X\n", k, Rz, Rx, Rz, Rx, Register[Rz].DATA, Register[SR].DATA);
                    printf("0x%08X: not r%d,r%-14d R%d=~R%d=0x%08X,SR=0x%08X\n", k, Rz, Rx, Rz, Rx, Register[Rz].DATA, Register[SR].DATA);                    
                    break;               
                case 0x09:
                    Rx = opcode_variable(X, DeslocX);
                    Ry = opcode_variable(Y, DeslocY);
                    Rz = opcode_variable(Z, DeslocZ);
                    Register[Rz].DATA = Register[Rx].DATA & Register[Ry].DATA;
                    R31 = (Register[Rz].DATA >>31) ^ 0x00000001;

                    setup_SR(Register[Rz].DATA == 0 ? true: false, false, R31 == 1 ? true : false, false, false, false);

                    fprintf(output, "0x%08X: xor r%d,%s%d,%s%-14d %s%d=%s%d^%s%d=0x%08X,SR=%08X\n", k,Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);
                    printf("0x%08X: xor r%d,%s%d,%s%-14d %s%d=%s%d^%s%d=0x%08X,SR=%08X\n", k,Rz,"r",Rx,"r",Ry, "R",adress ,"R",Rx,"R",Ry, Register[adress].DATA, Register[SR].DATA);                 
                    break;         
                // mudança de 0 para 1 nos primeiros quatros bits
                // e de tipo U para o F
                // operaçoes imediatas    
                case 0x12:
                    Rx = opcode_variable(X, DeslocX);
                    Ri = opcode_variable(I, DeslocI);
                    Rz = opcode_variable(Z, DeslocZ);

                    Register[Rz].DATA = Register[Rx].DATA + Ri;

                    fprintf(output, "0x%08X: addi r%d,r%d,%-14d R%d=R%d+0x%08X=0x%08X,SR=0x%08X\n", k, Rz, Rx, Ri, Rz, Rx, Ri, Register[Rz].DATA, Register[SR].DATA);
                    printf("0x%08X: addi r%d,r%d,%-14d R%d=R%d+0x%08X=0x%08X,SR=0x%08X\n", k, Rz, Rx, Ri, Rz, Rx, Ri, Register[Rz].DATA, Register[SR].DATA);                    
                    break;      
                case 0x13:
                    Rx = opcode_variable(X, DeslocX);
                    Ri = opcode_variable(I, DeslocI);
                    Rz = opcode_variable(Z, DeslocZ);

                    Register[Rz].DATA = Register[Rx].DATA - Ri;

                    fprintf(output, "0x%08X: subi r%d,r%d,%-14d R%d=R%d-0x%08X=0x%08X,SR=0x%08X\n", k, Rz, Rx, Ri, Rz, Rx, Ri, Register[Rz].DATA, Register[SR].DATA);
                    printf("0x%08X: subi r%d,r%d,%-14d R%d=R%d-0x%08X=0x%08X,SR=0x%08X\n", k, Rz, Rx, Ri, Rz, Rx, Ri, Register[Rz].DATA, Register[SR].DATA);                    
                    break;           
                case 0x14:
                    fprintf(output, "0x%08X: %s \n", k,"muli rz,rx,s");
                    printf("0x%08X: %s \n", k,"muli rz,rx,s");                    
                    break;      
                case 0x15:
                    fprintf(output, "0x%08X: %s \n", k,"divi rz,rx,s");
                    printf("0x%08X: %s \n", k,"divi rz,rx,s");                    
                    break;                  
                case 0x16:
                    fprintf(output, "0x%08X: %s \n", k,"modi rz,rx,s");
                    printf("0x%08X: %s \n", k,"modi rz,rx,s");                    
                    break;    
                case 0x17:
                    fprintf(output, "0x%08X: %s \n", k,"cmpi rx,s");
                    printf("0x%08X: %s \n", k,"cmpi rx,s");                    
                    break;      
                //operações de leituras
                case 0x18:
                    fprintf(output, "0x%08X: %s \n", k,"l8 rz,[rx+-s]");
                    printf("0x%08X: %s \n", k,"l8 rz,[rx+-s]");                    
                    break;                   
                case 0x19:
                    fprintf(output, "0x%08X: %s \n", k,"l16 rz,[rx+-s]");
                    printf("0x%08X: %s \n", k,"l16 rz,[rx+-s]");                    
                    break;     
                case 0x1A:
                    fprintf(output, "0x%08X: %s \n", k,"l32 rz,[rx+-s]");
                    printf("0x%08X: %s \n", k,"l32 rz,[rx+-s]");                    
                    break;     
                case 0x1B:
                    fprintf(output, "0x%08X: %s \n", k,"s8 [rx+-s],rz");
                    printf("0x%08X: %s \n", k,"s8 [rx+-s],rz");                    
                    break;    
                case 0x1C:
                    fprintf(output, "0x%08X: %s \n", k,"s16 [rx+-s],rz");
                    printf("0x%08X: %s \n", k,"s16 [rx+-s],rz");                    
                    break;     
                case 0x1D:
                    fprintf(output, "0x%08X: %s \n", k,"s32 [rx+-s],rz");
                    printf("0x%08X: %s \n", k,"s32 [rx+-s],rz");                    
                    break;      
                // mudança de 1 para 2 do primeiros bits em HEX
                // e do tipo F para o tipo S
                // operações de controle de fluxo
                case 0x2A:
                    fprintf(output, "0x%08X: % s \n", k,"bae s");
                    printf("0x%08X: %s \n", k,"bae s");                    
                    break;         
                case 0x2B:
                    fprintf(output, "0x%08X: % s \n", k,"bat s");
                    printf("0x%08X: %s \n", k,"bat s");                    
                    break;     
                case 0x2C:
                    fprintf(output, "0x%08X: % s \n", k,"bbe s");
                    printf("0x%08X: %s \n", k,"bbe s");                    
                    break;                              
                case 0x2D:
                    fprintf(output, "0x%08X: % s \n", k,"bbt s");
                    printf("0x%08X: %s \n", k,"bbt s");                    
                    break;     
                case 0x2E:
                    fprintf(output, "0x%08X: % s \n", k,"beq s");
                    printf("0x%08X: %s \n", k,"beq s");                    
                    break;     
                case 0x2F:
                    fprintf(output, "0x%08X: % s \n", k,"bge s");
                    printf("0x%08X: %s \n", k,"bge s");                    
                    break;       
                case 0x30:
                    fprintf(output, "0x%08X: % s \n", k,"bgt s");
                    printf("0x%08X: %s \n", k,"bgt s");                    
                    break;     
                case 0x31:
                    fprintf(output, "0x%08X: % s \n", k,"biv s");
                    printf("0x%08X: %s \n", k,"biv s");                    
                    break;     
                case 0x32:
                    fprintf(output, "0x%08X: % s \n", k,"ble s");
                    printf("0x%08X: %s \n", k,"ble s");                    
                    break;     
                case 0x33:
                    fprintf(output, "0x%08X: % s \n", k,"blt s");
                    printf("0x%08X: %s \n", k,"blt s");                    
                    break;     
                case 0x34:
                    fprintf(output, "0x%08X: % s \n", k,"bne s");
                    printf("0x%08X: %s \n", k,"bne s");                    
                    break;     
                case 0x35:
                    fprintf(output, "0x%08X: % s \n", k,"bni s");
                    printf("0x%08X: %s \n", k,"bni s");                    
                    break;     
                case 0x36:
                    fprintf(output, "0x%08X: % s \n", k,"bnz s");
                    printf("0x%08X: %s \n", k,"bnz s");                    
                    break;      
                case 0x37:
                    Ri = opcode_variable(I, DeslocI);                   
                    fprintf(output, "0x%08X: bun %d \n", k, Ri);
                    printf("0x%08X: bun %08X \n", k, Ri); 

                    k+=Ri*4;   
                    Ri --;
                    if(Ri > 0)
                        while (Ri--)
                            instruction = instruction->prox;     
                    break;     
                case 0x38:
                    fprintf(output, "0x%08X: % s \n", k,"bzd s");
                    printf("0x%08X: %s \n", k,"bzd s");                    
                    break;                                                                                                                                                                                                                                                                                                                        
                // OP = 011111
                case 0x3F:
                    Ri = opcode_variable(I, DeslocI);
                    
                    printf("0x%08X: %s \n", k,"int u ");
                    if(Ri ==0)
                    {
                        while (instruction->prox != NULL)
                            instruction = instruction->prox;
                        Register[PC].DATA = 0X0;    
                    }

                    
                    fprintf(output, "0x%08X: int %-23d  CR=0x00000000,PC=0x%08X\n", k, Ri, Register[PC].DATA);
                                        
                    break;
                // OP = Invalido
                default:
                    break;
            }
            k+=4;
            instruction = instruction->prox;
        }
        fprintf(output, "[END OF SIMULATION]\n");

        fclose(output);
    }
    else
    {
        printf("Informe o arquivo. Ex.: ./program arquivo.txt\n");
    }

    return(0);
}
