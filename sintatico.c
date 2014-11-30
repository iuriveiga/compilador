#include "analisador.h"
#include "sintatico.h"
#include "semantico.h"

//Declaração de variaveis
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
   
	declaracao_variaveis(); // Como tratar se não houver variável?...	
	declaracao_funcao(); // ...Chamamos função mesmo assim e tratamos lá dentro quando chamar o analex?	
	declaracao_funcao_principal();
}

void declaracao_variaveis(void)
{
	//Verifica se o proximo token é do tipo CHAR
	if((tk.cat == PR) && (tk.p_reservada == CHAR)){
		aux.tipo = CHAR;
	
		tk = analex();
		
		//Verifica se o proximo token é um identificador;
		if(tk.cat == ID){
			strcpy(aux.id, tk.lexema);
		    
			tk = analex();			
		   
		   if((tk.cat == SN) && (tk.cod == OPP)){
               aux.sit = PEN;           
           }else{
                  novo.escopo = escopo;
                  novo.classe = VAR;
                  novo.tipo = CHAR;
                  strcpy(novo.lexema, aux.id);            
                  insere_simbolo(novo);
       			//Verifica se o proximo token é ','
       			if((tk.cat == SN) && (tk.cod == CMM)){
                  
       				while(tk.cod == CMM){
                                 
       					tk = analex();
       					
       					//Verifica se o proximo token é um ID
       					if(tk.cat == ID){ 
                       
                                                     
                            strcpy(novo.lexema, tk.lexema);
                            insere_simbolo(novo);//
                                         
       						tk = analex();
       					} else erro(lin, 1); //Chama a mensagem de erro
       				}				
       			}			
       			//Verifica se o proximo token é um ';'
       			if((tk.cat == SN) && (tk.cod == SMC)){
                          
       				tk = analex();
       				declaracao_variaveis();//Chama a declaraçaõ de variaveis novamente
       			} else {
       				//aux.sit = PEN;
                   erro(lin, 8);			
       			}
        }
				
		} else erro(lin, 1); //Chama a mesangem de erro
	}	
	
	//Verifica se o token é do tipo INT
	else if((tk.cat == PR) && (tk.p_reservada == INT)){
		aux.tipo = INT;
		tk = analex();
		
		//Verifica se o proximo token é um identificador;
		if(tk.cat == ID){
			strcpy(aux.id, tk.lexema);
			tk = analex();			
		
			
			if((tk.cat == SN) && (tk.cod == OPP)){
               aux.sit = PEN;           
            }else{
                novo.escopo = escopo;
                novo.classe = VAR;
                novo.tipo = INT;
                strcpy(novo.lexema, aux.id);            
                insere_simbolo(novo); 
       			//Verifica se o proximo token é ','
       			if((tk.cat == SN) && (tk.cod == CMM)){
                         
       				while(tk.cod == CMM){
       					tk = analex();
       					
       					//Verifica se o proximo token é um ID
       					if(tk.cat == ID){
                            strcpy(novo.lexema, tk.lexema);
                            insere_simbolo(novo);//      
       						tk = analex();
       					} else erro(lin, 1); //Chama a mensagem de erro
       				}				
       			}			
       			//Verifica se o proximo token é um ';'
       			if((tk.cat == SN) && (tk.cod == SMC)){
       				tk = analex();
       				declaracao_variaveis();//Chama a declaraçaõ de variaveis novamente
       			} else {
       				//aux.sit = PEN;
                   erro(lin, 8);			
       			}
       }
				
		} else erro(lin, 1); //Chama a mesangem de erro
	}	
	
	//Verifica se o tokem é um tipo FLOAT	
	else if((tk.cat == PR) && (tk.p_reservada == FLOAT)){
		aux.tipo = FLOAT;
		tk = analex();
		
		//Verifica se o proximo token é um identificador;
		if(tk.cat == ID){
			strcpy(aux.id, tk.lexema);
			tk = analex();			
		
			
		if((tk.cat == SN) && (tk.cod == OPP)){
               aux.sit = PEN;           
         }else{
            novo.escopo = escopo;
            novo.classe = VAR;
            novo.tipo = FLOAT;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);
   			//Verifica se o proximo token é ','
   			if((tk.cat == SN) && (tk.cod == CMM)){
             
   				while(tk.cod == CMM){
   					tk = analex();
   					
   					//Verifica se o proximo token é um ID
   					if(tk.cat == ID){
                        strcpy(novo.lexema, tk.lexema);
                        insere_simbolo(novo);    
   						tk = analex();
   					} else erro(lin, 1); //Chama a mensagem de erro
   				}				
   			}			
   			//Verifica se o proximo token é um ';'
   			if((tk.cat == SN) && (tk.cod == SMC)){
   				tk = analex();
   				declaracao_variaveis();//Chama a declaraçaõ de variaveis novamente
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
	//Se existir pendencia siginfica que o tipo o identificador e o parentese já foi chamado
	if(aux.sit == PEN){
	
		//Verifica se o tipo é CHAR
		if(aux.tipo == CHAR){
            
            novo.escopo = escopo;
            novo.classe = FUNC;
            novo.tipo = CHAR;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);                
			
            if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
			    escopo = LO;
				parametros_formais();
				//Verifica se o proximo token é um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					//Verifica se o proximo tokem é um '{'
					if((tk.cat == SN) && (tk.cod == OPB)){
						//Libreando aux
						aux.sit = LIV;	
						tk = analex();					
						bloco_de_funcao();
						//Verifica se o token é um '}'
						if((tk.cat == SN) && (tk.cod == CLB)){
                            escopo = GL;       
							tk = analex();
							
                     
							remove_simbolo();
                     zombie_simbolo();							
                     declaracao_funcao();
						} else erro(lin, 5); //Chama mensagem de erro Caso não venha '}' 
						
					} else erro(lin, 4); //Chama mensagem de erro caso não venh '{'
												
				} else erro(lin, 3); /*Chama mensagem de erro
									 caso não venha ')' */
			} else erro(lin, 2);						 
		}		
		//Verifica se o tipo é INT
		else if(aux.tipo == INT){
             
            novo.escopo = escopo;
            novo.classe = FUNC;
            novo.tipo = INT;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);        
             
			if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
			    escopo = LO;
				parametros_formais();
				//Verifica se o proximo token é um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					//Verifica se o proximo tokem é um '{'
					if((tk.cat == SN) && (tk.cod == OPB)){
						//Libreando aux
						aux.sit = LIV;	
						tk = analex();					
						bloco_de_funcao();
						//Verifica se o token é um '}'
						if((tk.cat == SN) && (tk.cod == CLB)){
							tk = analex();
							escopo = GL;
							
							
							remove_simbolo();
                     zombie_simbolo();	
							
							declaracao_funcao();
						} else erro(lin, 5); //Chama mensagem de erro Caso não venha '}' 
						
					} else erro(lin, 4); //Chama mensagem de erro caso não venh '{'
												
				} else erro(lin, 3); /*Chama mensagem de erro
									 caso não venha ')' */
			} else erro(lin, 2);						 
		}		
		//Verifica se o tipo é FLOAT
		else if(aux.tipo == FLOAT){
             
            novo.escopo = escopo;
            novo.classe = FUNC;
            novo.tipo = FLOAT;
            strcpy(novo.lexema, aux.id);            
            insere_simbolo(novo);        
             
			if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
	            
                escopo = LO;		
				parametros_formais();
				//Verifica se o proximo token é um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					//Verifica se o proximo tokem é um '{'
					if((tk.cat == SN) && (tk.cod == OPB)){
						//Libreando aux
						aux.sit = LIV;	
						tk = analex();					
						bloco_de_funcao();
						//Verifica se o token é um '}'
						if((tk.cat == SN) && (tk.cod == CLB)){
							tk = analex();
							
							escopo = GL;
							
							
							remove_simbolo();
                     zombie_simbolo();	
                            
							declaracao_funcao();
						} else erro(lin, 5); //Chama mensagem de erro Caso não venha '}' 
						
					} else erro(lin, 4); //Chama mensagem de erro caso não venh '{'
												
				} else erro(lin, 3); /*Chama mensagem de erro
									 caso não venha ')' */
			} else erro(lin, 2);						 
		}		
		
	} else { 
	
		//Verifica se o tipo é char
		if((tk.cat == PR) && (tk.p_reservada == CHAR)){
			tk = analex();
			//Verifica se o proximo token é um identificador
			if(tk.cat == ID){
                 
                novo.escopo = escopo;
                novo.classe = FUNC;
                novo.tipo = CHAR;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);              
                      
				tk = analex();
				//Verifica se o proximo token é um '('
				if((tk.cat == SN) && (tk.cod == OPP)){
					tk = analex();
					
					escopo = LO;
					parametros_formais();
					//Verifica se o proximo token é um ')'
					if((tk.cat == SN) && (tk.cod == CLP)){
						tk = analex();
						//Verifica se o proximo token é um '{'
						if((tk.cat == SN) && (tk.cod == OPB)){
							tk = analex();
							bloco_de_funcao();
							//Verifica se o proximo token é um '}'
							if((tk.cat == SN) && (tk.cod == CLB)){
								tk = analex();
								escopo = GL;
								
								
							   remove_simbolo();
                        zombie_simbolo();	
								
								declaracao_funcao();
							} else erro(lin, 5); //Chama mensage de erro caso não venha '}'
							
						} else erro(lin, 4); //Chama mensagem de erro caso não venha '{'
						
					} else erro(lin, 3); //Chama o erro quando não vem um )

				} else erro(lin, 2);//Chama mensagem de erro caso não venha um (
				
			} else erro(lin, 1);//Chama a mensage de erro
		}

		//Verifica se o tipo é INT
		if((tk.cat == PR) && (tk.p_reservada == INT)){
			tk = analex();
			//Verifica se o proximo token é um identificador
			if(tk.cat == ID){
                
                novo.escopo = escopo;
                novo.classe = FUNC;
                novo.tipo = INT;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);                    
                      
				tk = analex();
				//Verifica se o proximo token é um '('
				if((tk.cat == SN) && (tk.cod == OPP)){
					tk = analex();
					escopo = LO;
					parametros_formais();
					//Verifica se o proximo token é um ')'
					if((tk.cat == SN) && (tk.cod == CLP)){
						tk = analex();
						//Verifica se o proximo token é um '{'
						if((tk.cat == SN) && (tk.cod == OPB)){
							tk = analex();
							bloco_de_funcao();
							//Verifica se o proximo token é um '}'
							if((tk.cat == SN) && (tk.cod == CLB)){
								tk = analex();
								
								escopo = GL;
								
								
							   remove_simbolo();
                        zombie_simbolo();	
								
								declaracao_funcao();
							} else erro(lin, 5); //Chama mensage de erro caso não venha '}'
							
						} else erro(lin, 4); //Chama mensagem de erro caso não venha '{'
						
					} else erro(lin, 3); //Chama o erro quando não vem um )

				} else erro(lin, 2);//Chama mensagem de erro caso não venha um (
				
			} else erro(lin, 1);//Chama a mensage de erro
		}

		//Verifica se o tipo é FLOAT
		if((tk.cat == PR) && (tk.p_reservada == FLOAT)){
			tk = analex();
			//Verifica se o proximo token é um identificador
			if(tk.cat == ID){
                 
                novo.escopo = escopo;
                novo.classe = FUNC;
                novo.tipo = FLOAT;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);                    
                      
                      
				tk = analex();
				//Verifica se o proximo token é um '('
				if((tk.cat == SN) && (tk.cod == OPP)){
					tk = analex();
					escopo = LO;
					parametros_formais();
					//Verifica se o proximo token é um ')'
					if((tk.cat == SN) && (tk.cod == CLP)){
						tk = analex();
						//Verifica se o proximo token é um '{'
						if((tk.cat == SN) && (tk.cod == OPB)){
							tk = analex();
							bloco_de_funcao();
							//Verifica se o proximo token é um '}'
							if((tk.cat == SN) && (tk.cod == CLB)){
								tk = analex();
								escopo = GL;
								
								
							   remove_simbolo();
                        zombie_simbolo();	
								
								declaracao_funcao();
							} else erro(lin, 5); //Chama mensage de erro caso não venha '}'
							
						} else erro(lin, 4); //Chama mensagem de erro caso não venha '{'
						
					} else erro(lin, 3); //Chama o erro quando não vem um )

				} else erro(lin, 2);//Chama mensagem de erro caso não venha um (
				
			} else erro(lin, 1);//Chama a mensage de erro
		}

        //Verifica se o tipo é VOID
		if((tk.cat == PR) && (tk.p_reservada == VOID)){
			tk = analex();
			//Verifica se o proximo token é um identificador
			if(tk.cat == ID){
                novo.escopo = escopo;
                novo.classe = FUNC;
                novo.tipo = VOID;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);                    
                      
				tk = analex();
				//Verifica se o proximo token é um '('
				if((tk.cat == SN) && (tk.cod == OPP)){
					tk = analex();
					escopo = LO;
					parametros_formais();
					//Verifica se o proximo token é um ')'
					if((tk.cat == SN) && (tk.cod == CLP)){
						tk = analex();
						//Verifica se o proximo token é um '{'
						if((tk.cat == SN) && (tk.cod == OPB)){
							tk = analex();
							bloco_de_funcao();
							//Verifica se o proximo token é um '}'
							if((tk.cat == SN) && (tk.cod == CLB)){
								tk = analex();
								
								
							   remove_simbolo();
                        zombie_simbolo();	
								
								escopo = GL;
								declaracao_funcao();
							} else erro(lin, 5); //Chama mensage de erro caso não venha '}'
							
						} else erro(lin, 4); //Chama mensagem de erro caso não venha '{'
						
					} else erro(lin, 3); //Chama o erro quando não vem um )

				} else erro(lin, 2);//Chama mensagem de erro caso não venha um (
				
			} else erro(lin, 1);//Chama a mensage de erro
		}			
	}
	
}

void declaracao_funcao_principal(void)
{
     
	if ((tk.cat == PR) && (tk.p_reservada == MAIN)){
		tk = analex();
		
		novo.escopo = escopo;
        novo.classe = FUNC;
        strcpy(novo.lexema, "main");            
        insere_simbolo(novo); 
		
		
		
		escopo = LO; 
		//Verificando se o token é um '{'
		if((tk.cat == SN) && (tk.cod == OPB)){
			tk = analex();
			bloco_de_funcao();
			//verifica se o proximo tken é um '}'
			if((tk.cat == SN) && (tk.cod == CLB)){  
  	             printf("Tabela Declaracao Final \n\n");
	                              
				exit(0);//Finaliza a execução do programa
			} else erro(lin, 5); //Chama mensagem de erro caso não venha '}'			
			
		} else erro(lin, 4); //Chama mesangem de erro caso não venha um '{' 		
		
	} else erro(lin, 6); // Chama uma mensagem de erro caso não venha o MAIN	
}

void parametros_formais(void)
{
	//Apenas declaração de parametros os parenteses são tratados fora
	
	//Verifica se o tipo é CHAR
	if((tk.cat == PR) && (tk.p_reservada == CHAR)){
		tk = analex();
		//verifica se o proximo token é um identificador
		if(tk.cat == ID){
            
            novo.escopo = escopo;
            novo.classe = PARAM;
            novo.tipo = CHAR;
            strcpy(novo.lexema, tk.lexema);            
            insere_simbolo(novo);            
                  
			tk = analex();
		} else erro(lin, 1); //Chama uma mensagem de erro caso não venha um ID
	}
	
	//Verifica se o Tipo é INT
	else if((tk.cat == PR) && (tk.p_reservada == INT)){
		tk = analex();
		//verifica se o proximo token é um identificador
		if(tk.cat == ID){
                  
            novo.escopo = escopo;
            novo.classe = PARAM;
            novo.tipo = INT;
            strcpy(novo.lexema, tk.lexema);            
            insere_simbolo(novo);                 
                  
			tk = analex();
		} else erro(lin, 1); //Chama uma mensagem de erro caso não venha um ID
	}
	
	//Verifica se o tipo é FLOAT
	else if((tk.cat == PR) && (tk.p_reservada == FLOAT)){
		tk = analex();
		//verifica se o proximo token é um identificador
		if(tk.cat == ID){
            
            novo.escopo = escopo;
            novo.classe = PARAM;
            novo.tipo = FLOAT;
            strcpy(novo.lexema, tk.lexema);            
            insere_simbolo(novo);                  
                  
			tk = analex();
		} else erro(lin, 1); //Chama uma mensagem de erro caso não venha um ID
	}
	
	//Caso o proximo token for ','
	while((tk.cat == SN) && (tk.cod == CMM)){
		tk = analex();
		//Verifica se o tipo é CHAR
		if((tk.cat == PR) && (tk.p_reservada == CHAR)){
			tk = analex();
			//verifica se o proximo token é um identificador
			if(tk.cat == ID){
                
               novo.escopo = escopo;
               novo.classe = PARAM;
               novo.tipo = CHAR;
               strcpy(novo.lexema, tk.lexema);            
               insere_simbolo(novo);                  
                      
				tk = analex();
			} else erro(lin, 1); //Chama uma mensagem de erro caso não venha um ID
		}
	
		//Verifica se o Tipo é INT
		else if((tk.cat == PR) && (tk.p_reservada == INT)){
			tk = analex();
			//verifica se o proximo token é um identificador
			if(tk.cat == ID){
                
                novo.escopo = escopo;
                novo.classe = PARAM;
                novo.tipo = INT;
                strcpy(novo.lexema, tk.lexema);            
                insere_simbolo(novo);                  
                      
				tk = analex();
			} else erro(lin, 1); //Chama uma mensagem de erro caso não venha um ID
		}
	
		//Verifica se o tipo é FLOAT
		else if((tk.cat == PR) && (tk.p_reservada == FLOAT)){
			tk = analex();
			//verifica se o proximo token é um identificador
			if(tk.cat == ID){
                 
                 novo.escopo = escopo;
                 novo.classe = PARAM;
                 novo.tipo = FLOAT;
                 strcpy(novo.lexema, tk.lexema);            
                 insere_simbolo(novo);                 
                      
				tk = analex();
			} else erro(lin, 1); //Chama uma mensagem de erro caso não venha um ID
		
		} else erro(lin, 7); //Chama mensagem de erro caso não seja feita nenhuma declaração
	
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
				
		//Verifica se o proximo token é um '='
		if((tk.cat == SN) && (tk.cod == ATB)){
                   
         tipo1 = verifica_tabela(id);      
			tk = analex();			
			tipo2 = expressao();
			
			if(tipo1 != tipo2){
                
              erro(lin, 12);           
            }
         			
			//Verifica se o proximo token é um ';'
			if((tk.cat == SN) && (tk.cod == SMC)){
				tk = analex();
			             			
				comando();
				
			} else erro(lin, 8); //Chama mensagem de erro caso não venha um ';'
			
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
			//Verifica se o proximo token é um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
				//Verifica se o proximo token é um ';'
				if((tk.cat == SN) && (tk.cod == SMC)){
					tk = analex();
					aux.sit = LIV;
					comando();
				} else erro(lin, 8); // Chmama mensagem de erro caso não venha ';'
			} else erro(lin, 3); // Chama mensagem de erro caso não venha ')'
			
		} else 	erro(lin, 2);
		
	} else {	
		if(tk.cat == ID){
			tk = analex();
			//Verifica se o procximo token é um '('
			if((tk.cat == SN) && (tk.cod == OPP)){
				tk = analex();
				lista_de_expressoes();
				//Verifica se o proximo token é um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					//Verifica se o proximo token é um ';'
					if((tk.cat == SN) && (tk.cod == SMC)){
						tk = analex();
						comando();
					} else erro(lin, 8); // Chmama mensagem de erro caso não venha ';'
					
				} else erro(lin, 3); // Chama mensagem de erro caso não venha ')'
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
			   
			//Verifica se o proximo token é um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
                  
				tk = analex();
			
				aux.sit = LIV;
				return tipo;
			
			} else erro(lin, 3); // Chama mensagem de erro caso não venha ')'
		} else {
         if(tk.cod == CLP){
             aux.sit = LIV; 
                
             return;               
         }    
			aux.sit = LIV;
		}
	} else {	
		if(tk.cat == ID){
			tk = analex();
			//Verifica se o procximo token é um '('
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
			
				//Verifica se o proximo token é um ')'
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					aux.sit = LIV;
					return tipo;
				} else erro(lin, 3); // Chama mensagem de erro caso não venha ')'
			}
		}
	}
}

void comando_condicional(void)
{
	//Verifica se o proximo token é um IF
	if((tk.cat == PR) && (tk.p_reservada == IF)){
		tk = analex();
		//Verifica se o proximo token é um '('
		if((tk.cat == SN) && (tk.cod == OPP)){
			tk = analex();
			expressao();
			//verifica se o proximo token é um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
				//Verifica se o proximo token é um '{'
				if((tk.cat == SN) && (tk.cod == OPB)){
					tk = analex();
					comando();
					//Verifica se o proximo token é um '}'
					if((tk.cat == SN) && (tk.cod == CLB)){
						tk = analex();
						comando();
					} else erro( lin, 5); //chama erro caso não venha '}'
					
				} else erro(lin, 4); // chama err caso não venha '{'
				
			} else erro(lin, 3); // chama erro caso não venha ')'
			
		} else erro(lin, 2); // chama o erro caso não venha um '('
	
	}
	
	//Verifica se o proximo token é um ELSE
	else if((tk.cat == PR) && (tk.p_reservada == ELSE)){
		tk = analex();
		//Verifica se o proximo token é um '{'
		if((tk.cat == SN) && (tk.cod == OPB)){
			tk = analex();
			comando();
			//Verifica se o proximo token é um '}'
			if((tk.cat == SN) && (tk.cod == CLB)){
				tk = analex();
				comando();
			} else erro(lin, 5); //chama erro caso não venha '}'
			
		} else erro(lin, 4); //chama erro caso não venha '{'
	}
}

void comando_repetitivo(void)
{
	//Verifica se o proximo token é um WHILE
	if((tk.cat == PR) && (tk.p_reservada == WHILE)){
		tk = analex();
		//Verifica se o proximo token é '('
		if((tk.cat == SN) && (tk.cod == OPP)){
			tk = analex();
			expressao();
			//Verifica se o proximo token é um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
				//Verifica se o proximo token é '{'
				if((tk.cat == SN) && (tk.cod == OPB)){
					tk = analex();
					comando();
					//Verifica se o proximo token é '}'
					if((tk.cat == SN) && (tk.cod == CLB)){
						tk = analex();
						comando();
					} else erro(lin, 5);//Chama erro caso não venha '}'
					
				} else erro(lin, 4); //chama erro caso não venha '{'
				
			} else erro(lin, 3); //Chama erro caso não venha ')'
			
		} else erro(lin, 2);//Chama erro caso não venha '('
	}
}

void comando(void)
{
        
	atribuicao();	    

	chamada_de_funcao2(); //Verificar a possibilidade de testar diretamente em comando...para evitar erros.
	 
	comando_condicional();
	 
	comando_repetitivo();
	

	
	//Verifica se o proximo token é GET
    if((tk.cat == PR) && (tk.p_reservada == GET)){
		tk = analex();
		//Verifica se o proximo token é '('
		if((tk.cat == SN) && (tk.cod == OPP)){
			tk = analex();
			if(tk.cat == ID){               
				tk = analex();
				
				if((tk.cat == SN) && (tk.cod == CLP)){
					tk = analex();
					
					if((tk.cat == SN) && (tk.cod == SMC)){               
   					tk = analex();
   					comando();
					} else erro(lin, 8); //Chama erro caso não venha ';'
					
				} else erro(lin, 3); //Chama erro caso não venha ')'
				
			} else erro(lin, 1);
			
		} else erro(lin, 2);		
	}
	
	else if((tk.cat == PR) && (tk.p_reservada == PUT)){
		tk = analex();
		if((tk.cat == SN) && (tk.cod == OPP)){
			tk = analex();
			literal();
			expressao_simples();
			//Verifica se o proximo token é um ')'
			if((tk.cat == SN) && (tk.cod == CLP)){
				tk = analex();
				if((tk.cat == SN) && (tk.cod == SMC)){
					tk = analex();
					comando();
				} else erro(lin, 8); //Chama erro caso não venha ';'
				
			} else erro(lin, 3); //Chama erro caso não venha ')'
			
		} else erro(lin, 2);//Chama erro caso não venha '('
		
	}else	if((tk.cat == SN) && (tk.cod == SMC)){
        tk = analex();
        comando();     
   }
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
		 //printf("Variavel %s\n",aux.id);
		// printf("tipo %d\n", tipo);
		//Verifica se o proximo token é um ';'
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
    return INT;
  }
  	   
  else if(tk.cat == CTR){
    tk = analex();
    return FLOAT;
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
   else if(tk.cat == CTS){
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
			printf(" MAIN esperado ");
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
         printf("Agurmento demais moço ");
         exit(1);
         break;
      case 15:
         printf("Argumento esperado ");
         exit(1);
         break;                       	
            	
	}	
}
