#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/shm.h>

#define MAXPROC 4
#define IO 3

const int QUANTUM[3] = { 1, 2, 4};

/*
    Variáveis Globais
*/
int nproc_ativos = MAXPROC;

typedef struct processo{
    char nome[20];
    int pid;
    int rajada[3];
    int nivel;
    int rajadaAtual;
}Processo;

void interpretador (FILE *arq, Processo *p)
{
    char exec[4];
    int i;

    for(i=0;!feof(arq);i++)
    {
        fscanf(arq, "%s %s %d %d %d\n", exec, p[i].nome, &p[i].rajada[0], &p[i].rajada[1], &p[i].rajada[2]);
        p[i].nivel = 1; // <=== MUDANDO AQUI PARA TESTAR, MARK
        p[i].rajadaAtual = 0; // <=== MUDANDO AQUI PARA TESTAR, MARK
    }
}

char* getNomeByPid (int pid, Processo *p)
{
	int i;
	
	for(i = 0 ; i < MAXPROC ; i++)
	{
		if(p[i].pid == pid)
		{
			return p[i].nome;
		}
	}
	printf("\nNão achou nome\n\n");

	return NULL;
}

int getRajadaAtualByPid (int pid, Processo *p)
{
	int i;

	for(i = 0 ; i < MAXPROC ; i++)
	{
		if(p[i].pid == pid)
		{
			return p[i].rajadaAtual;
		}
	}
	
	printf("\nNão achou RajadaAtual\n\n");
	return -2;
}

int getRajadaByPid (int pid, Processo *p)
{
	int i;

	for(i = 0 ; i < MAXPROC ; i++)
	{
		if(p[i].pid == pid) 
		{
			return p[i].rajada[p[i].rajadaAtual];
		}
	}
	
	printf("\nNão achou Rajada\n\n");
	return -2;
}

/*

	AS:
		1 : TERMINOU A RAJADA DENTRO DO QUANTUM ATUAL E ERA A ULTIMA
		0 : TERMINOU A RAJADA ATUAL DENTRO DO QUANTUM MAS NAO ERA A ULTIMA
	   -1 : NÃO TERMINOU A RAJADA ATUAL DENTRO DO QUANTUM

*/
int atualizaRajada(int pid, Processo *p, int quantum)
{
	int i;

	for(i = 0 ; i < MAXPROC ; i++)
	{
		if(p[i].pid == pid)
		{ 
			p[i].rajada[p[i].rajadaAtual] = p[i].rajada[p[i].rajadaAtual] - quantum;
			
			if(p[i].rajada[p[i].rajadaAtual] <= 0) // SUCESSO: Terminou rajada dentro do quantum.
			{
				if(p[i].rajadaAtual == 2) // TERMINOU TODAS AS RAJADAS
				{
					return 1;
				}
				else // AINDA FALTAM RAJADAS VAI PRO FIM DA FILA
				{
					p[i].rajadaAtual++;
					return 0;
				}
			}
			else // FALHA: Não terminou a rajada dentro do quantum.
			{
				return -1;
			}
		}
	}
	printf("\nNão achou o pid\n\n");
	return -2;
}

/*

	PROCESSO PROMOVIDO

*/
void sobeNivel(int pid, Processo *p)
{
	int i;
	
	for(i = 0 ; i < MAXPROC ; i++)
	{
		if(p[i].pid == pid) p[i].nivel--;
	}
}

/*

	PROCESSO REBAIXADO

*/
void desceNivel(int pid, Processo *p)
{
	int i;
	
	for(i = 0 ; i < MAXPROC ; i++)
	{
		if(p[i].pid == pid) p[i].nivel++;
	}
}

/*void inicializaProcessos(Processo *p)
{
	int i;
	for(i = 0 ; i < MAXPROC ; i++)
	{
		fork();
		//execve();
		//p[i].pid = getpid();
	}
}*/

int getNivelByPid(int pid, Processo *p)
{
	int i;
	for(i = 0 ; i < MAXPROC ; i++)
	{
		if(p[i].pid == pid) return p[i].nivel;
	}
	return -2;
}

void escalonador(Processo *p)
{           
    int contadorFila1 = MAXPROC;
    int contadorFila2 = 0;
    int contadorFila3 = 0;
    int contaProcessoAtivo = MAXPROC;
    int existeFila1 = 1, existeFila2 = 0;
    char nome[20] = "nome";
    int rajada;
    int rajadaAtual;
    int nivel;
    int i;
    int resultado;
    
    p[0].pid = 1;
    p[1].pid = 2;
    p[2].pid = 3;
    p[3].pid = 4;
    
    int f1[MAXPROC], f2[MAXPROC], f3[MAXPROC];

    for(i=0;i<MAXPROC;i++)
    {
        f1[i]= p[i].pid;
    }
    

    int frenteFila;

    //while( contaProcessoAtivo != 0 )
    //{
        frenteFila = 0;
        while(contadorFila1 != 0)
        {
                //printf("\nALERT: Número de processos na fila 1: %d\n", contadorFila1);
                //printf("ALERT: Frente fila atual: %d\n\n", frenteFila);
                
                if(f1[frenteFila] == -1)
                {
                	frenteFila++;
                }
                
             	strcpy(nome, getNomeByPid(f1[frenteFila], p));
           	rajada		= getRajadaByPid(f1[frenteFila], p);
                rajadaAtual 	= getRajadaAtualByPid(f1[frenteFila], p);
                nivel 		= getNivelByPid(f1[frenteFila], p);

                printf( "\n===> (!) %s vai para a frente da fila 1 com %d segundos de execucao restantes", nome, rajada );
                printf( "\n===> (!) Tem %d segundos para terminar\n", QUANTUM[0]);
                
                resultado = atualizaRajada(f1[frenteFila], p, QUANTUM[0]);
                
                if(resultado == 1) // SUCESSO: Terminou rajada dentro do quantum da fila1 e terminou todas as rajadas
                {
                	printf( "===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+1 );
                	printf( "\t Programa completou todas suas tarefas!\n\t Finalizar programa!\n\n" );
                	//if(!kill(pid, SIGCHLD))
                	contaProcessoAtivo--;
                	contadorFila1--;
                }
                else if(resultado == 0) // SUCESSO: Terminou rajada dentro do quantum da fila1 mas NÃO terminou todas as rajadas, VAI PRO FIM DA FILA
                {
                    printf("===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+2 );
                    printf("\t %s foi para a rajada numero %d\n", nome, rajadaAtual+1);
                    printf("\t %s foi para o fim da fila", nome);
                    printf("\n\t ...Simula E/S...\n");
                    sleep(IO);
                }
                else // REBAIXADO: Não terminou dentro do quantum da fila1, vai pra fila2
                {
                    desceNivel(f1[frenteFila], p);
                    printf("===> (-) Esgotou o quantum do nivel 1! Desceu para o nivel %d\n", getNivelByPid(f1[frenteFila], p) );
                    printf("\t Processo %s sai da fila 1 e vai para a 2.\n", nome);
                    contadorFila2++;
                    f2[contadorFila2-1] = f1[frenteFila];
                    f1[frenteFila] = -1;
                    contadorFila1--;
                }

                frenteFila++;

                if(frenteFila >= MAXPROC || contadorFila1 == 1) frenteFila = 0; 
        }
    //}
}


int main (int argc, char* argv[])
{
    int shm;
    Processo *p = (Processo*) malloc (MAXPROC * sizeof(Processo));
    FILE *arq;
    arq = fopen("exec.txt", "r");

    shm = shmget(IPC_PRIVATE, sizeof(Processo)*4, 0666 | IPC_CREAT);

    p = (Processo*)shmat(shm,0,0);

    interpretador(arq, p);

    int contadorFila1 = MAXPROC;

    escalonador(p);

}
