#ifndef FILA_
#define FILA_

#include <stdbool.h>
#define TAM_FILA 10000


typedef void *data;
typedef void *Fila;

// cria uma fila circular estatica com TAM_FILA espaços
Fila createQueue();

// checa se a fila esta vazia
bool isQueueEmpty(Fila X);

// checa se a fila esta cheia
bool isQueueFull(Fila X);

// insere no final da fila
void insertQueue(Fila X, data elemento);

// remove do começo da fila
data getDataQueue(Fila X);

int tamQueue(Fila F);

#endif