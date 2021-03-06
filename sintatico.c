#include "analisador.h"
#include "sintatico.h"
#include "semantico.h"

//Declara��o de variaveis
token tk;
auxiliar aux;
extern int lin;
ver_par func;


extern tabela_simbolos t_simb[MAXP];
extern enum escopos escopo;
tabela_simbolos novo;


void programa(void)
{     
	bloco();	
}

void bloco(void)
{
	aux.sit = LIV;
	tk = analex();
	escopo = GL;
    func.sit = LIV;
   
	declaracao_variaveis(); // Como tratar se n�o houver vari�vel?...	
	declaracao_funcao(); // ...Chamamos fun��o mesmo assim e tratamos l� dentro quando chamar o analex?	
	declaracao_funcao_principal();
}

void declaracao_variaveis(void)
{
	//Verifica se o proximo token � um identificador;
	if(tk.cat == PR){
		strcpy(aux.id, tk.lexema);
	    
		tk = analex();			
	   
	   if((tk.cat == SN) && (tk.cod == OPP)){
           aux.sit = PEN;           
       }else{
              novo.escopo = escopo;
              novo.classe = VARIAVEL;
              novo.tipo = CHAR;
              strcpy(novo.lexema, aux.id);            
              insere_simbolo(novo);
   			//Verifica se o proximo token � ','
   			tk = analex();
   			if((tk.cat == SN) && (tk.cod == CMM)){
              
   				while(tk.cod == CMM){
                             
   					tk = analex();
   					
   					//Verifica se o proximo token � um ID
   					if(tk.cat == ID){ 
                   
                                                 
                        strcpy(novo.lexema, tk.lexema);
                        insere_simbolo(novo);//
                                     
   						tk = analex();
   					} else erro(lin, 1); //Chama a mensagem de erro
   				}				
   			}			
   			//Verifica se o proximo token � um ':'
   			if((tk.cat == SN) && (tk.cod == DEF)){
                      
   				tk = analex();
   				declaracao_variaveis();//Chama a declara�a� de variaveis novamente
   			} else {
   				//aux.sit = PEN;
               erro(lin, 8);			
   			}
    }
	
	//Verifica se o proximo token � do tipo CHAR
	if((tk.cat == PR) && (tk.p_reservada == CHAR)){
		aux.tipo = CHAR;
	
		tk = analex();
		
				
		} else erro(lin, 1); //Chama a mesangem de erro
	}	
	
	//Verifica se o token � do tipo INT
	else if((tk.cat == PR) && (tk.p_reservada == INTEGER)){
		aux.tipo = INTEGER;
		tk = analex();
		
		//Verifica se o proximo token � um identificador;
		if(tk.cat == ID){
			strcpy(aux.id, tk.lexema);
			tk = analex();			
		
			
			if((tk.cat == SN) && (tk.cod == OPP)){
               aux.sit = PEN;           
            }else{
                novo.escopo = escopo;
                novo.classe = VARIAVEL;
                novo.tipo = INTEGER;
                strcpy(novo.lexema, aux.id);            
                insere_simbolo(novo); 
       			//Verifica se o proximo token � ','
       			if((tk.cat == SN) && (tk.cod == CMM)){
                         
       				while(tk.cod == CMM){
       					tk = analex();
       					
       					//Verifica se o proximo token � um ID
       					if(tk.cat == ID){
                            strcpy(novo.lexema, tk.lexema);
                            insere_simbolo(novo);//      
       						tk = analex();
       					} else erro(lin, 1); //Chama a mensagem de erro
       				}				
       			}			
       			//Verifica se o proximo token � um ';'
       			if((tk.cat == SN) && (tk.cod == SMC)){
       				tk = analex();
       				declaracao_variaveis();//Chama a declara�a� de variaveis novamente
       			} else {
       				//aux.sit = PEN;
                   erro(lin, 8);			
       			}
       }
				
		} else erro(lin, 1); //Chama a mesangem de erro
	}	
	
	//Verifica se o tokem � um tipo REAL	
	else if((tk.cat == PR) && (tk.p_reservada == REAL)){
		aux.tipo = REAL;
		tk = analex();
		
		//Verifica se o proximo token � um identificador;
		if(tk.cat == ID){
			strcpy(aux.id, tk.lexema);
			tk = analex();			
		
			
		if((tk.cat == SN) && (tk.cod == OPP)){
               aux.sit = PEN;           
         }else{
            novo.escopo = escopo;
            novo.classe = VARIAVEL;
            novo.tipo = REAL;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);
   			//Verifica se o proximo token � ','
   			if((tk.cat == SN) && (tk.cod == CMM)){
             
   				while(tk.cod == CMM){
   					tk = analex();
   					
   					//Verifica se o proximo token � um ID
   					if(tk.cat == ID){
                        strcpy(novo.lexema, tk.lexema);
                        insere_simbolo(novo);    
   						tk = analex();
   					} else erro(lin, 1); //Chama a mensagem de erro
   				}				
   			}			
   			//Verifica se o proximo token � um ';'
   			if((tk.cat == SN) && (tk.cod == SMC)){
   				tk = analex();
   				declaracao_variaveis();//Chama a declara�a� de variaveis novamente
   			} else {
   				//aux.sit = PEN;
               erro(lin, 8);			
   			}
       }
				
		} else erro(lin, 1); //Chama a mesangem de erro
	}	
	
}

void declaracao_funcao(void)
{
	//Verifica se existe alguma pendencia na variavel aux
	//Se existir pendencia siginfica que o tipo o identificador e o parentese j� foi chamado
	if(aux.sit == PEN){
	
		//Verifica se o tipo � CHAR
		if(aux.tipo == CHAR){
            
            novo.escopo = escopo;
            novo.classe = FUNCAO;
            novo.tipo = CHAR;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);                
			
            if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
			    escopo = LO;
				parametros_formais();
				//Verifica se o proximo token � um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					//Verifica se o proximo tokem � um '{'
						
				} else erro(lin, 3); /*Chama mensagem de erro
									 caso n�o venha ')' */
			} else erro(lin, 2);						 
		}		
		//Verifica se o tipo � INTEGER
		else if(aux.tipo == INTEGER){
             
            novo.escopo = escopo;
            novo.classe = FUNCAO;
            novo.tipo = INTEGER;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);        
             
			if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
			    escopo = LO;
				parametros_formais();
				//Verifica se o proximo token � um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();											
				} else erro(lin, 3); /*Chama mensagem de erro
									 caso n�o venha ')' */
			} else erro(lin, 2);						 
		}		
		//Verifica se o tipo � REAL
		else if(aux.tipo == REAL){
             
            novo.escopo = escopo;
            novo.classe = FUNCAO;
            novo.tipo = REAL;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);        
             
			if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
	            
                escopo = LO;		
				parametros_formais();
				//Verifica se o proximo token � um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
				} else erro(lin, 3); /*Chama mensagem de erro
									 caso n�o venha ')' */
			} else erro(lin, 2);						 
		}		
		
	} else { 
	
		//Verifica se o tipo � char
		if((tk.cat == PR) && (tk.p_reservada == CHAR)){
			tk = analex();
			//Verifica se o proximo token � um identificador
			if(tk.cat == ID){
                 
                novo.escopo = escopo;
                novo.classe = FUNCAO;
                novo.tipo = CHAR;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);              
                      
				tk = analex();
				//Verifica se o proximo token � um '('
				if((tk.cat == SN) && (tk.cod == OPP)){
					tk = analex();
					
					escopo = LO;
					parametros_formais();
					//Verifica se o proximo token � um ')'
					if((tk.cat == SN) && (tk.cod == CLP)){
						tk = analex();
					} else erro(lin, 3); //Chama o erro quando n�o vem um )

				} else erro(lin, 2);//Chama mensagem de erro caso n�o venha um (
				
			} else erro(lin, 1);//Chama a mensage de erro
		}

		//Verifica se o tipo � INTEGER
		if((tk.cat == PR) && (tk.p_reservada == INTEGER)){
			tk = analex();
			//Verifica se o proximo token � um identificador
			if(tk.cat == ID){
                
                novo.escopo = escopo;
                novo.classe = FUNCAO;
                novo.tipo = INTEGER;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);                    
                      
				tk = analex();
				//Verifica se o proximo token � um '('
				if((tk.cat == SN) && (tk.cod == OPP)){
					tk = analex();
					escopo = LO;
					parametros_formais();
					//Verifica se o proximo token � um ')'
					if((tk.cat == SN) && (tk.cod == CLP)){
						tk = analex();
					} else erro(lin, 3); //Chama o erro quando n�o vem um )

				} else erro(lin, 2);//Chama mensagem de erro caso n�o venha um (
				
			} else erro(lin, 1);//Chama a mensage de erro
		}

		//Verifica se o tipo � REAL
		if((tk.cat == PR) && (tk.p_reservada == REAL)){
			tk = analex();
			//Verifica se o proximo token � um identificador
			if(tk.cat == ID){
                 
                novo.escopo = escopo;
                novo.classe = FUNCAO;
                novo.tipo = REAL;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);                    
                      
                      
				tk = analex();
				//Verifica se o proximo token � um '('
				if((tk.cat == SN) && (tk.cod == OPP)){
					tk = analex();
					escopo = LO;
					parametros_formais();
					//Verifica se o proximo token � um ')'
					if((tk.cat == SN) && (tk.cod == CLP)){
						tk = analex();
					} else erro(lin, 3); //Chama o erro quando n�o vem um )

				} else erro(lin, 2);//Chama mensagem de erro caso n�o venha um (
				
			} else erro(lin, 1);//Chama a mensage de erro
		}
	}
}

void declaracao_funcao_principal(void)
{
     
	if ((tk.cat == PR) && (tk.p_reservada == BEGIN)){
		tk = analex();
		novo.escopo = escopo;
        novo.classe = FUNCAO;
        strcpy(novo.lexema, "begin");            
        insere_simbolo(novo); 
		escopo = LO; 
	} else erro(lin, 6); // Chama uma mensagem de erro caso n�o venha o begin	
}

void parametros_formais(void)
{
	//Apenas declara��o de parametros os parenteses s�o tratados fora
	
	//Verifica se o tipo � CHAR
	if((tk.cat == PR) && (tk.p_reservada == CHAR)){
		tk = analex();
		//verifica se o proximo token � um identificador
		if(tk.cat == ID){
            
            novo.escopo = escopo;
            novo.classe = PARAM;
            novo.tipo = CHAR;
            strcpy(novo.lexema, tk.lexema);            
            insere_simbolo(novo);            
                  
			tk = analex();
		} else erro(lin, 1); //Chama uma mensagem de erro caso n�o venha um ID
	}
	
	//Verifica se o Tipo � INTEGER
	else if((tk.cat == PR) && (tk.p_reservada == INTEGER)){
		tk = analex();
		//verifica se o proximo token � um identificador
		if(tk.cat == ID){
                  
            novo.escopo = escopo;
            novo.classe = PARAM;
            novo.tipo = INTEGER;
            strcpy(novo.lexema, tk.lexema);            
            insere_simbolo(novo);                 
                  
			tk = analex();
		} else erro(lin, 1); //Chama uma mensagem de erro caso n�o venha um ID
	}
	
	//Verifica se o tipo � REAL
	else if((tk.cat == PR) && (tk.p_reservada == REAL)){
		tk = analex();
		//verifica se o proximo token � um identificador
		if(tk.cat == ID){
            
            novo.escopo = escopo;
            novo.classe = PARAM;
            novo.tipo = REAL;
            strcpy(novo.lexema, tk.lexema);            
            insere_simbolo(novo);                  
                  
			tk = analex();
		} else erro(lin, 1); //Chama uma mensagem de erro caso n�o venha um ID
	}
	
	//Caso o proximo token for ','
	while((tk.cat == SN) && (tk.cod == CMM)){
		tk = analex();
		//Verifica se o tipo � CHAR
		if((tk.cat == PR) && (tk.p_reservada == CHAR)){
			tk = analex();
			//verifica se o proximo token � um identificador
			if(tk.cat == ID){
               novo.escopo = escopo;
               novo.classe = PARAM;
               novo.tipo = CHAR;
               strcpy(novo.lexema, tk.lexema);            
               insere_simbolo(novo);                  
			   tk = analex();
			} else erro(lin, 1); //Chama uma mensagem de erro caso n�o venha um ID
		}
	
		//Verifica se o Tipo � INTEGER
		else if((tk.cat == PR) && (tk.p_reservada == INTEGER)){
			tk = analex();
			//verifica se o proximo token � um identificador
			if(tk.cat == ID){
                novo.escopo = escopo;
                novo.classe = PARAM;
                novo.tipo = INTEGER;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);                  
				tk = analex();
			} else erro(lin, 1); //Chama uma mensagem de erro caso n�o venha um ID
		}
	
		//Verifica se o tipo � REAL
		else if((tk.cat == PR) && (tk.p_reservada == REAL)){
			tk = analex();
			//verifica se o proximo token � um identificador
			if(tk.cat == ID){
                 novo.escopo = escopo;
                 novo.classe = PARAM;
                 novo.tipo = REAL;
                 strcpy(novo.lexema, tk.lexema);            
                 insere_simbolo(novo);                 
	 			 tk = analex();
			} else erro(lin, 1); //Chama uma mensagem de erro caso n�o venha um ID
		
		} else erro(lin, 7); //Chama mensagem de erro caso n�o seja feita nenhuma declara��o
	
	}
}

void bloco_de_funcao(void)
{
	declaracao_variaveis();
	comando();
}

void atribuicao(void)
{
    int tipo1, tipo2;
    char id[50];
    tipo1 = tipo2 = 0;
	if(tk.cat == ID){
      strcpy(id, tk.lexema);       
		strcpy(aux.id, tk.lexema);
		tk = analex();
		//Verifica se o proximo token � um '='
		if((tk.cat == SN) && (tk.cod == ATB)){
         tipo1 = verifica_tabela(id);      
			tk = analex();			
			tipo2 = expressao();
			if(tipo1 != tipo2){
              erro(lin, 12);           
            }
         			
			//Verifica se o proximo token � um ';'
			if((tk.cat == SN) && (tk.cod == SMC)){
				tk = analex();
				comando();
			} else erro(lin, 8); //Chama mensagem de erro caso n�o venha um ';'
		} else{
			aux.sit = PEN;
		}
	}
}

void chamada_de_funcao1(void)
{
	if(aux.sit == PEN){
		if((tk.cat == SN) && (tk.cod == OPP)){
			tk = analex();
			lista_de_expressoes();
			//Verifica se o proximo token � um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
				//Verifica se o proximo token � um ';'
				if((tk.cat == SN) && (tk.cod == SMC)){
					tk = analex();
					aux.sit = LIV;
					comando();
				} else erro(lin, 8); // Chmama mensagem de erro caso n�o venha ';'
			} else erro(lin, 3); // Chama mensagem de erro caso n�o venha ')'
			
		} else 	erro(lin, 2);
		
	} else {	
		if(tk.cat == ID){
			tk = analex();
			//Verifica se o procximo token � um '('
			if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
				lista_de_expressoes();
				//Verifica se o proximo token � um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					//Verifica se o proximo token � um ';'
					if((tk.cat == SN) && (tk.cod == SMC)){
						tk = analex();
						comando();
					} else erro(lin, 8); // Chmama mensagem de erro caso n�o venha ';'
					
				} else erro(lin, 3); // Chama mensagem de erro caso n�o venha ')'
			}
		}
	}
}

int chamada_de_funcao2(void)
{
    int tipo; 
    char id[50];
  
	if(aux.sit == PEN){
		if((tk.cat == SN) && (tk.cod == OPP)){
            strcpy(id, aux.id);
            strcpy(func.id, aux.id);
 			tipo = verifica_tabela(id);
			   do{
                  tk = analex(); 
                  func.sit = PEN;   
      			   lista_de_expressoes();
      			   if(func.sit == LIV){
                     if(tk.cod == CMM){
                         erro(lin, 14);                 
                     }                     
                  }
            }while(tk.cod == CMM); 
            if(func.sit == PEN){
                erro(lin, 15);                    
            } 
			   
			//Verifica se o proximo token � um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
				aux.sit = LIV;
				return tipo;
			} else erro(lin, 3); // Chama mensagem de erro caso n�o venha ')'
		} else {
         if(tk.cod == CLP){
             aux.sit = LIV; 
             return 0;               
         }    
			aux.sit = LIV;
		}
	} else {	
		if(tk.cat == ID){
			tk = analex();
			//Verifica se o procximo token � um '('
			if((tk.cat == SN) && (tk.cod == OPP)){
                strcpy(id, tk.lexema);
                strcpy(func.id, tk.lexema);  
                tipo = verifica_tabela(id);
				do{
                  func.sit = PEN; 
                  
                  tk = analex();   
      			   lista_de_expressoes();
      			 
      			   if(func.sit == LIV){
                     if(tk.cod == CMM){
                              
                         erro(lin, 14);                 
                     }                     
                  }
			
            }while(tk.cod == CMM); 
            
            if(func.sit == PEN){
                erro(lin, 15);                    
            } 
				//Verifica se o proximo token � um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					aux.sit = LIV;
					return tipo;
				} else erro(lin, 3); // Chama mensagem de erro caso n�o venha ')'
			}
		}
	}
}

void comando_condicional(void)
{
	//Verifica se o proximo token � um IF
	if((tk.cat == PR) && (tk.p_reservada == IF)){
		tk = analex();
		//Verifica se o proximo token � um '('
		if((tk.cat == SN) && (tk.cod == OPP)){
			tk = analex();
			expressao();
			//verifica se o proximo token � um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
			} else erro(lin, 3); // chama erro caso n�o venha ')'
			
		} else erro(lin, 2); // chama o erro caso n�o venha um '('
	
	}
	
	//Verifica se o proximo token � um ELSE
	else if((tk.cat == PR) && (tk.p_reservada == ELSE)){
		tk = analex();
	}
}

void comando_repetitivo(void)
{
	//Verifica se o proximo token � um WHILE
	if((tk.cat == PR) && (tk.p_reservada == WHILE)){
		tk = analex();
		//Verifica se o proximo token � '('
		if((tk.cat == SN) && (tk.cod == OPP)){
			tk = analex();
			expressao();
			//Verifica se o proximo token � um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
			} else erro(lin, 3); //Chama erro caso n�o venha ')'
		} else erro(lin, 2);//Chama erro caso n�o venha '('
	}
}

void comando(void)
{
        
	atribuicao();	    

	chamada_de_funcao2(); 
	 
	comando_condicional();
	 
	comando_repetitivo();
}

void lista_de_expressoes(void)
{
	expressao();
}

int expressao(void)
{
   int tipo;
	tipo = expressao_simples();	
	relacao();
	return tipo;
}

void relacao(void)
{
	if(tk.cat == SN){
		if(tk.cod == GT){
			tk = analex();
			expressao();
		}
		
		else if(tk.cod == GE){
			tk = analex();
			expressao();
		} 
		
		else if(tk.cod == LT){
			tk = analex();
			expressao();
		} 
		
		else if(tk.cod == LE){
			tk = analex();
			expressao();
		} 
		
		else if(tk.cod == EQ){
			tk = analex();
			expressao();
		}

		else if(tk.cod == NE){
			tk = analex();
			expressao();
		}
	}
}

int expressao_simples(void)
{
	int tipo1, tipo2;
   tipo1 = tipo2 = 0;
    
   tipo1 = termo();

	while((tk.cod == SUM) || (tk.cod == SUB) || (tk.cod == OR)){
     tk = analex();
             
	  tipo2 = termo();
	  
      if(tipo1 != tipo2){
          
          erro(lin, 12);        
      }	
	}
   return tipo1;	
}

int termo(void)
{
   int tipo1, tipo2;
   tipo1 = tipo2 = 0;
	tipo1 = fator();

	while((tk.cod == MUL) || (tk.cod == DIV) || (tk.cod == MOD) || (tk.cod == AND)){
      tk = analex();          
		tipo2 = fator();
		
		if(tipo1 != tipo2){
            erro(lin, 12);      
        }
	}
	
	return tipo1;
}

int variavel(void)
{
    int tipo;
    char id[50];
     
	if(tk.cat == ID){
		strcpy(aux.id, tk.lexema);
		strcpy(id, tk.lexema);
		tk = analex();
		 tipo = verifica_tabela(id);
		//Verifica se o proximo token � um ';'
		if((tk.cat == SN) && (tk.cod == SMC)){

     	}
     aux.sit = PEN; 		
   }  
   return tipo;	 	
}

int fator(void)
{
  int tipo1 = 0;
  int tipo2 = 0;
  	
  if(tk.cat == CTI){
    tk = analex();
    return INTEGER;
  }
  	   
  else if(tk.cat == CTR){
    tk = analex();
    return REAL;
  }
  else if(tk.cat == ID){
     tipo1 = variavel();
             
     tipo2 = chamada_de_funcao2();
     if(tipo1 != tipo2){
        return tipo1;         
     }else return tipo2;  
  } 
  else if((tk.cat == SN) && (tk.cod == OPP)){
       tk = analex();
       
       tipo1 = expressao_simples();
      
       if((tk.cat == SN) && (tk.cod == CLP)){                 
           tk = analex();
           return tipo1;            
       } else erro(lin, 3);
  }  
}

void literal(void)
{
   if(tk.cat == CTL){
       tk = analex();                
   }
   else {
       tk = analex();     
   }
}

void erro(int lin, int err)
{	
	switch(err){
		case 1:
			printf("Identificador esperado na linha %d ", lin);
			exit(1);
			break;
		case 2:
			printf(" ( esperado na linha %d ", lin);
			exit(1);
			break;
		case 3:
			printf(" ) esperado na linha %d ", lin);
			exit(1);
			break;
		case 4:
			printf(" { esperado na linha %d ", lin);
			exit(1);
			break;
		case 5:
			printf(" } esperado na linha %d ", lin);
			exit(1);
			break;
		case 6:
			printf(" BEGIN esperado ");
			exit(1);
			break;
		case 7:
			printf(" Declaracao esperada na linha %d", lin);
			exit(1);
			break;
		case 8:
			printf(" ; Esperado na linha %d", lin);
			exit(1);
			break;
		case 9:
         printf(" ; ou ( esperado na linha %d", lin);
         exit(1);  
         break; 
      case 10:
         printf("Tentativa de insercao de id com mesmo nome na linha %d ", lin);
         exit(1); 
         break;
      case 11:
         printf("Id nao declarado: "); 
         exit(1);   
         break; 
      case 12:
         printf("Tentativa em realizar operacoes com tipos diferentes");
         exit(1);
         break;
      case 13:
         printf("Tipo de parametros incompativeis: ");
         exit(1);
         break;
      case 14:
         printf("Agurmento demais mo�o ");
         exit(1);
         break;
      case 15:
         printf("Argumento esperado ");
         exit(1);
         break;                       	
	}	
}


