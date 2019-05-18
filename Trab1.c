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
		p[i].nivel = 1; // <=== MUDANDO AQUI PARA TESTAR, MARK
		p[i].rajadaAtual = 0; // <=== MUDANDO AQUI PARA TESTAR, MARK
		p[i].prox=NULL;
		if(i!=0)
		{
			p[i].nivel = 1; // <=== MUDANDO AQUI PARA TESTAR, MARK
			p[i].rajadaAtual = 0; // <=== MUDANDO AQUI PARA TESTAR, MARK
			p[i-1].prox=&p[i];
		}
	}
}

void escalonador(Processo *p)
{
	Processo *f1=NULL, *f2=NULL, *f3=NULL, *final, *inicio;
	int i;

	f1=p; // <=== MUDANDO AQUI PARA TESTAR, MARK

	/* FILA 1 */
	//printf("[FILA 1]\n\n");	
	
	while(f1!=NULL)
	{	
		f1->rajada[f1->rajadaAtual] = f1->rajada[f1->rajadaAtual] - QUANTUM[f1->nivel-1];
		printf("%s - Tempo restante: %d Segundos - Nível: %d\n", f1->nome, f1->rajada[f1->rajadaAtual], f1->nivel);

		if(f1->rajada[f1->rajadaAtual] > 0) // REBAIXADO => Não completou dentro do quantum do nível 1
		{	
			f1->nivel++;
			printf("===> (-) Esgotou o quantum do nivel 1! Desceu para o nivel %d\n\n", f1->nivel);
			// TIRA O PROCESSO DA FILA 1 E COLOCA NA FILA 2
		}

		else
		{
			printf("===> (+) Completou a rajada numero %d dentro do quantum! ", f1->rajadaAtual+1);
			
			if(f1->rajadaAtual == 2) // Estava na ultima rajada: terminou todas as tarefas do processo
			{
				printf("\n\t Programa completou todas suas tarefas!\n\t Finalizar programa!\n\n"); // Enviar sinal ao processo PAI para finalizar o processo
			}
			else // Não estava na ultima rajada: VAI PRO FIM DA FILA
			{
				f1->rajadaAtual++;
				printf("\n\t ...Simula E/S... (3 segundos depois)...\n"); // Enviar sinal ao processo PAI para dormir 3 segundos ( sleep(3) )
				printf("\t %s foi para a rajada numero %d\n", f1->nome, f1->rajadaAtual+1);
				printf("\t %s foi para o fim da fila\n\n", f1->nome);
				// Vai pro final da fila1			
			}
		}

		f1 = f1->prox;
	}

	/* FILA 2 */
	//printf("[FILA 2]\n\n");	
	while(f2!=NULL)
	{
				

		f2->rajada[f2->rajadaAtual] =f2->rajada[f2->rajadaAtual] - QUANTUM[f2->nivel-1];
		printf("%s - Tempo restante: %d Segundos - Nível: %d\n", f2->nome, f2->rajada[f2->rajadaAtual], f2->nivel);

		if(f2->rajada[f2->rajadaAtual] > 0) // REBAIXADO => Não completou dentro do quantum do nível 2
		{
			f2->nivel++;

			printf("===> (-) Esgotou o quantum do nivel 2! Desceu para o nivel %d\n\n", f2->nivel);
			// TIRA O PROCESSO DA FILA 2 E COLOCA NA FILA 3
		}
		else
		{


			printf("===> (+) Completou a rajada numero %d dentro do quantum! ", f2->rajadaAtual+1);
			
			if(f2->rajadaAtual == 2) // Estava na ultima rajada: terminou todas as tarefas do processo
			{
				printf("\n\t Programa completou todas suas tarefas!\n\t Finalizar programa!\n\n"); // Enviar sinal ao processo PAI para finalizar o processo
			}
			else // Não estava na ultima rajada: ainda restam rajadas a serem executas
			{
				// PROMOVIDO => Completou dentro do quantum do nível 2
				f2->nivel--;
				f2->rajadaAtual++;
				printf("Subiu para o nivel %d\n", f2->nivel);
				printf("\t ...Simula E/S... (3 segundos depois)...\n"); // Enviar sinal ao processo PAI para dormir 3 segundos ( sleep(3) )
				printf("\t %s foi para a rajada numero %d\n", f2->nome, f2->rajadaAtual+1);
				printf("\t -Sai do loop 2 e vai pro loop 1-\n\n");
				// Aqui haverá um break; e Deverá ir para o loop do nível 1

			}
		}

		f2 = f2->prox;
	}
	
	/* FILA 3 */
	printf("[FILA 3]\n\n");	
	while(f3!=NULL)
	{
				

		f3->rajada[f3->rajadaAtual] =f3->rajada[f3->rajadaAtual] - QUANTUM[f3->nivel-1];
		printf("%s - Tempo restante: %d Segundos - Nível: %d\n", f3->nome, f3->rajada[f3->rajadaAtual], f3->nivel);

		if(f3->rajada[f3->rajadaAtual] > 0) // VAI PRO FIM DA FILA => Não completou dentro do quantum do nível 3
		{
			printf("===> (-) Esgotou o quantum do nivel 3! Vai pro fim da fila 3\n\n");
			// BOTA O PROCESSO NO FIM DA FILA
		}
		else
		{
			printf("===> (+) Completou a rajada numero %d dentro do quantum! ", f3->rajadaAtual+1);
			
			if(f3->rajadaAtual == 2) // Estava na ultima rajada: terminou todas as tarefas do processo
			{
				printf("\n\t Programa completou todas suas tarefas!\n\t Finalizar programa!\n\n"); // Enviar sinal ao processo PAI para finalizar o processo
			}
			else // Não estava na ultima rajada: ainda restam rajadas a serem executas
			{
				// PROMOVIDO => Completou dentro do quantum do nível 3
				f3->nivel--;
				f3->rajadaAtual++;
				printf("Subiu para o nivel %d\n", f3->nivel);
				printf("\t ...Simula E/S... (3 segundos depois)...\n"); // Enviar sinal ao processo PAI para dormir 3 segundos ( sleep(3) )
				printf("\t %s foi para a rajada numero %d\n", f3->nome, f3->rajadaAtual+1);
				printf("\t -Sai do loop 3 e vai pro loop 2-\n\n");
				// Aqui haverá um break; e Deverá ir para o loop do nível 2

			}
		}

		f3= f3->prox;
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
