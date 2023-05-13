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
    printf("Area C:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, -0.8, 1.4, 1.2, -1.6, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    printf("Area C^-1:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, 1.2, -1.6, -0.8, 1.4, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    printf("Area L:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, -0.8 + 2, 1.4, 1.2 + 2, -1.6, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    printf("Area N:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, -0.8, 1.4 + 2, 1.2, -1.6 + 2, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    printf("Area O:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, -0.8 - 2, 1.4, 1.2 - 2, -1.6, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    printf("Area S:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, -0.8, 1.4 - 2, 1.2, -1.6 - 2, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    printf("Area 1:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, 0.2, 1.5, 2.2, -1.5, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    printf("Area 1^-1:\n");
    Teste = createLst(-1);
    getNodesDentroRegiaoRadialT(t, 2.2, -1.5, 0.2, 1.5, Teste);
    while (!isEmptyLst(Teste))
    {
        Node n = popLst(Teste);
        printf("%s\n", (char *)getInfoRadialT(t, n));
    }
    killLst(Teste);

    freeRadialTree(&t, true);
}