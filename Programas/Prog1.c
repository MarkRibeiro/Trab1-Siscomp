#include<stdio.h>
#include<unistd.h> 
int main ()
{
	int i;
	printf("-Prog1 Inicializou\n");
	for(i=0;i<7; i++)
	{
		printf("\tExecutanto Prog1...\n");
		sleep(1);
	}
	
	printf("-Prog1 Finalizou\n");
	return 0;
}
