#include "Analisador.h"

#include <stdio.h>


//Tabela de palavras

struct tabela_palavra_reservada{
	char comando[20];
	char palavra;
} tabela[] = {
	"and", AND,
	"array", ARRAY,
	"begin", BEGIN,
	"case", CASE,
	"const", CONST,
	"div", DIV,
	"do", DO,
	"downto", DOWNTO,
	"else", ELSE,
	"end", END, 
	//tem problema com o file, ele não aceita por causa da linguagem C
	//"file", FILE,
	"for", FOR,
	"func", FUNC,
	"goto", GOTO,
	"if", IF,
	"in", IN,
	"label", LABEL,
	"mod", MOD,
	"nil", NIL,
	"not", NOT,
	"of", OF,
	"or", OR,
	"packed", PACKED,
	"proc", PROC,
	"progr", PROGR,
	"record", RECORD,
	"repeat", REPEAT,
	"set", SET,
	"then", THEN,
	"to", TO,
	"type", TYPE,
	"until", UNTIL,
	"var", VAR,
	"while", WHILE,
	"with", WITH,
	"char", CHAR,
	"string", STRING, 
	"integer", INTEGER,
	"real", REAL,
	"neq", NEQ
};

//pode tirar os simbolos especiais da lista de palavras reservadas já que estamos tratando eles diretamente no q0
char palavra_reservada_hash[64][10] = {"and", "array", "begin", "case", "const", "div", "do", "downto", "else", "end", "file", "for", 
"func", "goto", "if", "in", "label", "mod", "nil", "not", "of", "or", "packed", "proc", "progr", "record", "repeat", "set", "then", "to", "type",
"until", "var", "while", "with", "char", "string", "integer", "real", "neq","@"};

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

int lin = 1;
extern FILE *fp;

token analex(void)
{
	
	//ver qual o melhor local para incializar a tablea de palavra reservada
	int j;
	for (j=0; j<64; j++) {
	   	strcpy(hash[stringParaInt(palavra_reservada_hash[j])], palavra_reservada_hash[j]); 
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
		//printf("Caractere: %c\n", c);
		//printf("Estado: %d\n", estado);
		//system("pause");
		switch(estado){
			case 0:
			
				//system("pause");
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
					//system("pause");
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
				}
				else if( c == '"'){
					estado = 27;
					flag_estado_final = TRUE;					
				}
				else if( c == '\''){
					estado = 29;					
				}
				else if(isdigit(c)){
					estado = 31;
					*temp = c;
					temp++;
				}
				else if(isalpha(c)){
					estado = 35;
					*temp = c;
					temp++;
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
				//Se o proximo caractere for '/' va¡ para o estado 5
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
			
			//Se proximo token for '=' va¡ para estado 11
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
				//system("pause");
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

			//Se proximo token for '=' va¡ para estado 17
			//senao va para o estado 16
			case 15:
				//printf("cheguei aqui");
				//system("pause");
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
			//tá ficando em loop no = e não tá formando o p´roximo token
			case 17:
				printf("chegou no lugar que forma o token do :=\n");
				//system("pause");
				tk.cat = SN;
				tk.cod = ATB;
				//ungetc(c, fp);				
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
					estado = 27;
					flag_estado_final = TRUE;
				}				
				break;

			//Monta um token valido para '.'	
			case 27:
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
			case 28:
				tk.cat = SN;
				tk.cod = QTS;
				free(s);
				return tk;
				break;

			case 29:
				//Checa se o proximo caractere eh um asterisco
				if(c == '\'')
					estado = 30;
					flag_estado_final = TRUE;
				break;
				
			//Monta um token valido para fim de comentario	
			case 30:
				tk.cat = CTL;
				strcpy(tk.lexema, s);
				free(s);
				return tk;
				break;

			//Se o proximo token for digito continua no estado 31
			//Se o proximo token for ',' vai para o estado 33
			//senão vai para o estado 32
			case 31:
				if(isdigit(c)){
					*temp = c;
					temp++;
				}
				else if(c == ','){
					estado = 33;
					*temp = c;
					temp++;
				}
				else{
					estado = 32;
					flag_estado_final = TRUE;
				}				
				break;
			
			//Valida a constate de inteiros
			case 32:
				tk.cat = CTI;
				tk.ivalor = atoi(s);
				ungetc(c, fp);
				free(s);
				return tk;				
				break;
			//Se o proximo valor for digito continue no estado 37
			//Senão vá para o estado 38
			case 33:
				if(isdigit(c)){
					*temp = c;
					temp++;
				}
				else{
					estado = 34;
					flag_estado_final = TRUE;
				}
				break;
			
			//Valida uma constante real
			case 34:
				tk.cat = CTR;
				tk.fvalor = atof(s);
				ungetc(c, fp);
				free(s);
				return tk;
				break;						

			//Se o proximo token for letra ou digito continua no estado 34
			//Se for qualquer coisa va para o estado 35	
			case 35:
				if((isalpha(c)) || (isdigit(c))){
					*temp = c;
					temp++;
				}
				else{
					estado = 36;
					flag_estado_final = TRUE;
				}
				break;
				
			//Checa monta um token de indetificador ou palavra reservada
			case 36:				
				*temp = '\0';
				ungetc(c, fp);
				
				//printf("%s", c);
				//Verifica se o comando eh uma palavra reservada
				
				if (testePalavraReservada(s)) {
					//aqui tem como melhorar pra não ter que fazer o loop para o retorno
					//o problema é fazer cast pro tipo tk.p_reservada enum ou mudar de enum pra const char* palavra_reservada[] {"and", "or"}
					for(i = 0; *tabela[i].comando; i++){
						if(strcmp(tabela[i].comando, s) == 0){
							printf("montou o token de palavra reservada\n");
							tk.cat = PR;
							tk.p_reservada = tabela[i].palavra;						
							free(s);
							return tk;
						}						
					}
				}
				
				//Monta um tokem para identificador
				tk.cat = ID;
				strcpy(tk.lexema, s);
				free(s);
				return tk;				
				break;				
		}
	}
}


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
	
	token teste;
	
	while (!feof(fp)) {
		teste = analex();
		printf("Categoria: %d\n", (int)teste.cat);
		//como está se trabalhando com enum fica complicado converter ela pra string é mais fácil exibir o código dela que a posição dela na sequência de inicialização		
		printf("Palavra Reservada: %d\n", (int)teste.p_reservada);
		printf("Código: %d\n", (int)teste.cod);
		printf("Valor Inteiro: %d\n", teste.ivalor);
		printf("Lexema: %s\n\n\n", teste.lexema);
		
				
	} while (!feof(fp));
	
	return 0;
}			



