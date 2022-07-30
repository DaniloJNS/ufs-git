#include <stdio.h>
int  main()
{
int par,Cont;
par=0;
printf("serao exbidos os 25 primeiros numeros pares a partir do 2 e a soma entre eles");
for
  (Cont=2;  Cont<=50;  Cont=Cont+2)
{
printf("%d\n", Cont);
par = Cont + par;
}
printf(" a soma dos numeros exibidos eh: %d.",par);
}

