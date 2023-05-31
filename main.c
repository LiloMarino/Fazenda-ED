#include "radialtree.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "Bibliotecas/geradores.h"
#include <string.h>
#include <stdio.h>

int main()
{
    FILE* log;

    RadialTree All = newRadialTree(8, 0.1);
    log = CriaLog("Registro","txt");
    ArqGeo Geo00 = abreLeituraGeo("../00-caso-de-teste.geo");
    ArqQry Qry00 = abreLeituraQry("../00-caso-de-teste.qry");
    InterpretaGeo(Geo00,All);
    OperaSVG("00-caso-de-teste.geo",All);
    InterpretaQry(Qry00,All,log,"../logs/");
    OperaSVG("00-caso-de-teste.qry",All);
    fechaGeo(Geo00);
    fechaQry(Qry00);
    fclose(log);
    freeRadialTree(&All, true);

    All = newRadialTree(8, 0.1);
    log = CriaLog("Registro","txt");
    ArqGeo Geo01 = abreLeituraGeo("../01-caso-de-teste.geo");
    ArqQry Qry01 = abreLeituraQry("../01-caso-de-teste.qry");
    InterpretaGeo(Geo01,All);
    OperaSVG("01-caso-de-teste.geo",All);
    InterpretaQry(Qry01,All,log,"../logs/");
    OperaSVG("01-caso-de-teste.qry",All);
    fechaGeo(Geo01);
    fechaQry(Qry01);
    fclose(log);
    freeRadialTree(&All, true);

    All = newRadialTree(8, 0.1);
    log = CriaLog("Registro","txt");
    ArqGeo Geo02 = abreLeituraGeo("../02-caso-de-teste.geo");
    ArqQry Qry02 = abreLeituraQry("../02-caso-de-teste.qry");
    InterpretaGeo(Geo02,All);
    OperaSVG("02-caso-de-teste.geo",All);
    InterpretaQry(Qry02,All,log,"../logs/");
    OperaSVG("02-caso-de-teste.qry",All);
    fechaGeo(Geo02);
    fechaQry(Qry02);
    fclose(log);
    freeRadialTree(&All, true);

    All = newRadialTree(8, 0.1);
    log = CriaLog("Registro","txt");
    ArqGeo Geo03 = abreLeituraGeo("../03-caso-de-teste.geo");
    ArqQry Qry03 = abreLeituraQry("../03-caso-de-teste.qry");
    InterpretaGeo(Geo03,All);
    OperaSVG("03-caso-de-teste.geo",All);
    InterpretaQry(Qry03,All,log,"../logs/");
    OperaSVG("03-caso-de-teste.qry",All);
    fechaGeo(Geo03);
    fechaQry(Qry03);
    fclose(log);
    freeRadialTree(&All, true);

    All = newRadialTree(8, 0.1);
    log = CriaLog("Registro","txt");
    ArqGeo Geo04 = abreLeituraGeo("../04-caso-de-teste.geo");
    ArqQry Qry04 = abreLeituraQry("../04-caso-de-teste.qry");
    InterpretaGeo(Geo04,All);
    OperaSVG("04-caso-de-teste.geo",All);
    InterpretaQry(Qry04,All,log,"../logs/");
    OperaSVG("04-caso-de-teste.qry",All);
    fechaGeo(Geo04);
    fechaQry(Qry04);
    fclose(log);
    freeRadialTree(&All, true);
}