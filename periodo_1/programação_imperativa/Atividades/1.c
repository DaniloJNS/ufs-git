#include <stdio.h>
 
int main() {
float A, B, C, D, exame, media, Media;
scanf("%f", &A);
scanf("%f", &B);
scanf("%f", &C);
scanf("%f", &D);
media = (A*2 + B*3 + C*4 + D*1)/ 10;
printf("Media: %.1f\n", media);
if(media>=7)
printf("Aluno aprovado.\n");
if(media<5)
printf("Aluno reprovado.\n");
if (media>=5 && media <7)
{
printf("Aluno em exame.\n");
scanf("%f", &exame);
printf("Nota do exame: %.1f\n", exame);
Media= (exame + media)/2;
if(Media>=5)
printf("Aluno aprovado.\n");
if(Media<5)
printf("Aluno reprovado.\n");
printf("Media final: %.1f\n", Media);
} 
    return 0;
}
