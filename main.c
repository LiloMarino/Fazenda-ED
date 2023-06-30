#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radialtree.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "dot.h"
#include "Bibliotecas/geradores.h"
#include "Bibliotecas/path.h"

FILE *ARQDOT;
char *FNARQDOT;

int main(int argc, char **argv)
{
    char *PathInput = NULL, *PathOutput = NULL, *nomeGeo = NULL, *nomeQry = NULL, *numSetor = NULL, *fator = NULL;
    char *OutputGeoQry = NULL, *OutputGeo = NULL, *InputQry = NULL, *InputGeo = NULL;

    ArgumentosDeComando(&PathInput, &PathOutput, &nomeGeo, &nomeQry, &numSetor, &fator, argc, argv);
    char *nomeGeoQry = ConcatenaNomes(nomeGeo, nomeQry);
    char *nomeGeo_semExt = RemoveExtensao(nomeGeo);

    /* Arruma o Input e o Output adicionando a barra final */
    if (PathInput[strlen(PathInput) - 1] != '/')
    {
        char *PathInput1 = malloc(strlen(PathInput) + 2);
        strcpy(PathInput1, PathInput);
        strcat(PathInput1, "/");
        PathInput = PathInput1;
    }
    if (PathOutput[strlen(PathOutput) - 1] != '/')
    {
        char *PathOutput1 = malloc(strlen(PathOutput) + 2);
        strcpy(PathOutput1, PathOutput);
        strcat(PathOutput1, "/");
        PathOutput = PathOutput1;
    }

    joinFilePath(PathInput, nomeGeo, &InputGeo);
    joinFilePath(PathInput, nomeQry, &InputQry);
    joinFilePath(PathOutput, nomeGeo_semExt, &OutputGeo);
    joinFilePath(PathOutput, nomeGeoQry, &OutputGeoQry);
    FNARQDOT = OutputGeoQry;

    /* Inicia o processamento de todas as informações e produz os resultados */
    RadialTree All = newRadialTree(atoi(numSetor), atoi(fator)/100.0);
    ArqGeo Geo = abreLeituraGeo(InputGeo);
    ArqQry Qry = abreLeituraQry(InputQry);
    FILE *log = CriaLog(OutputGeoQry, "txt");
    ARQDOT = CriaLog(FNARQDOT, "dot");
    InicializaDot(ARQDOT);
    InterpretaGeo(Geo, All);
    OperaSVG(OutputGeo, All);
    InterpretaQry(Qry, &All, log);
    OperaSVG(OutputGeoQry, All);

    /*Realiza todos os frees*/
    freeRadialTree(&All, true);
    fechaGeo(Geo);
    fechaQry(Qry);
    fclose(log);
    TerminaDot(ARQDOT);
    CriaPngDot(OutputGeoQry);
    free(PathInput);
    free(PathOutput);
    free(InputGeo);
    free(InputQry);
    free(OutputGeo);
    free(OutputGeoQry);
    free(nomeGeoQry);
    free(nomeGeo_semExt);
}