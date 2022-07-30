#include <stdio.h>
int  main()
{
int  Num, soma,Cont;
soma=0;
for
  (Cont=1;  Cont<=150;  Cont=Cont+1)
{
printf("Digite  um  valor:  ");
scanf("%d",&Num);
if   (Num%3==0)
soma= Num+soma;
}
printf(" a soma dos numeros lidos multiplos por 3 eh: %d.",soma);
}
