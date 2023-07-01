#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radialtree.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "dot.h"
#include "def.h"
#include "Bibliotecas/geradores.h"
#include "Bibliotecas/path.h"
#include "Bibliotecas/efficiency.h"

FILE *ARQDOT;
char *FNARQDOT;

int main(int argc, char **argv)
{
    char *PathInput = NULL, *PathOutput = NULL, *nomeGeo = NULL, *nomeQry = NULL, *numSetor = NULL, *fator = NULL;
    char *OutputGeoQry = NULL, *OutputGeo = NULL, *InputQry = NULL, *InputGeo = NULL;

    ArgumentosDeComando(&PathInput, &PathOutput, &nomeGeo, &nomeQry, &numSetor, &fator, argc, argv);

    ArrumaPath(&PathInput, &PathOutput);
    joinFilePath(PathInput, nomeGeo, &InputGeo);
    joinFilePath(PathInput, nomeQry, &InputQry);

    nomeQry = getFileName(nomeQry);
    char *nomeGeoQry = ConcatenaNomes(nomeGeo, nomeQry);
    char *nomeGeo_semExt = RemoveExtensao(nomeGeo);

    joinFilePath(PathOutput, nomeGeo_semExt, &OutputGeo);
    joinFilePath(PathOutput, nomeGeoQry, &OutputGeoQry);

    /* Inicia o processamento de todas as informações e produz os resultados */
    RadialTree All = newRadialTree(atoi(numSetor), atoi(fator) / 100.0);
    ArqGeo Geo = abreLeituraGeo(InputGeo);
    ArqQry Qry = abreLeituraQry(InputQry);
    FILE *log;
    if (Qry != NULL)
    {
        log = CriaLog(OutputGeoQry, "txt");
    }
    else
    {
        log = CriaLog(OutputGeo, "txt");
    }

    FNARQDOT = OutputGeo;
    ARQDOT = CriaLog(FNARQDOT, "dot");
    InicializaDot(ARQDOT);
    InterpretaGeo(Geo, All);
    OperaSVG(OutputGeo, All);
    TerminaDot(ARQDOT);
    if (Qry == NULL)
    {
        #if SKIP_IMAGE_GENERATION != 1
        iniciarTempo();
        CriaPngDot(OutputGeo);
        finalizarTempo();
        #endif
    }

    if (Qry != NULL)
    {
        FNARQDOT = OutputGeoQry;
        ARQDOT = CriaLog(FNARQDOT, "dot");
        CopiaDot(ARQDOT, OutputGeo);
        InterpretaQry(Qry, &All, log, OutputGeoQry);
        OperaSVG(OutputGeoQry, All);
        TerminaDot(ARQDOT);
        #if SKIP_IMAGE_GENERATION != 1
        iniciarTempo();
        CriaPngDot(OutputGeoQry);
        finalizarTempo();
        #endif
    }

    /*Realiza todos os frees*/
    freeRadialTree(&All, true);
    fechaGeo(Geo);
    fechaQry(Qry);
    fclose(log);
    free(PathInput);
    free(PathOutput);
    free(InputGeo);
    free(InputQry);
    free(OutputGeo);
    free(OutputGeoQry);
    free(nomeQry);
    free(nomeGeoQry);
    free(nomeGeo_semExt);
}