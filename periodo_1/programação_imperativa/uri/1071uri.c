#include <stdio.h>
int main()
{
int x, y, i, z; 
scanf ("%d %d", &x, &y); 
if (x < y)
{
	 
	 }
	  if (x == y)
	  { 
		  printf ("0\n"); 
		  }
		  else
		  { 
			  z=0;
			  for (i = y+1; i < x; i++)
			  {
			   if (i%2 !=0)
			   {
			    z = i + z; 
			   
			    }
			     
			}
			printf ("%d\n", z); 
		} 
return 0; 
}
