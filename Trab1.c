#include<stdio.h>
#include<unistd.h>

int main (int argc, char* argv[])
{
	if(fork()!=0)
		execv(argv[1], argv);
	
	else
	{	
		if(fork()!=0)
			execv(argv[2], argv);
		
		else
			execv(argv[3], argv);
	}
	
}
