#include "listadupla.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct StListaDupla
{
    Item info;
    struct StListaDupla *prox;
    struct StListaDupla *ant;
};

struct StInfoLista
{
    struct StListaDupla *inicio;
    struct StListaDupla *final;
    int capac;
    int length;
};

struct StIterator
{
    struct StListaDupla *curr;
    bool reverso;
};

typedef struct StListaDupla ListaDupla;
typedef struct StInfoLista ListaInfo;
typedef struct StIterator IteratorInfo;

Lista createLst(int capacidade)
{
    if (capacidade < 0)
    {
        capacidade = CAPAC_ILIMITADA;
    }

    ListaInfo *lista = calloc(1, sizeof(ListaInfo));
    lista->capac = capacidade;

    return lista;
}

int lengthLst(Lista L)
{
    ListaInfo *lista = (ListaInfo *)L;
    return lista->length;
}

int maxLengthLst(Lista L)
{
    ListaInfo *lista = (ListaInfo *)L;
    if (lista->capac == CAPAC_ILIMITADA)
    {
        return -1;
    }
    else
    {
        return lista->capac;
    }
}

bool isEmptyLst(Lista L)
{
    return ((ListaInfo *)L)->length == 0;
}

bool isFullLst(Lista L)
{
    ListaInfo* lista = (ListaInfo*)L;
    if (lista->capac == CAPAC_ILIMITADA)
    {
        return false;
    }
    else
    {
        return lista->length == lista->capac;
    }
}

Posic insertLst(Lista L, Item info)
{
    ListaInfo *lista = (ListaInfo *)L;

    if (isFullLst(L))
    {
        return NIL; // Lista cheia, retorna NIL
    }

    ListaDupla *aux = calloc(1, sizeof(ListaDupla));
    aux->info = info;

    if (lista->inicio == NULL)
    {
        lista->inicio = aux; // Lista vazia, insere como o primeiro elemento
    }
    else
    {
        ListaDupla *p = lista->final;
        aux->ant = p;
        p->prox = aux;
    }

    lista->final = aux;
    lista->length++;

    return aux;
}

Item popLst(Lista L)
{
    ListaInfo *lista = (ListaInfo *)L;

    if (lista->inicio == NULL)
    {
        printf("Erro: Lista vazia\n");
        exit(1);
    }

    ListaDupla *aux = lista->inicio;
    lista->inicio = aux->prox;

    if (lista->inicio != NULL)
    {
        lista->inicio->ant = NULL;
    }
    else
    {
        lista->final = NULL;
    }

    Item item = aux->info;
    free(aux);
    lista->length--;

    return item;
}

void removeLst(Lista L, Posic p)
{
    ListaInfo *lista = (ListaInfo *)L;
    ListaDupla *rmv = (ListaDupla *)p;

    if (rmv == lista->inicio)
    {
        if (rmv->prox != NULL)
        {
            lista->inicio = rmv->prox;
            rmv->prox->ant = rmv->ant;
        }
        else
        {
            lista->inicio = NULL;
            lista->final = NULL; // Atualiza também o ponteiro final quando a lista fica vazia
        }
    }
    else if (rmv->prox != NULL)
    {
        rmv->prox->ant = rmv->ant;
        rmv->ant->prox = rmv->prox;
    }
    else
    {
        rmv->ant->prox = NULL;
        lista->final = rmv->ant; // Atualiza o ponteiro final quando o último elemento é removido
    }

    (lista->length)--;
    free(rmv);
}

Item getLst(Posic p)
{
    ListaDupla *aux = (ListaDupla *)p;
    return aux->info;
}

Posic insertBefore(Lista L, Posic p, Item info)
{
    // Verifica se a lista está cheia
    if (isFullLst(L))
    {
        return NIL;
    }
    else
    {
        // Insere um item na lista antes do item indicado por p
        ListaDupla *aux = (ListaDupla *)p;
        ListaDupla *new = malloc(sizeof(ListaDupla));
        new->info = info;
        new->prox = aux;
        aux->ant = new;
        new->ant = aux->ant;
        if (new->ant != NULL)
        {
            new->ant->prox = new;
        }
        else
        {
            ((ListaInfo *)L)->inicio = new;
        }
        (((ListaInfo *)L)->length)++;

        return new;
    }
}

Posic insertAfterLst(Lista L, Posic p, Item info)
{
    // Verifica se a lista está cheia
    if (isFullLst(L))
    {
        return NIL;
    }
    else
    {
        // Insere um item na lista depois do item indicado por p
        ListaDupla *aux = (ListaDupla *)p;
        ListaDupla *new = malloc(sizeof(ListaDupla));
        new->info = info;
        new->ant = aux;
        aux->prox = new;
        new->prox = aux->prox;
        if (new->prox != NULL)
        {
            new->prox->ant = new;
        }
        else
        {
            ((ListaInfo *)L)->final = new;
        }
        (((ListaInfo *)L)->length)++;

        return new;
    }
}

Posic getFirstLst(Lista L)
{
    if (((ListaInfo *)L)->length == 0)
    {
        return NIL;
    }
    else
    {
        return ((ListaInfo *)L)->inicio;
    }
}

Posic getNextLst(Lista L, Posic p)
{
    if (((ListaInfo *)L)->final == p)
    {
        return NIL;
    }
    else
    {
        return ((ListaDupla *)p)->prox;
    }
}

Posic getLastLst(Lista L)
{
    if (((ListaInfo *)L)->length == 0)
    {
        return NIL;
    }
    else
    {
        return ((ListaInfo *)L)->final;
    }
}

Posic getPreviousLst(Lista L, Posic p)
{
    if (((ListaInfo *)L)->inicio == p)
    {
        return NIL;
    }
    else
    {
        return ((ListaDupla *)p)->ant;
    }
}

void killLst(Lista L)
{
    ListaDupla *rmv, *p;
    ListaInfo *lista = (ListaInfo *)L;
    
    rmv = lista->inicio;
    p = lista->inicio;
    
    while (p != NULL)
    {
        p = p->prox;
        free(rmv);
        rmv = p;
    }
    
    lista->inicio = NULL;
    lista->final = NULL;
    lista->length = 0;
    free(L);
}

/*
 * Iterador
 */

Iterador createIterador(Lista L, bool reverso)
{
    IteratorInfo *it = malloc(sizeof(IteratorInfo));
    it->reverso = reverso;
    if (reverso)
    {
        it->curr = ((ListaInfo *)L)->final;
    }
    else
    {
        it->curr = ((ListaInfo *)L)->inicio;
    }
    return it;
}

bool isIteratorEmpty(Lista L, Iterador it)
{
    if (it != NULL)
    {
        if (((IteratorInfo *)it)->reverso)
        {
            return (((IteratorInfo *)it)->curr == NULL);
        }
        else
        {
            return (((IteratorInfo *)it)->curr == NULL);
        }
    }
    else
    {
        return true;
    }
}

Item getIteratorNext(Lista L, Iterador it)
{
    Item valor = ((IteratorInfo *)it)->curr->info;
    if (((IteratorInfo *)it)->reverso)
    {
        ((IteratorInfo *)it)->curr = (ListaDupla *)getPreviousLst(L, ((IteratorInfo *)it)->curr);
    }
    else
    {
        ((IteratorInfo *)it)->curr = (ListaDupla *)getNextLst(L, ((IteratorInfo *)it)->curr);
    }
    return valor;
}

void killIterator(Iterador it)
{
    free(it);
}

/*
 * High-order functions
 */

// A função "map" recebe uma lista "L" e um ponteiro para uma função "f" que será aplicada em cada elemento da lista.
// Ela retorna uma nova lista com os elementos transformados pela função "f".
Lista map(Lista L, Apply f, void *aux)
{
    Lista novaLista = createLst(((ListaInfo *)L)->capac);
    Iterador it = createIterador(L, false);
    while (!isIteratorEmpty(L, it))
    {
        Item item = getIteratorNext(L, it);
        insertLst(novaLista, f(item, aux));
    }
    killIterator(it);
    return novaLista;
}

// A função "filter" é similar à função "map", porém seleciona apenas os elementos que satisfazem a condição imposta por "f".
// Ela recebe uma Lista "L" e um ponteiro para uma função "f" que retorna true ou false.
// A função retorna uma nova lista contendo apenas os itens que satisfazem a condição.
Lista filter(Lista L, Check f, void *aux)
{
    Lista novaLista = createLst(((ListaInfo *)L)->capac);
    Iterador it = createIterador(L, false);
    while (!isIteratorEmpty(L, it))
    {
        Item item = getIteratorNext(L, it);
        if (f(item, aux))
        {
            insertLst(novaLista, item);
        }
    }
    killIterator(it);
    return novaLista;
}

// A função "fold" é usada para reduzir a lista L a um único valor, aplicando a função "f" sucessivamente em cada item da lista.
// Ela recebe uma lista "L", um ponteiro para uma função "f" e uma Clausura "c".
void fold(Lista L, ApplyClosure f, Clausura c)
{
    Iterador it = createIterador(L, false);
    while (!isIteratorEmpty(L, it))
    {
        Item item = getIteratorNext(L, it);
        f(item, c);
    }
    killIterator(it);
}
