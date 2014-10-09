//Impede multiplas inclusões deste arquivo
#ifndef ANALISADOR_H
#define ANALISADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

//Constantes Booleanas
#define TRUE 1
#define FALSE 0

//Representação da categoria
enum categoria{PR = 1, ID, CTI, CTR, CTL, CTS, SN};

//Representação de sinais duplos
enum tokens{GT = 1, GE, LT, LE, EQ, NE, NT, SUM, SUB, MUL, DIV, MOD, ATB, DEF, OPB, CLB, OPP, CLP, CMM, SMC, AND, OR};

//Palavras reservadas
enum palavra_reservada{/*AND = 1,*/ ARRAY, BEGIN, CASE, CONST, /*DIV,*/ DO, DOWNTO, ELSE, END, /*FILE,*/ FOR, FUNC, GOTO, IF, IN,
LABEL, /*MOD,*/ NIL, NOT, OF, /*OR,*/ PACKED, PROC, PROGR, RECORD, REPEAT, SET, THEN, TO, TYPE, UNTIL, VAR, WHILE, WITH, CHAR,
STRING, INTEGER, REAL, NEQ};



//Estrutura do token
typedef struct token{
    enum categoria cat;	
	enum palavra_reservada p_reservada;
    union{		
		int cod;
		int ivalor;
		float fvalor;
		char lexema[MAX];
    };       
}token;


typedef int boolean;



token analex(void);



#endif
