#ifndef LISTADUPLA_H
#define LISTADUPLA_H

#include <stdbool.h>

/**
Uma lista e´ uma colecao ordenada de itens (ou seja, um elemento possui um
antecessor e um sucessor)  em que seus elementos podem
ser acessados atraves de sua posicao (tipo Posic).
Uma lista sem nenhum elemento (comprimento 0) e' denominada lista vazia.

Uma instancia do tipo Posic indica a posicao de um item dentro da lista.
Este tipo possui um valor invalido que indica nao se referir a nenhum item.
Tal valor invalido e' denotado por NIL.

Este modulo prove diversos iteradores: 1 deles genérico e 3 comuns a linguagens
de programacao funcionais (high-order functions).
*/

typedef void *Lista;
typedef void *Posic;
typedef void *Item;

typedef void *Iterador;
typedef void *Clausura;

#define NIL NULL
#define CAPAC_ILIMITADA -1

/** Retorna uma lista vazia. A lista pode definir um limite maximo de
elementos armazenados (capacidade). Caso capacidade < 0, o tamanho da lista é
ilimitado */
Lista createLst(int capacidade);

/* Retorna o numero de elementos da lista. */
int lengthLst(Lista L);

/** Retorna a capacidade da lista. Retorna -1, se a capacidade é ilimitada */
int maxLengthLst(Lista L);

/** Retorna verdadeiro se a lista L estiver vazia */
bool isEmptyLst(Lista L);

/** Retorna verdadeiro se a lista L estiver cheia. Ou seja, se a lista
   tiver sido criada com uma capacidade máxima e lenght(L) == maxLength(L). */
bool isFullLst(Lista L);

/**  Insere o item info no final da lista L. O comprimento da
lista e' acrescido de 1 elemento.
Retorna um indicador para o elemento acrescentado; ou NIL, se a lista estiver
cheia */
Posic insertLst(Lista L, Item info);

/** Remove e retorna o primeiro elemento da lista L.
    A lista nao pode  estar vazia */
Item popLst(Lista L);

/** Remove o elemento da lista L indicado por p.  p deve indicar um elemento existente em L. O comprimento da lista e' diminuido de 1 elemento. */
void removeLst(Lista L, Posic p);

/** Retorna o valor do item da lista indicado por p.
    p deve indicar um elemento existente em L. */
Item getLst(Posic p);

/** Insere o item info na posicao imediatamente anterior ao
item indicado por p. O comprimento da lista e' acrescido de 1 elemento.
Retorna um indicador para o elemento acrescentado. p deve indicar um
elemento existente em L.*/
Posic insertBefore(Lista L, Posic p, Item info);

/** Insere o item info na posicao imediatamente posterior ao
item indicado por p. O comprimento da lista e' acrescido de 1 elemento.
Retorna um indicador para o elemento acrescentado. p deve indicar um
elemento existente em L.*/
Posic insertAfterLst(Lista L, Posic p, Item info);

/** Retorna o indicador do primeiro elemento de L. Se
length(L)=0, retorna NIL. */
Posic getFirstLst(Lista L);

/** Retorna o indicador do elemento de L seguinte ao elemento
indicado por p. Se p for o ultimo elemento da lista, retorna NIL.
p deve indicar um elemento existente em L.*/
Posic getNextLst(Lista L, Posic p);

/** Retorna o indicador do ultimo elemento de L. Se
length(L)=0, retorna NIL.*/
Posic getLastLst(Lista L);

/** Retorna o indicador do elemento de L anterior ao elemento
indicado por p. Se p for o primeiro elemento da lista, retorna NIL.
p deve indicar um elemento existente em L. */
Posic getPreviousLst(Lista L, Posic p);

/** Libera toda memoria alocada pela lista. */
void killLst(Lista L);

/**
 ** Iteradores
 **
 ** (nota: existe uma certa redundancia com getFirst, getLast,
 **  getNext e getPrevious).
 **/

/**
   Retorna um iterador sobre a lista L.
   O iterador "anda" do inicio ate' o fim, caso reverso for falso;
   do fim para o inicio, se reverso for verdadeiro.
 */
Iterador createIterador(Lista L, bool reverso);

/**
   Retorna verdadeiro caso o iterador esteja esgotado,
   i.e., todos os item ja' tenham sido retornados; falso,
   caso contrario.
 */
bool isIteratorEmpty(Lista L, Iterador it);

/**
   Retorna o proximo item. O iterador nao deve estar esgotado.
   Caso o item retornado seja o ultima, subsequentes invocacoes
   a isIteratorEmpty retornam verdadeiro.
 */
Item getIteratorNext(Lista L, Iterador it);

/**
   Libera os recursos usados pelo iterador.
 */
void killIterator(Iterador it);

/**
 ** High-order functions
 **/

typedef Item (*Apply)(Item item, void *aux);
typedef bool (*Check)(Item item, void *aux);
typedef void (*ApplyClosure)(Item item, Clausura c);

/** Cria uma nova lista. Aplica a funcao f a cada item de L
    e insere o resultado na nova lista.
 */
Lista map(Lista L, Apply f, void *aux);

/**
   Cria uma nova lista contendo os itens de L para os quais a
   invocacao da funcao f retornar verdeira.
 */
Lista filter(Lista L, Check f, void *aux);

/**
   Aplica a funcao f a cada elemento de L, possivelmente, atualizando
   o conteudo de c.
 */
void fold(Lista L, ApplyClosure f, Clausura c);

#endif