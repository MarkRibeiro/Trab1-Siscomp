#include<stdio.h>
#include<unistd.h> 
int main ()
{
	int i;
	printf("-Prog2 Inicializou\n");
	for(i=0;i<14; i++)
	{
		printf("\tExecutanto Prog2...\n");
		sleep(1);
	}
	
	printf("-Prog2 Finalizou\n");
	return 0;
}
