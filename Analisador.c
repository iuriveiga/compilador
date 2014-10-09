#include "analisador.h"

//Tabela de palavras
/*
struct tabela_palavra_reservada{
	char comando[20];
	char palavra;
} tabela[] = {
	"char", CHAR,
	"int", INT,
	"float", FLOAT,
	"void", VOID,
	"if", IF,
	"else", ELSE,
	"while", WHILE,
	"put", PUT,
	"get", GET,
	"main", MAIN,	
};
*/

char palavra_reservada[64][10] = {"and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for", 
"func", "goto", "if", "in", "label", "mod", "nil", "not", "of", "or", "packed", "proc", "progr", "record", "repeat", "set", "then", "to", "type",
"until", "var", "while", "with", "char", "string", "integer", "real", "+", "-", "*", "/", "=", ".", ",", ";", ":", "'", "neq", "<", "<=", 
">=", ">", "(", ")", "[", "]", "{", "}", "..", "@"} ;

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

	//resolve colis„o de trÍs strings no 46
	if ((aux==46) && (tamanho>1)){
		if (tamanho==2){
			aux=48;
		}
		else{ 
			aux=49;
		}
	}
	
	//resolve colis„o de strings no 13
	if ((aux==13) && (numero==118))
		aux = 15;
		
	//resolve colis„o de strings no 112
	if ((aux==112) && (numero==111))
		aux=111;
	
	//resolve colis„o de strings no 30
	if (aux==30 && tamanho==4)
		aux=32;

	//resolve colis„o de strings no 78
	if (aux==78 && tamanho==4)
		aux=75;
	
	//resolve colis„o de strings no 4
	if (aux==4 && tamanho==4)
		aux=5;
			
	//resolve colis„o de strings no 93
	if (aux==93 && tamanho==3)
		aux=1;
	
	//resolve colis„o de strings no 44
	if (aux==44 && tamanho==2)
		aux=41;

	//resolve colis„o de strings no 14
	if (aux==14 && tamanho==6)
		aux=12;
	
	//resolve colis„o de strings no 95
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


int lin = 1;
extern FILE *fp;

token analex(void)
{
	
	//ver qual o melhor local para incializar a tablea de palavra reservada
	int j;
	for (j=0; j<64; j++) {
	   	strcpy(hash[stringParaInt(palavra_reservada[j])], palavra_reservada[j]); 
	   	//printf("O indice chave %d foi definido para a string %s\n", stringParaInt(palavra_reservada[i]), palavra_reservada[i]);
    }
	
  	boolean flag_estado_final = FALSE;
	int estado = 0;
	int i;
	char c, *s, *temp;
	token tk;
	
	
	s = (char *) malloc(MAX * sizeof(char));
	
	if(!s){
		printf("Falha na alocacao");
	}
	
	temp = s;
	
	while(TRUE){
		if(!flag_estado_final){
			c = fgetc(fp);
		}
		
		switch(estado){
			case 0:
			
				if(isspace(c)){
                    if(c == '\r') 
                        lin++;                        
                                                									
					continue;
                }
                	
				else if(c == '/')
					estado = 1;				
				else if( c == '<')
					estado = 6;
				else if( c == '>')
					estado = 10;
				else if( c == '='){
					estado = 13;
					flag_estado_final = TRUE;
				}
				else if( c == ','){
					estado = 14;
					flag_estado_final = TRUE;
				}
				else if( c == ':')
					estado = 15;
				else if(c == '('){
					estado = 18;
					flag_estado_final = TRUE;
				}	
				else if( c == ')'){
					estado = 19;
					flag_estado_final = TRUE;
				}	
				else if( c == '+'){
					estado = 20;
					flag_estado_final = TRUE;					
				}	
				else if( c == '-'){
					estado = 21;
					flag_estado_final = TRUE;
				}	
				else if( c == '*'){
					estado = 22;
					flag_estado_final = TRUE;
				}	
				else if( c == '%'){
					estado = 23;
					flag_estado_final = TRUE;
				}	
				else if( c == ';'){
					estado = 24;
					flag_estado_final = TRUE;
				}	
				else if( c == '.'){
					estado = 25;
					//o 25 n„o pode ser final, para considerar o token "."
					//temos que fazer igual ao que reconhece sÛ o "/"
					//mudar o autÙmato
					//flag_estado_final = TRUE;
				}
				else if( c == '"'){
					estado = 27;
					flag_estado_final = TRUE;					
				}
				else if( c == '\''){
					estado = 28;
					flag_estado_final = TRUE;					
				}
				else if(isdigit(c)){
					estado = 30;
					*temp = c;
					temp++;
					flag_estado_final = TRUE;					
				}
				else if(isalpha(c)){
					estado = 34;
					*temp = c;
					temp++;
					flag_estado_final = TRUE;					
				}
				break;
				
			case 1:
				if(c == '*')
					estado = 3;
				else{
					estado = 2;
					flag_estado_final = TRUE;
				}				
				break;

			//Retorna '/' como token valido	
			case 2:			
				tk.cat = SN;				
				tk.cod = DIV;
				ungetc(c, fp);
				free(s);
				return tk;
				break;
			
			case 3:
				//Checa se o proximo caractere eh um asterisco
				if(c == '*')
					estado = 4;					
				break;
				
			case 4:
				//Se o proximo caractere for '/' va° para o estado 5
				if(c == '/'){
					estado = 5;
					flag_estado_final = TRUE;
				}
				//Se o caractere for outra coisa retorne para o estado 3 e espera um '*'
				else
					estado = 3;				
				break;

			case 5:
				estado = 0;
				flag_estado_final = FALSE;
				break;
			
			//Checa se o proximo caractere for um '=' va para o estado 7 senao va para o esatdo 8
			case 6:
				if((c != '>') || (c != '=')){
					estado = 7;
					flag_estado_final = TRUE;
				}
				else{
					if(c == '>'){
						estado = 8;
						flag_estado_final = TRUE;
					} else {
						if(c == '='){
						estado = 9;
						flag_estado_final = TRUE;
						}
					}	 
				}
				
				break;

			//Monta um token valido para '<'  	
			case 7:
				tk.cat = SN;
				tk.cod = LT;
				ungetc(c, fp);
				free(s);
				return tk;
				break;

			//Monta o token "<>" e retorna uma representacao desse token  	
			case 8:
				tk.cat = SN;
				tk.cod = NE;				
				return tk;				
				break;
			
			//Caso o proximo token for '=' vai para o estado 9
			//Monta o token "<=" e retorna uma representacao desse token  	
			case 9:
				tk.cat = SN;
				tk.cod = LE;
				free(s);
				return tk;				
				break;
			
			//Se proximo token for '=' va° para estado 11
			//senao va para o estado 12
			case 10:
				if(c == '='){
					estado = 11;
					flag_estado_final = TRUE;
				}
				else{
					estado = 12;
					flag_estado_final = TRUE;
				}				
				break;
			
			//Monta o token valido para ">="
			case 11:
				tk.cat = SN;
				tk.cod = GE;
				free(s);
				return tk;
				break;

			//Monta o teken valido para '>'
			case 12:
				tk.cat = SN;
				tk.cod = GT;
				ungetc(c, fp);
				free(s);
				return tk;
				break;
			
			//Monta o token valido para "="	
			case 13:
				tk.cat = SN;
				tk.cod = EQ;
				break;

			//Monta o token valido para ','
			case 14:
				tk.cat = SN;
				tk.cod = CMM;
				free(s);
				return tk;
				break;

			//Se proximo token for '=' va° para estado 17
			//senao va para o estado 16
			case 15:
				
				if(c == '='){
					estado = 17;
					flag_estado_final = TRUE;
				}
				else{
					estado = 16;
					flag_estado_final = TRUE;
				}				
				break;
				
			//Monta o token valido para ':'	
			case 16:
				tk.cat = SN;
				tk.cod = DEF;
				ungetc(c, fp);
				free(s);
				return tk;
				break;

			//Monta o token valido para ':='	
			case 17:
				tk.cat = SN;
				tk.cod = ATB;
				ungetc(c, fp);
				free(s);
				return tk;
				break;
				
			//Monta um token valido para '('	
			case 18:
				tk.cat = SN;				
				tk.cod = OPP;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para ')'	
			case 19:
				tk.cat = SN;				
				tk.cod = CLP;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para '+'	
			case 20:
				tk.cat = SN;
				tk.cod = SUM;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para '-'	
			case 21:
				tk.cat = SN;				
				tk.cod = SUB;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para '*'	
			case 22:
				tk.cat = SN;
				tk.cod = MUL;
				free(s);
				return tk;
				break;
			
			//Monta um token valido para '%'	
			case 23:
				tk.cat = SN;
				tk.cod = MOD;
				free(s);
				return tk;
				break;
			
			//Monta um token valido para ';'
			case 24:
				tk.cat = SN;
				tk.cod = SMC;
				free(s);
				return tk;
				break;
			
			case 25:
				if(c == '.'){
					estado = 26;
					flag_estado_final = TRUE;
				}
				else{
					estado = 36;
					flag_estado_final = TRUE;
				}				
				break;

			//Monta um token valido para '.'	
			case 36:
				tk.cat = SN;
				tk.cod = DOT;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para '..'	
			case 26:
				tk.cat = SN;
				tk.cod = ARR;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para ' " '	
			case 27:
				tk.cat = SN;
				tk.cod = QTS;
				free(s);
				return tk;
				break;
			
			case 28:
				if(c == '\'' || c == '"'){
					estado = 29;
					flag_estado_final = TRUE;
				}
				else{
					estado = 28;
					flag_estado_final = TRUE;
				}				
				break;

			case 29:
				if(c == '\''){
					estado = 28;
					flag_estado_final = TRUE;
				}
				else{
					estado = 29;
					flag_estado_final = TRUE;
				}				
				break;

			//Monta um token valido para '''	
			
			case 28:
				tk.cat = SN;
				tk.cod = OAP;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para ' '' '	
			case 29:
				tk.cat = SN;
				tk.cod = CAP;
				free(s);
				return tk;
				break;

			// CONTINUAR DAQUI. TA FEITO ATE Q29
			// FAZER A PARTIR DO Q30

			case 30:
				if(isdigit(c)){
					*temp = c;
					temp++;
				}
					
				else if(c == '.'){
					estado = 32;
					*temp = c;
					temp++;
					flag_estado_final = TRUE;
				}
				else{
					estado = 31;
					flag_estado_final = TRUE;
				}				
				break;
				
			//Valida a constate de inteiros
			case 31:
				tk.cat = CTI;
				tk.ivalor = atoi(s);
				ungetc(c, fp);
				free(s);
				return tk;				
				break;
				
			//Se o proximo valor for digito continue no estado 32
			//Sen√£o v√° para o estado 33
			case 32:
				if(isdigit(c)){
					*temp = c;
					temp++;
				}
				else{
					estado = 33;
					flag_estado_final = TRUE;
				}
				break;
				
			//Valida uma constante real
			case 33:
				tk.cat = CTR;
				tk.fvalor = atof(s);
				ungetc(c, fp);
				free(s);
				return tk;
				break;								

			//Se o proximo token for letra ou digito continua no estado 34
			//Se for qualquer coisa va para o estado 35	
			case 34:
				if((isalpha(c)) || (isdigit(c))){
					*temp = c;
					temp++;
				}
				else{
					estado = 35;
					flag_estado_final = TRUE;
				}
				break;
				
			//Checa monta um token de indetificador ou palavra reservada
			case 35:				
				*temp = '\0';
				ungetc(c, fp);
				
				//Verifica se o comando em uma palavra reservada
				if (testePalavraReservada(s)) {
					tk.cat = PR;
					strcpy(tk.p_reservada, strupr(hash[stringParaInt(strlwr(s))])); 
					free(s);
					return tk;					
				}
				
				//Monta um tokem para identificador
				tk.cat = ID;
				strcpy(tk.lexema, s);
				free(s);
				return tk;				
				break;				
				
				
			//FIM DO NOSSO AUT‘MATO
			//EXCLUI A PARTE DO C”DIGO ANTIGO
			//TEM DOIS ESTADOS DO C”DIGO DELES QUE N√O EST√O NO
			//AUT‘MATO DELES, O 41 E 42, AND E OR
			
			/*
			//Monta um tokem valido para AND
			case 41:
				tk.cat = SN;
				tk.cod = AND;
				free(s);
				return tk;
				break;
				
			//Monta um tokem valido para OR
			case 42:
				tk.cat = SN;
				tk.cod = OR;
				free(s);
				return tk;
				break;
			*/

		}
	}
}

			
			/* COMENTARIOS IMPORTANTES
			

			UTILIZADO PARA NOT
			Retorna 'not' como token valido	
			
			case 8:
				tk.cat = SN;				
				tk.cod = NT;
				ungetc(c, fp);
				free(s);
				return tk;
				break;

				//Sen√£o v√° para o estado 11
			case 9:
				if(c == '='){
					estado = 10;
					flag_estado_final = TRUE;					
				}				
				else{
				   estado = 11;	
				   flag_estado_final = TRUE;
				}   
				break;
			*/


