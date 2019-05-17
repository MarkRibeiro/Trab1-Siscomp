#include<stdio.h>
#include<unistd.h> 
int main (int argc, char *argv[])
{
	int i, j;
	int aux;
	
	printf("Iniciou\n");
	for(i=1;i<argc;i++)
	{
		aux=argv[i]-48;
		for(j=0;j<aux; j++)
		{
			printf("Prog1\n");
			sleep(1);
		}
		printf("E/S\n");
		sleep(3);
	}
	printf("Finalizou\n");
	return 0;
}
