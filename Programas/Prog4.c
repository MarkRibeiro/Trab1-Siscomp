#include<stdio.h>
#include<unistd.h> 
int main ()
{
	int i;
	printf("-Prog4 Inicializou\n");
	for(i=0;i<56; i++)
	{
		printf("\tExecutanto Prog4...\n");
		sleep(1);
	}
	
	printf("-Prog4 Finalizou\n");
	return 0;
}
