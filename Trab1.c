#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>

#define MAXPROC 4

const int QUANTUM[3] = { 1, 2, 4};

/*
	Variáveis Globais
*/
int nproc_ativos = MAXPROC;

typedef struct processo{
	char nome[20];
	int rajada[3];
	int nivel;
	int rajadaAtual;
	struct processo *prox;
}Processo;



void interpretador (FILE *arq, Processo *p)
{
	char lixo[4];
	int i;

	for(i=0;!feof(arq);i++)
	{
		fscanf(arq, "%s %s %d %d %d\n", lixo, p[i].nome, &p[i].rajada[0], &p[i].rajada[1], &p[i].rajada[2]);
		p[i].nivel = 1;
		p[i].rajadaAtual = 0;
		p[i].prox=NULL;
		if(i!=0)
		{
			p[i-1].prox=&p[i];
		}
	}
}

void escalonador(Processo *p)
{
	Processo *f1=NULL, *f2=NULL, *f3=NULL;

	while(p!=NULL)
	{
		p->rajada[p->rajadaAtual] =p->rajada[p->rajadaAtual] - QUANTUM[p->rajadaAtual];
		printf("%s Unidades de tempo restantes: %d ut - Nível: %d\n", p->nome, p->rajada[p->rajadaAtual], p->nivel);

		if(p->rajada[p->rajadaAtual] > 0)
		{
			// Não completou no quantum do nível => Díminiu 1 nível
			p->nivel++;

			printf("===> (-) Esgotou o quantum do nivel! ");

			if(p->nivel > 3)
			{
				p->nivel--;
			}
			else
			{
				printf("Desceu para o nivel %d", p->nivel);
			}
			printf("\n\n");
		}
		else
		{
			printf("===> (+) Completou dentro do quantum\n\n");
		}

		final = p;
		inicio = p->prox;
		final->prox=NULL;

		if(p->prox!=NULL)
		{
			p=p->prox;
		}
		p->prox=final;

		p=inicio;
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
	escalonador(p);

}