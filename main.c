#include "analisador.h"



FILE *fp;


int main(int argc, char *argv[])
{		
	
	if(argc != 2){
		printf("Voce não informou o arquivo.\n");
		exit(1);	
   }
	
	if((fp = fopen(argv[1], "rb")) == NULL){
		printf("Arquivo nao pode ser aberto.\n");
		exit(1);
	}	
	
	//programa();
	
	return 0;
}
