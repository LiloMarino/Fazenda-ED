#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "dot.h"
#include "radialtree.h"
#include "Bibliotecas/learquivo.h"

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
    fflush(fdot);
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
        fprintf(fdot, "Raiz -> \"%c%d\"\n", Forma, ID);
    }
    else
    {
        char Forma1 = ((Figura *)getInfoRadialT(All, pai))->Tipo;
        int ID1 = ((Figura *)getInfoRadialT(All, pai))->ID;
        char Forma2 = ((Figura *)getInfoRadialT(All, filho))->Tipo;
        int ID2 = ((Figura *)getInfoRadialT(All, filho))->ID;
        fprintf(fdot, "\"%c%d\" -> \"%c%d\"\n", Forma1, ID1, Forma2, ID2);
    }
    fflush(fdot);
}

void MarcaNoRemovido(ArqDot fdot, RadialTree All, Node removido)
{
    char Forma = ((Figura *)getInfoRadialT(All, removido))->Tipo;
    int ID = ((Figura *)getInfoRadialT(All, removido))->ID;
    fprintf(fdot, "%c%d [shape=none, label=\"X\", color=red, fontcolor=red, fontsize=20, width=0.3, height=0.3];\n", Forma, ID);
    fflush(fdot);
}

void CopiaDot(ArqDot fdot, const char *OutputGeo)
{
    char fn[strlen(OutputGeo) + 5];
    strcpy(fn, OutputGeo);
    strcat(fn,".dot");
    FILE *faux = fopen(fn,"r");
    if (!faux)
    {
        printf("Erro ao copiar DOT");
    }
    else
    {
        char *buf = NULL;
        while (leLinha(faux,&buf))
        {
            if (buf[0] != '}')
            {
                fprintf(fdot,"%s",buf);
            }
        }
    }
    fflush(fdot);
    fclose(faux);
}

void CriaPngDot(const char nome[])
{
    char nomearq[strlen(nome) + 6];
    char nomepng[strlen(nome) + 6];

    int n = 1;
    sprintf(nomearq, "%s.dot", nome);
    sprintf(nomepng, "%s.png", nome);

    // Verifica se o arquivo já existe
    FILE *vrfy = fopen(nomearq, "r");
    while (vrfy != NULL)
    {
        fclose(vrfy);
        char command[2*strlen(nomearq) + 30];

        // Aplicar otimizações
        sprintf(command, "dot -Kneato -Gmaxiter=100 -Tpng %s -o %s", nomearq, nomepng);
        system(command);

        n++;
        sprintf(nomearq, "%s-%d.dot", nome, n);
        sprintf(nomepng, "%s-%d.png", nome, n);
        vrfy = fopen(nomearq, "r");
    }
}