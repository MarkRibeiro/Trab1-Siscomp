#include<stdio.h>
#include<unistd.h> 
int main ()
{
	int i;
	printf("-Prog3 Inicializou\n");
	for(i=0;i<28; i++)
	{
		printf("\tExecutanto Prog3...\n");
		sleep(1);
	}
	
	printf("-Prog3 Finalizou\n");
	return 0;
}
