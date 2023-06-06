#include "svg-dot.h"
#include <string.h>
#include "radialtree.h"
#include "geo.h"
#include "Bibliotecas/arqsvg.h"
#include "def.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    getInfosDentroRegiaoRadialT(All, SIZE_X1_Y1, SIZE_X1_Y1, SIZE_X2_Y2, SIZE_X2_Y2, GetRetangulo, Ret);
    getInfosDentroRegiaoRadialT(All, SIZE_X1_Y1, SIZE_X1_Y1, SIZE_X2_Y2, SIZE_X2_Y2, GetLinha, Lin);
    getInfosDentroRegiaoRadialT(All, SIZE_X1_Y1, SIZE_X1_Y1, SIZE_X2_Y2, SIZE_X2_Y2, GetCirculo, Circ);
    getInfosDentroRegiaoRadialT(All, SIZE_X1_Y1, SIZE_X1_Y1, SIZE_X2_Y2, SIZE_X2_Y2, GetTexto, Tex);

    /*Cria as figuras no SVG baseado nas listas*/
    Iterador R = createIterador(Ret, false);

    while (!isIteratorEmpty(Ret, R))
    {
        Info i = getInfoRadialT(All, getIteratorNext(Ret, R));
        CriaRetanguloSvg(B, i);
        MostraID(B, i);
    }

    Iterador L = createIterador(Lin, false);

    while (!isIteratorEmpty(Lin, L))
    {
        Info i = getInfoRadialT(All, getIteratorNext(Lin, L));
        CriaLinhaSvg(B, i);
        MostraID(B, i);
    }

    Iterador C = createIterador(Circ, false);

    while (!isIteratorEmpty(Circ, C))
    {
        Info i = getInfoRadialT(All, getIteratorNext(Circ, C));
        CriaCirculoSvg(B, i);
        MostraID(B, i);
    }

    Iterador T = createIterador(Tex, false);

    while (!isIteratorEmpty(Tex, T))
    {
        Info i = getInfoRadialT(All, getIteratorNext(Tex, T));
        CriaTextoSvg(B, i);
        MostraID(B, i);
    }

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

void InicializaDot(ArqDot fdot)
{
    fprintf(fdot, "digraph Tree {\n");
    fprintf(fdot, "node [shape=box];\n");
}

void TerminaDot(ArqDot fdot)
{
    if (fdot != NULL)
    {
        fprintf(fdot, "}");
        fclose(fdot);
    }
}

void CriaPngDot(char nome[])
{
    char ext[] = "dot";
    // Aloca memória para o nome do arquivo
    char nomearq[strlen(nome) + 10 + strlen(ext)];

    int n = 1;
    sprintf(nomearq, "%s.%s", nome, ext);

    // Verifica se o arquivo já existe
    FILE *vrfy = fopen(nomearq, "r");
    while (vrfy != NULL)
    {
        fclose(vrfy);
        char command[strlen(nomearq) + 30];
        sprintf(command, "dot -Tpng %s -o %s.png", nomearq, nomearq);
        system(command);
        n++;
        sprintf(nomearq, "%s-%d.%s", nome, n, ext);
        vrfy = fopen(nomearq, "r");
    }
}