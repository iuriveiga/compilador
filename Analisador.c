#include "analisador.h"

//Tabela de palavras
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


int lin = 1;
extern FILE *fp;

token analex(void)
{
  	boolean flag_estado_final = FALSE;
	int estado = 0;
	int i;
	char c, *s, *temp;
	token tk;
	
	
	s = (char *) malloc(MAX * sizeof(char));
	
	if(!s){
		printf("Falha na alocação");
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
				else if( c == '!')
					estado = 6;
				else if( c == '<')
					estado = 9;
				else if( c == '>')
					estado = 12;
				else if( c == '=')
					estado = 15;
				else if(c == '{'){
					estado = 18;
					flag_estado_final = TRUE;
				}	
				else if( c == '}'){
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
				else if( c == '('){
					estado = 25;
					flag_estado_final = TRUE;
				}	
				else if( c == ')'){
					estado = 26;
					flag_estado_final = TRUE;
				}	
				else if( c == '\''){
					estado = 27;
					flag_estado_final = TRUE;					
				}	
				else if( c == '"'){
					estado = 31;
				}
				else if( c == ','){
					estado = 34;
					flag_estado_final = TRUE;
				}	
				else if(isdigit(c)){
					estado = 35;
					*temp = c;
					temp++;
				}	
				else if(isalpha(c)){
					estado = 39;
					*temp = c;
					temp++;
				}

				else if(c == '&'){
					estado = 41;
					flag_estado_final = TRUE;
				}
				
				else if(c == '|'){
					estado = 42;
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
				//Checa se o proximo caractere é asteristico
				if(c == '*')
					estado = 4;					
				break;
				
			case 4:
				//Se o proximo caractere for '/' vá para o estado 5
				if(c == '/'){
					estado = 5;
					flag_estado_final = TRUE;
				}
				//Se o caractere for outra coisa retorne para o estado 3 e espera um '*'
				else
					estado = 3;				
				break;
			//Fim de comentario	
			case 5:
				estado = 0;
				flag_estado_final = FALSE;
				break;
			
			//Checa se o proximo caractere for um '=' va para o estado 7 senão va para o esatdo 8
			case 6:
				if(c == '='){
					estado = 7;
					flag_estado_final = TRUE;
				}
				else{
					estado = 8;
					flag_estado_final = TRUE;
				}
				
				break;
			
			//Monta o token "!=" e retorna uma representação desse token  	
			case 7:
				tk.cat = SN;
				tk.cod = NE;				
				return tk;				
				break;
			
			//Retorna '!' como token valido	
			case 8:
				tk.cat = SN;				
				tk.cod = NT;
				ungetc(c, fp);
				free(s);
				return tk;
				break;
			
			//Caso o proximo token for '=' vá para o estado 10
			//Senão vá para o estado 11
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
			
			//Monta um token valindo para "<=" 	
			case 10:
				tk.cat = SN;
				tk.cod = LE;
				free(s);
				return tk;				
				break;
			
			//Monta um token valido para '<'	
			case 11:
				tk.cat = SN;
				tk.cod = LT;
				ungetc(c, fp);
				free(s);
				return tk;
				break;
			
			//Se proximo token for '=' vá para estado 13
			//senão va para o estado 14
			case 12:
				
				if(c == '='){
					estado = 13;
					flag_estado_final = TRUE;
				}
				else{
					estado = 14;
					flag_estado_final = TRUE;
				}				
				break;
				
			//Monta o token valido para ">="
			case 13:
				tk.cat = SN;
				tk.cod = GE;
				free(s);
				return tk;
				break;
			
			//Monta o teken valido para '>'
			case 14:
				tk.cat = SN;
				tk.cod = GT;
				ungetc(c, fp);
				free(s);
				return tk;
				break;
				
			//Se o proximo token for '=' vá para o estado 16
			//Senão vá para o estado 17
			case 15:
				if(c == '='){
					estado = 16;
					flag_estado_final = TRUE;
				}
				else{
					estado = 17;
					flag_estado_final = TRUE;
				}
				break;
				
			//Monta o token valido para "=="	
			case 16:
				tk.cat = SN;
				tk.cod = EQ;
				break;
				
			//Monta o token valido para '='	
			case 17:
				tk.cat = SN;
				tk.cod = ATB;
				ungetc(c, fp);
				free(s);
				return tk;
				break;
				
			//Monta um token valido para '{'	
			case 18:
				tk.cat = SN;				
				tk.cod = OPB;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para '}'	
			case 19:
				tk.cat = SN;				
				tk.cod = CLB;
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
				
			//Monta um token valido para '('	
			case 25:
				tk.cat = SN;
				tk.cod = OPP;
				free(s);
				return tk;
				break;
				
			//Monta um token valido para ')'	
			case 26:
				tk.cat = SN;
				tk.cod = CLP;
				free(s);
				return tk;
				break;
				
			
			//Se caractere for digito ou letra estado = 28
			//Se caractere for '\'' estado igual a 29;
			
			case 27:
				if((isalpha(c)) || (isdigit(c))){				
					estado = 28;
					*temp = c;					
				}
				if (c == '\''){
					estado = 29;
					*temp = '\0';
					flag_estado_final = TRUE;
				}									
				break;
				
			//Se o proximo token for '\'' vá para estado 29
			case 28:
				if(c == '\''){					
					estado = 29;
					flag_estado_final = TRUE;
				}
				else{
					estado = 30;
					flag_estado_final = TRUE;
				}	
				break;				
			
			//Monta uma constante de literais
			case 29:
				tk.cat = CTL;
				strcpy(tk.lexema, s);
				free(s);
				return tk;
				break;
			
			//Chama a mensagem de erro	
			case 30:
				printf("\' Esperado na linha %d", lin);
				break;
				
			//Se o proximo token for qualquer coisa continue no estado 31
			//Se o proximo token for '\"' vá para o estado 32
			//Se o proximo token for um retorno de carro va para o estado 33;
			case 31:
				if(c == '\r'){
					estado = 33;
					flag_estado_final = TRUE;
				}
				else if(c == '\"'){					
					estado = 32;
					flag_estado_final = TRUE;
				}
				else{
					*temp = c;
					temp++;
				}				
				break;
				
			//Monta a constante de string	
			case 32:
				tk.cat = CTS;
				strcpy(tk.lexema, s);
				free(s);
				return tk;
				break;
			
			//Chama a mensagem de erro	
			case 33:
				printf("Quebra de linha não esperada na linha %d", lin);
				break;
			
			//Monta o token valido para ','
			case 34:
				tk.cat = SN;
				tk.cod = CMM;
				free(s);
				return tk;
				break;
				
			//Se o proximo token for digito continua no estado 35
			//Se o proximo token for '.' vá para o estado 37
			//senão vá para o estado 36
			case 35:
				if(isdigit(c)){
					*temp = c;
					temp++;
				}
					
				else if(c == '.'){
					estado = 37;
					*temp = c;
					temp++;
				}
				else{
					estado = 36;
					flag_estado_final = TRUE;
				}				
				break;
			
			//Valida a constate de inteiros
			case 36:
				tk.cat = CTI;
				tk.ivalor = atoi(s);
				ungetc(c, fp);
				free(s);
				return tk;				
				break;
			//Se o proximo valor for digito continue no estado 37
			//Senão vá para o estado 38
			case 37:
				if(isdigit(c)){
					*temp = c;
					temp++;
				}
				else{
					estado = 38;
					flag_estado_final = TRUE;
				}
				break;
			
			//Valida uma constante real
			case 38:
				tk.cat = CTR;
				tk.fvalor = atof(s);
				ungetc(c, fp);
				free(s);
				return tk;
				break;
			
			//Se o proximo token for letra ou digito continua no estado 39
			//Se for qualquer coisa va para o estado 40	
			case 39:
				if((isalpha(c)) || (isdigit(c))){
					*temp = c;
					temp++;
				}
				else{
					estado = 40;
					flag_estado_final = TRUE;
				}
				break;
			
			//Checa monta um token de indetificador ou palavra reservada
			case 40:				
				*temp = '\0';
				ungetc(c, fp);
				
				//Verifica se o comando em uma palavra reservada
				for(i = 0; *tabela[i].comando; i++){
					if(strcmp(tabela[i].comando, s) == 0){
						tk.cat = PR;
						tk.p_reservada = tabela[i].palavra;						
						free(s);
						return tk;
					}						
				}
				
				//Monta um tokem para identificador
				tk.cat = ID;
				strcpy(tk.lexema, s);
				free(s);
				return tk;				
				break;
			
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
		}
	}
}