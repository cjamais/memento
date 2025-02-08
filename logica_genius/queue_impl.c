#include "queue.h"

struct queue{
    T_Info* elements;
    int size;
    int last;
};

T_Queue init(int max_size){
    if(max_size<1){
        return NULL;
    }

    T_Queue queue = malloc(sizeof(struct queue));
    if(queue!=NULL){
        queue->last = -1;
        queue->size = max_size;
        //inicializando o vetor
        queue->elements = malloc(sizeof(T_Info)*queue->size);
        if(queue->elements==NULL){
            free(queue);
            queue = NULL;
        }
    }

    return queue;
}

void destroy(T_Queue queue){
    free(queue->elements);
    free(queue);
}

bool is_full(T_Queue queue){
    if(queue!=NULL)
        return queue->last == queue->size-1;
    else
        return false;
}

bool is_empty(T_Queue queue){
    if(queue!=NULL)
        return queue->last == -1;
    else
        return false;
}

bool enqueue(T_Queue queue, T_Info element){
    if(queue == NULL){
        return false;
    }
    if(is_full(queue)){
        return false;
    }

    queue->last += 1;
    queue->elements[queue->last] = element;
    return true;
}

bool dequeue(T_Queue queue, T_Info* element){
    if(queue == NULL){
        return false;
    }
    if(is_empty(queue)){
        return false;
    }

    *element = queue->elements[0];

    for(int i=0;i<queue->last;i++){
        queue->elements[i] = queue->elements[i+1];
    }
    queue->last -= 1;
    return true;
}