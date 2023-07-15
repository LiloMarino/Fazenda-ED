#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "geo.h"
#include "dot.h"
#include "radialtree.h"
#include "Bibliotecas/learquivo.h"
#include "Bibliotecas/efficiency.h"

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
    fprintf(fdot, "    node [shape=box];\n");
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
        Figura* Child = getInfoRadialT(All, filho);
        char Forma = Child->Tipo;
        int ID = Child->ID;
        double X,Y;
        GetCoordenadas(&X,&Y,Child);
        fprintf(fdot, "    Raiz -> \"%c%d X:%0.2lf Y:%0.2lf\"\n", Forma, ID, X, Y);
    }
    else
    {
        Figura* Father = getInfoRadialT(All, pai);
        Figura* Child = getInfoRadialT(All, filho);
        char Forma1 = Father->Tipo;
        int ID1 = Father->ID;
        char Forma2 = Child->Tipo;
        int ID2 = Child->ID;
        double X1,Y1,X2,Y2;
        GetCoordenadas(&X1,&Y1,Father);
        GetCoordenadas(&X2,&Y2,Child);
        fprintf(fdot, "    \"%c%d X:%0.2lf Y:%0.2lf\" -> \"%c%d X:%0.2lf Y:%0.2lf\"\n", Forma1, ID1, X1, Y1, Forma2, ID2, X2, Y2);
    }
    fflush(fdot);
}

void MarcaNoRemovido(ArqDot fdot, RadialTree All, Node removido)
{
    Figura* Rmv = getInfoRadialT(All, removido);
    char Forma = Rmv->Tipo;
    int ID = Rmv->ID;
    double X,Y;
    GetCoordenadas(&X,&Y,Rmv);
    fprintf(fdot, "    \"%c%d X:%0.2lf Y:%0.2lf\" [shape=box, color=red, fontcolor=red];\n", Forma, ID, X, Y);
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
        sprintf(command, "dot -Tpng %s -o %s", nomearq, nomepng);
        printf("\nTempo para gerar o %s\n",nomepng);
        iniciarTempo();
        system(command);
        finalizarTempo();
        n++;
        sprintf(nomearq, "%s-%d.dot", nome, n);
        sprintf(nomepng, "%s-%d.png", nome, n);
        vrfy = fopen(nomearq, "r");
    }
}