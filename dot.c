#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "dot.h"
#include "radialtree.h"

struct StFigura
{
    int ID;
    char Tipo;
    Item Figura;
    int RefCount; // Necessário para não dar free() mais de uma vez
};

typedef struct StFigura Figura;

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

void LigaNo(ArqDot fdot, RadialTree All, Node pai, Node filho)
{
    if (pai == NULL)
    {
        char Forma = ((Figura *)getInfoRadialT(All, filho))->Tipo;
        int ID = ((Figura *)getInfoRadialT(All, filho))->ID;
        fprintf(fdot, "Raiz -> %c%d\n", Forma, ID);
    }
    else
    {
        char Forma1 = ((Figura *)getInfoRadialT(All, pai))->Tipo;
        int ID1 = ((Figura *)getInfoRadialT(All, pai))->ID;
        char Forma2 = ((Figura *)getInfoRadialT(All, filho))->Tipo;
        int ID2 = ((Figura *)getInfoRadialT(All, filho))->ID;
        fprintf(fdot, "%c%d -> %c%d\n", Forma1, ID1, Forma2, ID2);
    }
}

void MarcaNoRemovido(ArqDot fdot, RadialTree All, Node removido)
{
    char Forma = ((Figura *)getInfoRadialT(All, removido))->Tipo;
    int ID = ((Figura *)getInfoRadialT(All, removido))->ID;
    fprintf(fdot, "%c%d [shape=none, label=\"X\", color=red, fontcolor=red, fontsize=20, width=0.3, height=0.3];\n", Forma, ID);
}

void CriaPngDot(const char nome[])
{
    char ext[] = "dot";
    char nomearq[strlen(nome) + strlen(ext) + 2];

    int n = 1;
    sprintf(nomearq, "%s.%s", nome, ext);

    // Verifica se o arquivo já existe
    FILE *vrfy = fopen(nomearq, "r");
    while (vrfy != NULL)
    {
        fclose(vrfy);
        char command[2*strlen(nomearq) + 30];
        sprintf(command, "dot -Tpng %s -o %s.png", nomearq, nome);
        system(command);
        n++;
        sprintf(nomearq, "%s-%d.%s", nome, n, ext);
        vrfy = fopen(nomearq, "r");
    }
}