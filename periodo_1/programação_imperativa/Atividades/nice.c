#include <stdio.h>
int  main()
{
int  Num, soma,Cont;
soma=0;
for
  (Cont=1;  Cont<=65;  Cont=Cont+1)
{
printf("Digite  um  valor:  ");
scanf("%d",&Num);
if   (Num)
soma= Num+soma;
}
printf(" a soma dos numeros lidos eh: %d.",soma);
}
