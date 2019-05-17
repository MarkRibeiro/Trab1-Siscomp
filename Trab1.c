#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>
#define KEY 5678

typedef struct processo{
	char nome[20];
	int rajada[3];
	int nivel;
}Processo;

typedef struct filas{
	int quantum;
	Processo p[3];
}Fila;

void interpretador (FILE *arq, Processo *p)
{
	char lixo[4];
	int i;

	for(i=0;!feof(arq);i++)
	{
		fscanf(arq, "%s %s %d %d %d\n", lixo, p[i].nome, &p[i].rajada[0], &p[i].rajada[1], &p[i].rajada[2]);
		
		p[i].nivel = 1;
	}
}

int main (int argc, char* argv[])
{
	int shm;
	Processo *p;
	FILE *arq;
	arq = fopen("exec.txt", "r");

	shm = shmget(IPC_PRIVATE, sizeof(Processo)*4, 0666 | IPC_CREAT);

	p = (Processo*)shmat(shm,0,0);

	interpretador(arq, p);

	printf("%s %d %d %d %d\n", p[0].nome, p[0].rajada[0], p[0].rajada[1], p[0].rajada[2], p[0].nivel);
}