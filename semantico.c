#include "semantico.h"
#include "analisador.h"
#include "sintatico.h"

extern ver_par func;
extern int lin;
tabela_simbolos t_simb[MAXP];
enum escopos escopo;

int topo = 0;
int base = 0;


void insere_simbolo(tabela_simbolos novo)
{
     int i;

     if(topo < MAXP){     
         for(i = topo; i >= base; i--){
             
             if(strcmp(t_simb[i].lexema, novo.lexema) ==  0){
                                     
                  if(t_simb[i].escopo == novo.escopo){
                              
                     if(t_simb[i].classe != PARAM){
                           
                              
                               erro(lin, 10);                    
                                                              
                     }  else {
                             if(t_simb[i].zombie == F){
                            erro(lin, 10);                           
                            }                  
                        }                           
                  }
             }                                     
             
         }
        
         t_simb[topo].escopo = novo.escopo;
         t_simb[topo].tipo = novo.tipo;
         t_simb[topo].classe = novo.classe;
         strcpy(t_simb[topo].lexema, novo.lexema);
         topo++;     
     }
}

void remove_simbolo(void)
{
     int i;
     
     i = topo - 1;     
     
     while(i >= base ){
             
             
             if((t_simb[i].classe == VAR) && (t_simb[i].escopo == LO)){
                  
                  t_simb[i].classe = 0;
                  t_simb[i].tipo = 0;
                  t_simb[i].lexema[0] = '\0';
                  i--;
                  topo = i;                                     
             } else break;
                             
     }
    topo++; 
            
}

void zombie_simbolo(void)
{
    int i;
    
    
    
    i = topo - 1;
    
    while(i >= base){
        if(t_simb[i].classe == PARAM){              
              t_simb[i].zombie = T; 
              i--;
        }else break;                
    }    
}

int verifica_tabela(char id[])
{
   int i, j, k;
   int tipo1, tipo2;
   
   j = k = topo;
   i = 0;
   
     if(func.sit == PEN){
               
       while(i <= topo){
            if(strcmp(t_simb[i].lexema, func.id) ==  0){
               printf("verica %s\n", func.id);                          
               i++;                                           
               if(t_simb[i].classe == PARAM){
                   tipo1 = t_simb[i].tipo;
                   printf("simbo %s\n", t_simb[i].lexema);
                   printf("TIpo simb %d\n", tipo1); 
                   printf("ID %s\n", id);              
                   while(j >= base){
                      if(strcmp(t_simb[j].lexema, id) ==  0){
                          tipo2 = t_simb[j].tipo;
                          if(tipo1 == tipo2){
                             if(t_simb[i + 1].classe == PARAM){
                                 strcpy(func.id, t_simb[i].lexema);
                                 func.sit = PEN;
                                 return tipo1;                    
                             }else{
                                func.sit = LIV;
                                return tipo1;          
                             }               
                          } else{
                             erro(lin, 13);           
                          }                                                   
                      }
                      j--;                                        
                   }                              
               }
            }            
         i++;      
       }        
   }

       while(k >= base){
          if(strcmp(t_simb[k].lexema, id) ==  0){
              if(t_simb[k].classe == VAR){
                   return t_simb[k].tipo;
                  // return tipo;                    
              }
              else if(t_simb[k].classe == PARAM){
                   return t_simb[k].tipo;                
              }
              else if(t_simb[k].classe == FUNC){
                   return t_simb[k].tipo;               
              }                   
                                      
          }  
          
          k--;       
       } 
  
   printf("%s nao declarado ", id);
   exit(1);        
}





void imprime_tabela(void)
{
   int i;
   
   for(i = topo; i >= base; i--){
       if(t_simb[i].classe == VAR){
            if(t_simb[i].tipo == INTEGER){
                printf("%s ",t_simb[i].lexema);
                printf("VAR ");
                printf("INTEGER ");
                printf("%d\n ",t_simb[i].escopo);                 
            }else if(t_simb[i].tipo == REAL){
                printf("%s ",t_simb[i].lexema);
                printf("VAR ");
                printf("REAL ");
                printf("%d\n ",t_simb[i].escopo);     
            }else if(t_simb[i].tipo == CHAR){
                printf("%s ",t_simb[i].lexema);
                printf("VAR ");
                printf("CHAR ");
                printf("%d\n ",t_simb[i].escopo);     
            }                                   
       }else if(t_simb[i].classe == PARAM){
            if(t_simb[i].tipo == INTEGER){
                printf("%s ",t_simb[i].lexema);
                printf("PARAM ");
                printf("INTEGER ");
                printf("%d ",t_simb[i].escopo);
                printf("%d\n ",t_simb[i].zombie);                 
            }else if(t_simb[i].tipo == REAL){
                printf("%s ",t_simb[i].lexema);
                printf("PARAM ");
                printf("FLOAT ");
                printf("%d ",t_simb[i].escopo);
                printf("%d\n ",t_simb[i].zombie);       
            }else if(t_simb[i].tipo == CHAR){
                printf("%s ",t_simb[i].lexema);
                printf("PARAM ");
                printf("CHAR ");
                printf("%d ",t_simb[i].escopo);  
                printf("%d\n ",t_simb[i].zombie);     
            }                                    
       }else if(t_simb[i].classe == FUNC){
             
             if(t_simb[i].tipo == INTEGER){
                printf("%s ",t_simb[i].lexema);
                printf("FUNC ");
                printf("INTEGER ");
                printf("%d\n ",t_simb[i].escopo);                 
            }else if(t_simb[i].tipo == REAL){
                printf("%s ",t_simb[i].lexema);
                printf("FUNC ");
                printf("REAL ");
                printf("%d\n ",t_simb[i].escopo);     
            }else if(t_simb[i].tipo == CHAR){
                printf("%s ",t_simb[i].lexema);
                printf("FUNC ");
                printf("CHAR ");
                printf("%d\n ",t_simb[i].escopo);     
            }                        
       }        
   }
          
}
