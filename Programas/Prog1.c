#include<stdio.h>
#include<unistd.h> 
int main ()
{
	int i;
	
	printf("Iniciou\n");
	for(i=0;i<15; i++)
	{
		printf("Prog1\n");
		sleep(1);
	}
	
	printf("Finalizou\n");
	return 0;
}
