#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Fil
{

   int inicio, fim, size;
   data dados[TAM_FILA];

} _fila;

Fila createQueue()
{
   _fila *aux = calloc(1, sizeof(_fila));

   aux->size = 0;
   aux->inicio = 0;
   aux->fim = 0;

   return (Fila)aux;
}

int tamQueue(Fila F)
{
   _fila *aux = (_fila *)F;
   return aux->size;
}

bool isQueueEmpty(Fila X)
{
   _fila *aux = (_fila *)X;

   if (aux->inicio == aux->fim)
      return true;
   else
      return false;
}

bool isQueueFull(Fila X){
   _fila* aux = (_fila*) X;

    if(((aux->fim + 1) % TAM_FILA == aux->inicio)) return true;
    else return false;
}

void insertQueue(Fila X, data elemento)
{
   if (isQueueFull(X))
   {
      printf("Fila cheia!\n");
      return;
   }
   else
   {
      _fila *aux = (_fila *)X;
      aux->dados[aux->fim] = elemento;
      aux->fim = (aux->fim + 1) % TAM_FILA;
      aux->size++;
   }
}

data getDataQueue(Fila X)
{
   if (isQueueEmpty(X))
      return 0;

   else
   {
      _fila *aux = (_fila *)X;

      data elemento = aux->dados[aux->inicio];
      aux->inicio = (aux->inicio + 1) % TAM_FILA;
      aux->size--;
      return elemento;
   }
}