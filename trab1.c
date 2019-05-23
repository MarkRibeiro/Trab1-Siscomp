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
				p[i].rajadaAtual++;
				if(p[i].rajadaAtual > 2) // TERMINOU TODAS AS RAJADAS
				{
					return 1;
				}
				else // AINDA FALTAM RAJADAS VAI PRO FIM DA FILA
				{
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

int getFimFila(int frenteFila)
{
	if(frenteFila == 0)
	{
		return 3;
	}
	else
	{
		return frenteFila-1;
	}
}

void escalonador(Processo *p)
{           
    int contadorFila1 = MAXPROC;
    int contadorFila2 = 0;
    int contadorFila3 = 0;
    int contaProcessoAtivo = MAXPROC;
    int existeFila1 = 1, existeFila2 = 0;
    char nome[20] = "nome";
    int nivel;
    int rajada;
    int rajadaAtual;
    int i;
    int resultado;
    
    p[0].pid = 1;
    p[1].pid = 2;
    p[2].pid = 3;
    p[3].pid = 4;
    
    int f1[MAXPROC], f2[MAXPROC], f3[MAXPROC];

    for(i=0;i<MAXPROC;i++)
    {
        f1[i] = p[i].pid;
        f2[i] = -1;
        f3[i] = -1;
    }
    

    int frenteFila1 = 0, frenteFila2 = 0, frenteFila3 = 0;
    int fimFila1, fimFila2, fimFila3;
    int auxfrenteFila2 = 1;
    int auxfrenteFila3 = 1;

    while( contaProcessoAtivo != 0 )
    {
    
    	if(contadorFila1 != 0)
    	{
    		printf("\n+----------------------------------------------+\n");
        	printf("|		    FILA 1		       |\n");
        	printf("+----------------------------------------------+\n");
    	}
    
        while(contadorFila1 != 0)
        {
                if(f1[frenteFila1] == -1)
                {
                	frenteFila1++;
                }
                
             	strcpy(nome, getNomeByPid(f1[frenteFila1], p));
           	rajada		= getRajadaByPid(f1[frenteFila1], p);
                rajadaAtual 	= getRajadaAtualByPid(f1[frenteFila1], p);
                nivel 		= getNivelByPid(f1[frenteFila1], p);

                printf( "\n===> (!) %s vai para a frente da fila 1 com %d segundos de execucao restantes", nome, rajada );
                printf( "\n===> (!) Tem %d segundos para terminar\n", QUANTUM[0]);
                sleep(QUANTUM[0]);
                
                resultado = atualizaRajada(f1[frenteFila1], p, QUANTUM[0]);
                
                if(resultado == 1) // SUCESSO: Terminou rajada dentro do quantum da fila1 e terminou todas as rajadas
                {
                	printf( "===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+1 );
                	printf( "\t Programa completou todas suas tarefas!\n\t Finalizar programa!\n\n" );
                	//if(!kill(pid, SIGCHLD))
                	f1[frenteFila1] = -1;
                	contaProcessoAtivo--;
                	contadorFila1--;
                }
                else if(resultado == 0) // SUCESSO: Terminou rajada dentro do quantum da fila1 mas NÃO terminou todas as rajadas, VAI PRO FIM DA FILA
                {
                    printf("===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+1 );
                    printf("\t %s foi para a rajada numero %d\n", nome, getRajadaAtualByPid(f1[frenteFila1], p)+1);
                    printf("\t ...Simula E/S...\n");
                    sleep(IO);
                    printf("\t %s foi para o fim da fila\n", nome);
                }
                else // REBAIXADO: Não terminou dentro do quantum da fila1, vai pra fila2
                {
                    desceNivel(f1[frenteFila1], p);
                    printf("===> (-) Esgotou o quantum do nivel 1! Desceu para o nivel %d\n", getNivelByPid(f1[frenteFila1], p) );
                    printf("\t Processo %s sai da fila 1 e vai para a 2.\n", nome);
                    contadorFila2++;
                    contadorFila1--; 
                      
                    fimFila2 = getFimFila(auxfrenteFila2); // pega o fim da fila2 baseado na frente atual da fila 2
                    
                    f2[fimFila2] = f1[frenteFila1]; // Bota no fim da fila 2
                    
                    f1[frenteFila1] = -1; // tira da fila 1 colocando -1 no lugar
                    
                    auxfrenteFila2++;
                    if(auxfrenteFila2 >= MAXPROC) auxfrenteFila2 = 0;
                    
                    if(frenteFila2 >= MAXPROC) frenteFila2 = 0;
                }

                frenteFila1++;
                fimFila1 = getFimFila(frenteFila1);

                if(frenteFila1 >= MAXPROC || contadorFila1 == 1) frenteFila1 = 0;
                
                printf("\n-ALERT: Estado da fila1: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f1[i]);
                }
                printf("\n");
        	printf("-ALERT: Estado da fila2: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f2[i]);
                }
                printf("\n");
         	printf("-ALERT: Estado da fila3: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f3[i]);
                }
                printf("\n");
                printf("\n---------------------------------------------------------------------------------------------------------------------------------------\n");
        }
        
        existeFila1 = 0;
        
        if(contadorFila1 == 0)
    	{
    		printf("\n+----------------------------------------------+\n");
        	printf("|		    FILA 2		       |\n");
        	printf("+----------------------------------------------+\n");
    	}
        
        while(contadorFila2 != 0)
        { 
        	if(existeFila1 == 1) break;
        	
        	if(f2[frenteFila2] == -1)
                {
                	frenteFila2++;
                }
                
                strcpy(nome, getNomeByPid(f2[frenteFila2], p));
           	rajada		= getRajadaByPid(f2[frenteFila2], p);
                rajadaAtual 	= getRajadaAtualByPid(f2[frenteFila2], p);
                nivel 		= getNivelByPid(f2[frenteFila2], p);
                
                printf( "\n===> (!) %s vai para a frente da fila 2 com %d segundos de execucao restantes", nome, rajada );
                printf( "\n===> (!) Tem %d segundos para terminar\n", QUANTUM[1]);
                sleep(QUANTUM[1]);
                
                resultado = atualizaRajada(f2[frenteFila2], p, QUANTUM[1]);
                
                if(resultado == 1) // SUCESSO: Terminou rajada dentro do quantum da fila2 e terminou todas as rajadas
                {
                	printf( "===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+1 );
                	printf( "\t Programa completou todas suas tarefas!\n\t Finalizar programa!\n\n" );
                	//if(!kill(pid, SIGCHLD))
                	f2[frenteFila2] = -1;
                	contaProcessoAtivo--;
                	contadorFila2--;
                }
                else if(resultado == 0) // PROMOVIDO: Terminou rajada dentro do quantum da fila2 mas NÃO terminou todas as rajadas, sobe de nível
                {
                    sobeNivel(f2[frenteFila2], p);
                    
                    printf("===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+1 );
                    printf("\t ...Simula E/S...\n");
                    sleep(IO);
                    printf("\t %s foi para a rajada numero %d\n", nome, getRajadaAtualByPid(f2[frenteFila2], p)+1);
                    printf("\t Processo %s sai da fila 2 e vai para a 1.\n", nome);
                    
                    f1[frenteFila1] = f2[frenteFila2];
                    f2[frenteFila2] = -1;
                    contadorFila2--;
                    contadorFila1++;
                    existeFila1 = 1;
                }
                else // REBAIXADO: Não terminou dentro do quantum da fila2, vai pra fila3
                {
                    desceNivel(f2[frenteFila2], p);
                    printf("===> (-) Esgotou o quantum do nivel 2! Desceu para o nivel %d\n", getNivelByPid(f2[frenteFila2], p) );
                    printf("\t Processo %s sai da fila 2 e vai para a 3.\n", nome);
                    contadorFila3++;
                    contadorFila2--; 
                      
                    fimFila3 = getFimFila(auxfrenteFila3); // pega o fim da fila2 baseado na frente atual da fila 2
                    
                    f3[fimFila3] = f2[frenteFila2]; // Bota no fim da fila 2
                    
                    f2[frenteFila2] = -1; // tira da fila 1 colocando -1 no lugar
                    
                    auxfrenteFila3++;
                    if(auxfrenteFila3 >= MAXPROC) auxfrenteFila3 = 0;
                    
                    if(frenteFila3 >= MAXPROC) frenteFila3 = 0;
                }
                
                frenteFila2++;
                fimFila2 = getFimFila(frenteFila2);
                
                if(frenteFila2 >= MAXPROC) frenteFila2 = 0;
                
                printf("\n-ALERT: Estado da fila1: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f1[i]);
                }
                printf("\n");
        	printf("-ALERT: Estado da fila2: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f2[i]);
                }
                printf("\n");
         	printf("-ALERT: Estado da fila3: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f3[i]);
                }
                printf("\n");
                printf("\n---------------------------------------------------------------------------------------------------------------------------------------\n");
        }
               
        existeFila2 = 0;
        
        if(contadorFila2 == 0 && contadorFila1 == 0)
    	{
    		printf("\n+----------------------------------------------+\n");
        	printf("|		    FILA 3		       |\n");
        	printf("+----------------------------------------------+\n");
    	}
        
        while(contadorFila3 != 0)
        {
                
                if(existeFila2 == 1 || existeFila1 == 1) break;
                
                if(f3[frenteFila3] == -1)
                {
                	frenteFila3++;
                }
                
                strcpy(nome, getNomeByPid(f3[frenteFila3], p));
           	rajada		= getRajadaByPid(f3[frenteFila3], p);
                rajadaAtual 	= getRajadaAtualByPid(f3[frenteFila3], p);
                nivel 		= getNivelByPid(f3[frenteFila3], p);
                
                printf( "\n===> (!) %s vai para a frente da fila 3 com %d segundos de execucao restantes", nome, rajada );
                printf( "\n===> (!) Tem %d segundos para terminar\n", QUANTUM[2]);
                sleep(QUANTUM[2]);
                
                resultado = atualizaRajada(f3[frenteFila3], p, QUANTUM[2]);
                
                if(resultado == 1) // SUCESSO: Terminou rajada dentro do quantum da fila2 e terminou todas as rajadas
                {
                	printf( "===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+1 );
                	printf( "\t Programa completou todas suas tarefas!\n\t Finalizar programa!\n\n" );
                	//if(!kill(pid, SIGCHLD))
                	f3[frenteFila3] = -1;
                	contaProcessoAtivo--;
                	contadorFila3--;
                }
                else if(resultado == 0) // PROMOVIDO: Terminou rajada dentro do quantum da fila3 mas NÃO terminou todas as rajadas, sobe de nível
                {
                    	sobeNivel(f3[frenteFila3], p);
                    
                   	 printf("===> (+) Completou a rajada numero %d dentro do quantum!\n", rajadaAtual+1 );
                   	 printf("\t ...Simula E/S...\n");
                   	 sleep(IO);
                   	 printf("\t %s foi para a rajada numero %d\n", nome, getRajadaAtualByPid(f3[frenteFila3], p)+1);
                   	 printf("\t Processo %s sai da fila 3 e vai para a 2.\n", nome);
                  	  
                    	f2[frenteFila2] = f3[frenteFila3];
                    	f3[frenteFila3] = -1;
                    	contadorFila3--;
                    	contadorFila2++;
                    	existeFila2 = 1;
                }
                else // FALHA: Não terminou dentro do quantum da fila1, VAI PRO FIM DA FILA
                {
                	printf("===> (-) Esgotou o quantum do nivel 3!\n");
                	printf("\t %s foi para o fim da fila\n", nome);
                }
                
                frenteFila3++;
                fimFila3 = getFimFila(frenteFila3);
                
                if(frenteFila3 >= MAXPROC) frenteFila3 = 0;
                
                printf("\n-ALERT: Estado da fila1: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f1[i]);
                }
                printf("\n");
        	printf("-ALERT: Estado da fila2: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f2[i]);
                }
                printf("\n");
         	printf("-ALERT: Estado da fila3: ");
                for(i = 0 ; i < MAXPROC ; i++)
                {
                	printf("(%d) ", f3[i]);
                }
                printf("\n");
                printf("\n---------------------------------------------------------------------------------------------------------------------------------------\n");
        }
                   
    }
    
    printf("\nTHE END\n\n");
    
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

    escalonador(p);

}
