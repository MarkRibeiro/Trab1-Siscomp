#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

typedef struct processo{
	char nome[20];
	int rajada;
}Processo;

typedef struct filas{
	int quantum;
	Processo p[3];
}Fila;

int main (int argc, char* argv[])
{

	Fila f[3];
	Processo p1, p2, p3;

	strcpy(p1.nome, "a");
	p1.rajada=1;

	strcpy(p2.nome, "b");
	p2.rajada=2;

	strcpy(p3.nome, "c");
	p3.rajada=4;

	f[0].quantum=1;
	f[0].p[0]=p1;

	f[1].quantum=1;
	f[1].p[0]=p2;

	f[2].quantum=1;
	f[2].p[0]=p3;

	execve(f[0].p[0].nome, argv, 0);
	execve(f[1].p[0].nome, argv, 0);
	execve(f[2].p[0].nome, argv, 0);	
	
	if(fork()!=0) //Pai: Escalonador
	{
		
	}
	
	else //Filho: Filas
	{	
		if(fork()!=0)
			execv(f[1].p[0].nome, argv);

		else
			execv(f[2].p[0].nome, argv);
	}
	
}