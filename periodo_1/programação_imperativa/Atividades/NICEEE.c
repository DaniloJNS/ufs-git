
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main()
{
	double d,m, c;
	int i , j, k, n,s;
	system("color F0");
	printf("tamanho da matriz: ");
	scanf("%d", &n);
	double M[n][n], B[n], X[n];
	for(i=0;i<=n-1;i++)
	{
		system("cls");
		for(j=0;j<=n-1;j++)
		{
			printf("\n 			M[%d][%d]=  ", i, j);
			scanf("%lf", &M[i][j]);
		}
			printf("\n 			B[%d]=  ", i);
			scanf("%lf", &B[i]);
	}
	if(M[0][0]==0)
	{
		for(i=0;i<=n-1;i++)
		{
			if(M[i][0]!=0)
			{
				c=B[i];
				B[i]=B[0];
				B[0]=c;
				for(j=0;j<=n-1;j++)
				{
					
					d=M[i][j];
					M[i][j]=M[0][j];
					M[0][j]=d;
				}
			break;
			}
		}
     }
     for(k=0;k<=n-1;k++)
     {
     for(i=1+k;i<=n-1;i++)
		{
			
			m=-(M[i][k]/M[k][k]);
				B[i]=B[i]+(m*B[k]);
				for(j=0+k;j<=n-1;j++)
				{
					M[i][j]=M[i][j]+(m*M[k][j]);
				}
		}
	  }
	system("cls");
	printf(" Matriz A:");
	for(i=0;i<=n-1;i++)
	{
			printf("\n\n");
			for(j=0;j<=n-1;j++)
			{
				printf("	%lf", M[i][j]);
			}
			printf("	%lf", B[i]);
	}
	
	X[n-1]=B[n-1]/M[n-1][n-1];
	
	for(k=n-2;k>=0;k--){
		s=0;
		for(j=k;j<=n-1;j++){
			s=s+M[k][j]*X[j];
			X[k]=(B[k]-s)/M[k][k];}}
			printf("\n\n");
			printf("vetor X: \n \n \n");
		for(j=0;j<=n-1;j++)
			{
				printf("	%lf\n\n", X[j]);
			}
	return 0;
}

