#include <stdio.h>
int main()
{
	float Nota;
	short int av, aluno, total;
	for(aluno=1; aluno<=150; aluno++)
	{
		total=0;
		for(av=1; av<=12; av++)
		{
		printf("digite a nota do aluno:\n");
		scanf("%f", &Nota);
		if(Nota>=6)
		total= total +1;
		}
		if(total>=7)
		printf(" Aluno aprovado\n");
		else
		printf("aluno reprovado\n");
	}
	return 0;
}
