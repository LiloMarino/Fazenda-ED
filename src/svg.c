#include <string.h>
#include <stdlib.h>
#include "svg.h"
#include "config.h"
#include "geo.h"
#include "Bibliotecas/geradores.h"
#include "Bibliotecas/arqsvg.h"
#include "Bibliotecas/path.h"


void OperaSVG(char nome[], RadialTree All)
{
    ArqSvg B = abreEscritaSvg(nome);

    /*Cria as listas e separa as figuras por tipo e as insere nas listas*/
    Lista Ret = createLst(-1);
    Lista Lin = createLst(-1);
    Lista Circ = createLst(-1);
    Lista Tex = createLst(-1);

    visitaLarguraRadialT(All, GetRetangulo, Ret);
    visitaLarguraRadialT(All, GetLinha, Lin);
    visitaLarguraRadialT(All, GetCirculo, Circ);
    visitaLarguraRadialT(All, GetTexto, Tex);

    /*Cria as figuras no SVG baseado nas listas*/
    Iterador R = createIterador(Ret, false);

    while (!isIteratorEmpty(Ret, R))
    {
        Info i = getIteratorNext(Ret, R);
        CriaRetanguloSvg(B, i);
        #if SHOW_ID_FIGURA == 1
        MostraID(B, i);
        #endif
    }

    Iterador L = createIterador(Lin, false);

    while (!isIteratorEmpty(Lin, L))
    {
        Info i = getIteratorNext(Lin, L);
        CriaLinhaSvg(B, i);
        #if SHOW_ID_FIGURA == 1
        MostraID(B, i);
        #endif
    }

    Iterador C = createIterador(Circ, false);

    while (!isIteratorEmpty(Circ, C))
    {
        Info i = getIteratorNext(Circ, C);
        CriaCirculoSvg(B, i);
        #if SHOW_ID_FIGURA == 1
        MostraID(B, i);
        #endif
    }

    Iterador T = createIterador(Tex, false);

    while (!isIteratorEmpty(Tex, T))
    {
        Info i = getIteratorNext(Tex, T);
        CriaTextoSvg(B, i);
        #if SHOW_ID_FIGURA == 1
        MostraID(B, i);
        #endif
    }

    killIterator(R);
    killIterator(L);
    killIterator(C);
    killIterator(T);

    killLst(Ret);
    killLst(Lin);
    killLst(Circ);
    killLst(Tex);

    fechaSvg(B);
}

void CriaFrame(RadialTree All,char *OutputGeoQry, int *num)
{
    char *path = getPath(OutputGeoQry), *file = getFileName(OutputGeoQry);
    char nomepasta[strlen(file) + 10];
    sprintf(nomepasta, "frames%s", file);
    CriaPasta(path,nomepasta);
    char nome[strlen(path) + strlen(nomepasta) + strlen(file) + 10];
    sprintf(nome, "%s/%s/%s-frame-%d", path, nomepasta, file, *num);
    (*num)++;
    OperaSVG(nome, All);
    free(path);
    free(file);
}

