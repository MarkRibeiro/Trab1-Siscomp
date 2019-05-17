#include<stdio.h>
#include<unistd.h> 
int main ()
{
	int i, f = 1;
	printf("Iniciou\n");
	for(i=20;i>1;i--)
	{
		f=f*i;
	}
	
	printf("Finalizou\n");
	return 0;
}
