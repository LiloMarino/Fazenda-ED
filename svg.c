#include <string.h>
#include "radialtree.h"
#include "geo.h"
#include "Bibliotecas/arqsvg.h"

void OperaSVG(char nome[], RadialTree All)
{
    /*Nomeia e abre o arquivo SVG*/
    char nomeSVG[strlen(nome) + 10];
    strcpy(nomeSVG, nome);
    strcat(nomeSVG, ".svg");
    ArqSvg B = abreEscritaSvg(nomeSVG);

    /*Cria as listas e separa as figuras por tipo e as insere nas listas*/
    Lista Ret = createLst(-1);
    Lista Lin = createLst(-1);
    Lista Circ = createLst(-1);
    Lista Tex = createLst(-1);

    getInfosDentroRegiaoRadialT(All, );

    /*Cria as figuras no SVG baseado nas listas*/
    Iterador R = createIterador(Ret, false);

    while (!isIteratorEmpty(Ret, R))
        CriaRetanguloSvg(B, getIteratorNext(Ret, R));

    Iterador L = createIterador(Lin, false);

    while (!isIteratorEmpty(Lin, L))
        CriaLinhaSvg(B, getIteratorNext(Lin, L));

    Iterador C = createIterador(Circ, false);

    while (!isIteratorEmpty(Circ, C))
        CriaCirculoSvg(B, getIteratorNext(Circ, C));

    Iterador T = createIterador(Tex, false);

    while (!isIteratorEmpty(Tex, T))
        CriaTextoSvg(B, getIteratorNext(Tex, T));

    killIterator(R);
    killIterator(L);
    killIterator(C);
    killIterator(T);

    /** @note Observar se limpar a lista vai limpar as structs que contém as informações internas dos objetos*/
    killLst(Ret);
    killLst(Lin);
    killLst(Circ);
    killLst(Tex);

    fechaSvg(B);
}