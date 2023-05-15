#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "geo.h"
#include "qry.h"
#include "Bibliotecas/arqsvg.h"
#include "Bibliotecas/learquivo.h"
#include "radialtree.h"

/*========================================================================================================== *
 * Funções GEO                                                                                               *
 *========================================================================================================== */

struct StFigura
{
    int ID;
    char Tipo;
    Item Figura;
};

struct StCirculo
{
    int ID;
    double x, y, raio;
    char *corb, *corp;
};

struct StRetangulo
{
    int ID;
    double x, y, larg, alt, pont;
    char *corb, *corp;
};

struct StLinha
{
    int ID;
    double x1, x2, y1, y2;
    char *cor;
};

struct StTxtStyle
{
    char *fFamily, *fWeight, *fSize;
};

struct StTexto
{
    int ID;
    double x, y;
    char *corb, *corp, *txto, *fFamily, *fWeight, *fSize;
    char *a;
    char *rotacao;
};

typedef struct StFigura Figura;
typedef struct StCirculo Circulo;
typedef struct StRetangulo Retangulo;
typedef struct StLinha Linha;
typedef struct StTxtStyle EstiloTxt;
typedef struct StTexto Texto;

ArqGeo abreLeituraGeo(char *fn)
{
    ArqGeo fgeo;
    fgeo = fopen(fn, "r");
    return fgeo;
}

void InterpretaGeo(ArqGeo fgeo, RadialTree All)
{
    char comando[2];
    char *linha = NULL;
    EstiloTxt *style = malloc(sizeof(EstiloTxt));
    style->fFamily = my_strdup("arial");
    style->fWeight = NULL;
    style->fSize = NULL;
    while (leLinha(fgeo, &linha))
    {
        sscanf(linha, "%s", comando);
        if (strcmp(comando, "c") == 0)
        {
            Circulo *c = malloc(sizeof(Circulo));
            sscanf(linha, "%s %d %lf %lf %lf", comando, &c->ID, &c->x, &c->y, &c->raio);
            c->corb = getParametroI(linha, 5);
            c->corp = getParametroI(linha, 6);
            Figura *f = malloc(sizeof(Figura));
            f->ID = c->ID;
            f->Tipo = 'C';
            f->Figura = c;
            insertRadialT(All, c->x, c->y, f);
        }
        else if (strcmp(comando, "r") == 0)
        {
            Retangulo *r = malloc(sizeof(Retangulo));
            sscanf(linha, "%s %d %lf %lf %lf %lf", comando, &r->ID, &r->x, &r->y, &r->larg, &r->alt);
            r->corb = getParametroI(linha, 6);
            r->corp = getParametroI(linha, 7);
            r->pont = -1;
            Figura *f = malloc(sizeof(Figura));
            f->ID = r->ID;
            f->Tipo = 'R';
            f->Figura = r;
            insertRadialT(All, r->x, r->y, f);
        }
        else if (strcmp(comando, "l") == 0)
        {
            Linha *l = malloc(sizeof(Linha));
            sscanf(linha, "%s %d %lf %lf %lf %lf", comando, &l->ID, &l->x1, &l->y1, &l->x2, &l->y2);
            l->cor = getParametroI(linha, 6);
            Figura *f = malloc(sizeof(Figura));
            f->ID = l->ID;
            f->Tipo = 'L';
            f->Figura = l;
            insertRadialT(All, l->x1, l->y1, f);
        }
        else if (strcmp(comando, "ts") == 0)
        {
            if (style->fFamily != NULL)
            {
                free(style->fFamily);
            }
            if (style->fWeight != NULL)
            {
                free(style->fWeight);
            }
            if (style->fSize != NULL)
            {
                free(style->fSize);
            }
            style->fFamily = getParametroI(linha, 1);
            style->fWeight = getParametroI(linha, 2);
            style->fSize = getParametroI(linha, 3);
        }
        else if (strcmp(comando, "t") == 0)
        {
            Texto *t = malloc(sizeof(Texto));
            sscanf(linha, "%s %d %lf %lf", comando, &t->ID, &t->x, &t->y);
            t->corb = getParametroI(linha, 4);
            t->corp = getParametroI(linha, 5);
            t->a = getParametroI(linha, 6);
            t->txto = getParametroDepoisI(linha, 7);
            t->rotacao = NULL;
            if (style->fFamily != NULL)
            {
                t->fFamily = my_strdup(style->fFamily);
            }
            else
            {
                t->fFamily = NULL;
            }
            if (style->fWeight != NULL)
            {
                t->fWeight = my_strdup(style->fWeight);
            }
            else
            {
                t->fWeight = NULL;
            }
            if (style->fSize != NULL)
            {
                t->fSize = my_strdup(style->fSize);
            }
            else
            {
                t->fSize = NULL;
            }
            Figura *f = malloc(sizeof(Figura));
            f->ID = t->ID;
            f->Tipo = 'T';
            f->Figura = t;
            insertRadialT(All, t->x, t->y, f);
        }
        else
        {
            printf("Comando desconhecido: %s\n", comando);
        }
    }
    if (style->fFamily != NULL)
    {
        free(style->fFamily);
    }
    if (style->fWeight != NULL)
    {
        free(style->fWeight);
    }
    if (style->fSize != NULL)
    {
        free(style->fSize);
    }
    free(style);
}

void CriaRetanguloSvg(ArqSvg fsvg, Item info)
{
    Figura *f = info;
    Retangulo *r = f->Figura;
    char *deco = NULL;
    preparaDecoracao(&deco, 0, r->corb, r->corp, NULL, -1, -1, -1, r->pont);
    escreveRetanguloSvg(fsvg, r->x, r->y, r->larg, r->alt, deco);
}

void CriaCirculoSvg(ArqSvg fsvg, Item info)
{
    Figura *f = info;
    Circulo *c = f->Figura;
    char *deco = NULL;
    preparaDecoracao(&deco, 0, c->corb, c->corp, NULL, -1, -1, -1, -1);
    escreveCirculoSvg(fsvg, c->x, c->y, c->raio, deco);
}

void CriaLinhaSvg(ArqSvg fsvg, Item info)
{
    Figura *f = info;
    Linha *l = f->Figura;
    char *deco = NULL;
    preparaDecoracao(&deco, 0, l->cor, NULL, NULL, -1, -1, -1, -1);
    escreveLinhaSvg(fsvg, l->x1, l->y1, l->x2, l->y2, deco);
}

void CriaTextoSvg(ArqSvg fsvg, Item info)
{
    Figura *f = info;
    Texto *t = f->Figura;
    char *deco = NULL, *fontWeight = NULL, *textAnchor = NULL;
    if (*(t->a) == 'i')
    {
        textAnchor = my_strdup("start");
    }
    else if (*(t->a) == 'f')
    {
        textAnchor = my_strdup("end");
    }
    else
    {
        textAnchor = my_strdup("middle");
    }
    if (t->fWeight != NULL)
    {
        if (strcmp(t->fWeight, "l") == 0)
        {
            fontWeight = my_strdup("lighter");
        }
        else if (strcmp(t->fWeight, "b") == 0)
        {
            fontWeight = my_strdup("bold");
        }
        else if (strcmp(t->fWeight, "b+") == 0)
        {
            fontWeight = my_strdup("bolder");
        }
        else
        {
            fontWeight = my_strdup("normal");
        }
    }
    preparaDecoracaoTexto(&deco, 0, t->fFamily, NULL, fontWeight, t->fSize, t->corb, t->corp, textAnchor, t->rotacao);
    escreveTextoSvg(fsvg, t->x, t->y, t->txto, deco);
    free(fontWeight);
    free(textAnchor);
}

bool GetRetangulo(Info figura, double x1, double y1, double x2, double y2)
{
    Figura *f = figura;
    return (f->Tipo == 'R');
}

bool GetCirculo(Info figura, double x1, double y1, double x2, double y2)
{
    Figura *f = figura;
    return (f->Tipo == 'C');
}

bool GetLinha(Info figura, double x1, double y1, double x2, double y2)
{
    Figura *f = figura;
    return (f->Tipo == 'L');
}

bool GetTexto(Info figura, double x1, double y1, double x2, double y2)
{
    Figura *f = figura;
    return (f->Tipo == 'T');
}

void fechaGeo(ArqGeo fgeo)
{
    if (fgeo != NULL)
    {
        fclose(fgeo);
    }
}

/*========================================================================================================== *
 * Funções QRY                                                                                               *
 *========================================================================================================== */

ArqQry abreLeituraQry(char *fn)
{
    ArqQry fqry;
    fqry = fopen(fn, "r");
    return fqry;
}

void fechaQry(ArqQry fqry)
{
    if (fqry != NULL)
    {
        fclose(fqry);
    }
}