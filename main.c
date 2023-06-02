#include "radialtree.h"
#include "geo.h"
#include "qry.h"
#include "svg-dot.h"
#include "Bibliotecas/geradores.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

FILE* ARQDOT;

int main()
{
    ARQDOT = CriaLog("../logs/Arvore","dot");
    InicializaDot(ARQDOT);
    FILE* log;
    RadialTree All = newRadialTree(3, 0.1);
    log = CriaLog("../logs/Registro","txt");
    ArqGeo Geo00 = abreLeituraGeo("../caso-de-teste.geo");
    ArqQry Qry00 = abreLeituraQry("../caso-de-teste.qry");
    InterpretaGeo(Geo00,All);
    OperaSVG("caso-de-teste.geo",All);
    InterpretaQry(Qry00,&All,log,"../logs/");
    //OperaSVG("caso-de-teste.qry",All);
    fechaGeo(Geo00);
    fechaQry(Qry00);
    fclose(log);
    freeRadialTree(&All, true);
    CriaPngDot(ARQDOT);
    TerminaDot(ARQDOT);
}