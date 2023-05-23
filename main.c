#include "radialtree.h"
#include <string.h>
#include <stdio.h>
int main()
{
    RadialTree t = newRadialTree(8, 0.1);
    insertRadialT(t, 0, 0, strdup("Ponto A"));
    insertRadialT(t, 1, 0.5, strdup("Ponto C"));
    insertRadialT(t, 0.5, 1, strdup("Ponto D"));
    insertRadialT(t, -0.5, 1, strdup("Ponto E"));
    insertRadialT(t, -1, 0.5, strdup("Ponto F"));
    insertRadialT(t, -1, -0.5, strdup("Ponto G"));
    insertRadialT(t, -0.5, -1, strdup("Ponto H"));
    insertRadialT(t, 0.5, -1, strdup("Ponto I"));
    insertRadialT(t, 1, -0.5, strdup("Ponto J"));
    insertRadialT(t, 0.6, 0.2, strdup("Ponto K"));
    Lista Teste;
    // Imprime
    printf("Area Total:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, 1, -1, -1, 1, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);
    // Remove os nós
    removeNoRadialT(&t, getNodeRadialT(t, 1, -0.5, 0.1)); // Ponto K
    removeNoRadialT(&t, getNodeRadialT(t, 1, 0.5, 0.1)); // Ponto C
    // Imprime
    printf("Area Total:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, 1, -1, -1, 1, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);
    freeRadialTree(&t, true);
}