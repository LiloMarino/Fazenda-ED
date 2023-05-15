#include "radialtree.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include <string.h>
#include <stdio.h>

int main()
{
    RadialTree All = newRadialTree(8, 0.1);
    ArqGeo Geo00 = abreLeituraGeo("../00-smpls-caca-disp.geo");
    InterpretaGeo(Geo00,All);
    OperaSVG("00-smpls-caca-disp",All);
    fechaGeo(Geo00);
    freeRadialTree(&All, true);
    All = newRadialTree(8, 0.1);
    ArqGeo Geo01 = abreLeituraGeo("../01-caca-fotos.geo");
    InterpretaGeo(Geo01,All);
    OperaSVG("01-caca-fotos",All);
    fechaGeo(Geo01);
    freeRadialTree(&All, true);
    All = newRadialTree(8, 0.1);
    ArqGeo Geo02 = abreLeituraGeo("../01-smpls-balao-fotografa.geo");
    InterpretaGeo(Geo02,All);
    OperaSVG("01-smpls-balao-fotografa",All);
    fechaGeo(Geo02);
    freeRadialTree(&All, true);
    All = newRadialTree(8, 0.1);
    ArqGeo Geo03 = abreLeituraGeo("../02-planta-cidade.geo");
    InterpretaGeo(Geo03,All);
    OperaSVG("02-planta-cidade",All);
    fechaGeo(Geo03);
    freeRadialTree(&All, true);
    All = newRadialTree(8, 0.1);
    ArqGeo Geo04 = abreLeituraGeo("../03-cenario-grande.geo");
    InterpretaGeo(Geo04,All);
    OperaSVG("03-cenario-grande",All);
    fechaGeo(Geo04);
    freeRadialTree(&All, true);
}