#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
 
typedef struct
{
    int codigo;
    char nome[200];
    float nota;
}Aluno;
 
Aluno aluno_especial;
 
int main()
{
    setlocale(LC_ALL,"portuguese");
 
    aluno_especial.codigo = 0;
    strcpy(aluno_especial.nome, "NULL");
    aluno_especial.nota = 0.0;
 
    printf(" \n O código do aluno especial é: %d ", aluno_especial.codigo);
    printf(" \n O nome do aluno especial é: %s ", aluno_especial.nome);
    printf(" \n A nota do aluno especial é: %.2f ", aluno_especial.nota);
    printf(" \n \n");
 
    printf(" Digite o código do aluno especial: ");
    scanf("%d%*c", &aluno_especial.codigo);
 
    printf(" Digite o nome do aluno especial: ");
    scanf("%s*c", &aluno_especial.nome[200]);
 
    printf(" Digite a nota do aluno especial: ");
    scanf("%f%*c", &aluno_especial.nota);
 
    printf(" \n O código do aluno especial é: %d ", aluno_especial.codigo);
    printf(" \n O nome do aluno especial é: %s ", aluno_especial.nome);
    printf(" \n A nota do aluno especial é: %.2f ", aluno_especial.nota);
    printf(" \n ");
 
    return 0;
}
