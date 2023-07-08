#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radialtree.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "dot.h"
#include "config.h"
#include "Bibliotecas/geradores.h"
#include "Bibliotecas/path.h"

#if FINAL_DOT_ONLY != 1
FILE *ARQDOT;
char *FNARQDOT;
#endif 

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

    /* Executa o .geo */
    #if FINAL_DOT_ONLY != 1
    FNARQDOT = OutputGeo;
    ARQDOT = CriaLog(FNARQDOT, "dot");
    InicializaDot(ARQDOT);
    #endif
    
    InterpretaGeo(Geo, All);
    OperaSVG(OutputGeo, All);

    #if FINAL_DOT_ONLY != 1
    TerminaDot(ARQDOT);
    if (Qry == NULL)
    {
        #if SKIP_IMAGE_GENERATION != 1
        CriaPngDot(OutputGeo);
        #endif
    }
    else
    {
        DeleteDuplicates(FNARQDOT,"dot");
    }
    #endif

    /* Executa o .qry */
    if (Qry != NULL)
    {
        #if FINAL_DOT_ONLY != 1
        FNARQDOT = OutputGeoQry;
        ARQDOT = CriaLog(FNARQDOT, "dot");
        CopiaDot(ARQDOT, OutputGeo);
        #endif

        InterpretaQry(Qry, &All, log, OutputGeoQry);
        OperaSVG(OutputGeoQry, All);

        #if FINAL_DOT_ONLY != 1
        TerminaDot(ARQDOT);
        #endif
        #if SKIP_IMAGE_GENERATION != 1
        CriaPngDot(OutputGeoQry);
        #endif
    }

    /* Printa um único dot, caso a opção esteja ativada */
    #if FINAL_DOT_ONLY != 0
    if (Qry != NULL)
    {
        if (printDotRadialTree(All, OutputGeoQry))
        {
            printf("Arquivo dot criado com sucesso!");
        }
    }
    else
    {
        if (printDotRadialTree(All, OutputGeo))
        {
            printf("Arquivo dot criado com sucesso!");
        }
    }
    #endif

    /*Realiza todos os frees*/
    killRadialTree(&All);
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