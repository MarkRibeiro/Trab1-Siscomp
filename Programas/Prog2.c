#include<stdio.h>
#include<unistd.h> 
int main ()
{
	int i;
	
	printf("Iniciou\n");
	for(i=0;i<20; i++)
	{
		printf("Prog2\n");
		sleep(1);
	}
	
	printf("Finalizou\n");
	return 0;
}
