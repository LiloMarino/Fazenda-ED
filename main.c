#include "radialtree.h"
#include "string.h"
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
    getNodeRadialT(t, 0, 0, 0.1);
    getNodeRadialT(t, 1, 0.5, 0.1);
    getNodeRadialT(t, 0.5, 1, 0.1);
    getNodeRadialT(t, -0.5, 1, 0.1);
    getNodeRadialT(t, -1, 0.5, 0.1);
    getNodeRadialT(t, -1, -0.5, 0.1);
    getNodeRadialT(t, -0.5, -1, 0.1);
    getNodeRadialT(t, 0.5, -1, 0.1);
    getNodeRadialT(t, 1, -0.5, 0.1);
    removeNoRadialT(&t,getNodeRadialT(t, 0.6, 0.2, 0.1));
    removeNoRadialT(&t,getNodeRadialT(t, 1, 0.5, 0.1));
    freeRadialTree(&t,true);
}