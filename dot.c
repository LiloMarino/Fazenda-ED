#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "dot.h"

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