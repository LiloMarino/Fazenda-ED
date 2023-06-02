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
        CriaRetanguloSvg(B, getInfoRadialT(All, getIteratorNext(Ret, R)));

    Iterador L = createIterador(Lin, false);

    while (!isIteratorEmpty(Lin, L))
        CriaLinhaSvg(B, getInfoRadialT(All, getIteratorNext(Lin, L)));

    Iterador C = createIterador(Circ, false);

    while (!isIteratorEmpty(Circ, C))
        CriaCirculoSvg(B, getInfoRadialT(All, getIteratorNext(Circ, C)));

    Iterador T = createIterador(Tex, false);

    while (!isIteratorEmpty(Tex, T))
        CriaTextoSvg(B, getInfoRadialT(All, getIteratorNext(Tex, T)));

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

void CriaPngDot(ArqDot fdot)
{
    int descritor = fileno(fdot);
    char nome_do_arquivo[256];

    if (descritor != -1)
    {
        ssize_t tamanho = readlink("/proc/self/fd/<descritor>", nome_do_arquivo, sizeof(nome_do_arquivo) - 1);
        if (tamanho != -1)
        {
            nome_do_arquivo[tamanho] = '\0';
            printf("O nome do arquivo é: %s\n", nome_do_arquivo);
        }
        else
        {
            printf("Não foi possível obter o nome do arquivo.\n");
        }
    }
    else
    {
        printf("Não foi possível obter o descritor de arquivo.\n");
    }

    char command[1000];
    sprintf(command, "dot -Tpng %s -o %s.png", nome_do_arquivo, nome_do_arquivo);
    system(command);
}