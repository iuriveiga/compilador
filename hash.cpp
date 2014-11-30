#include <iostream>
#include <string.h>
#include <stdio.h>

char palavra_reservada[64][10] = {"and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for", 
"func", "goto", "if", "in", "label", "mod", "nil", "not", "of", "or", "packed", "proc", "progr", "record", "repeat", "set", "then", "to", "type",
"until", "var", "while", "with", "char", "string", "integer", "real", "+", "-", "*", "/", "=", ".", ",", ";", ":", "'", "neq", "<", "<=", 
">=", ">", "(", ")", "[", "]", "{", "}", "..", "@", "/*", "*/"  } ;

char hash[113][10];

int revert(int n){
    int ret=0,i=1;
    while(i<=n){
        ret*=10;
        ret+=(n%(i*10)-n%i)/i;
        i*=10;
    }
    return ret;
}

int stringParaInt(char *string) {
    int tamanho, numero, aux, i;
    tamanho = strlen(string);  
    aux = 0;
	for (i=0; i<tamanho; i++){
    	numero = string[i];
		int comp = ((i+1) % 2);
	 	if (comp==0){
    		aux = aux + numero;
    	} 
		else {
	   		aux = aux + revert(numero);
    	}
    }
	aux = aux % 113;

	//resolve colisão de três strings no 46
	if ((aux==46) && (tamanho>1)){
		if (tamanho==2){
			aux=48;
		}
		else{ 
			aux=49;
		}
	}
	
	//resolve colisão de strings no 13
	if ((aux==13) && (numero==118))
		aux = 15;
		
	//resolve colisão de strings no 112
	if ((aux==112) && (numero==111))
		aux=111;
	
	//resolve colisão de strings no 30
	if (aux==30 && tamanho==4)
		aux=32;

	//resolve colisão de strings no 78
	if (aux==78 && tamanho==4)
		aux=75;
	
	//resolve colisão de strings no 4
	if (aux==4 && tamanho==4)
		aux=5;
			
	//resolve colisão de strings no 93
	if (aux==93 && tamanho==3)
		aux=1;
	
	//resolve colisão de strings no 44
	if (aux==44 && tamanho==2)
		aux=41;

	//resolve colisão de strings no 14
	if (aux==14 && tamanho==6)
		aux=12;
	
	//resolve colisão de strings no 95
	if (aux==95 && numero==59)
		aux=96;
																
	return aux;
}

int testePalavraReservada (char *string){
	if (strcmp(hash[stringParaInt(strlwr(string))], strlwr(string))==0)
		return 1;
	else
		return 0;
}

int main(int argc, char** argv) {
    int i;
    char palavra[50];
    for (i=0; i<64; i++) {
	   	strcpy(hash[stringParaInt(palavra_reservada[i])], palavra_reservada[i]); 
	   	//printf("O indice chave %d foi definido para a string %s\n", stringParaInt(palavra_reservada[i]), palavra_reservada[i]);
    }
    printf("Entre com a string: ");
    gets(palavra);
    printf("O resultado foi: %d", testePalavraReservada(palavra));
	return 0;
}
