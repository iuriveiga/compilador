#ifndef SEMANTICO_H
#define SEMANTICO_H


#define TAM 50
#define MAXP 1000

enum classes{VARIAVEL = 1, FUNCAO, PARAM}; 
enum escopos{GL = 1, LO}; 
enum zomb{F, T};

  
typedef struct tabela_simbolos{ 
  char lexema[TAM]; 
  int classe; 
  int tipo; 
  int escopo; 
  int zombie;       
}tabela_simbolos; 

typedef struct verfica_parametros{
   char id[TAM];
   int sit;        
}ver_par;


void insere_simbolo(tabela_simbolos);
void remove_simbolo(void);
void zombie_simbolo(void);
void imprime_tabela(void);
int verifica_tabela(char id[]);
#endif
