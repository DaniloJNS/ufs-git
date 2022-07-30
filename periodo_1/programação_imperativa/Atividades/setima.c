#include <stdio.h>
int  main()
{
int  Cont;
float venda, parcial, porcentagem, montante;
parcial=0;
porcentagem = 0.135;

for
  (Cont=1;  Cont<=35;  Cont=Cont+1)
{
printf("Digite  um  valor da venda feita pelo departamento %d:  ", Cont);
scanf("%f",&venda);
if   (venda>= 1750)
{
parcial = venda * porcentagem;
montante= parcial+montante;
}
}
printf(" Montante a ser destina a ONG eh: %.2f.",montante);
}
