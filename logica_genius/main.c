#include <stdio.h>
#include <time.h>
#include "queue.h"

int main(){

    T_Queue fila, sequencia, fila_copia;
    T_Info saiu, esperado, entrada_usuario;
    int errou = 0;

    fila = init(20);
    sequencia = init(20);
    fila_copia = init(20);
    
    srand(time(NULL));
    
    for(int i=0;i<20;i++){
        int random = (rand() % 4) + 1;
        printf("%d %s\n", random, enqueue(fila,random) ? "Entrou" : "Não entrou"); 
    }
    
    int rodada = 1;
    
    while(rodada<=20 && !errou){
        printf("Rodada %d\nCores que piscaram:",rodada);
        for(int i=0;i<rodada;i++){
            dequeue(fila,&saiu);
            enqueue(sequencia,saiu);
            enqueue(fila_copia,saiu);
            printf(" %d",saiu);
        }
        while(!is_empty(fila)){
            dequeue(fila,&saiu);
            enqueue(fila_copia,saiu);
        }
        printf("\nVez do usuário:\n");
        while(!is_empty(sequencia)){
            dequeue(sequencia,&esperado);
            scanf("%d",&entrada_usuario);
            if(entrada_usuario==esperado){
                printf("OK\n");
            }else{
                printf("ERROU!\n");
                errou = 1;
                break;
            }
        }
        
        while(!is_empty(fila_copia)){
            dequeue(fila_copia,&saiu);
            enqueue(fila,saiu);
        }
        rodada++;
    }
    
    destroy(fila_copia);
    destroy(sequencia);
    destroy(fila);
    return 0;
}