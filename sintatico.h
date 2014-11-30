#ifndef SINTATICO_H
#define SINTATICO_H

#define TAM 50

enum situacao{PEN = 1, LIV};



typedef struct tab{
	enum situacao sit;
	int tipo;
	char id[TAM];	
}auxiliar;

void programa(void);
void bloco(void);
void declaracao_variaveis(void);
void declaracao_funcao(void);
void declaracao_funcao_principal(void);
void parametros_formais(void);
void bloco_de_funcao(void);
void comando(void);
void literal(void); //declarar
int expressao_simples(void);
void atribuicao(void);
void chamada_de_funcao1(void);
int chamada_de_funcao2(void);
void comando_condicional(void);
void comando_repetitivo(void);
void lista_de_expressoes(void);
int expressao(void);
void relacao(void);
int termo(void);
int fator(void);
int variavel(void);
void erro(int, int);




#endif
