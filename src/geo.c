#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Bibliotecas/learquivo.h"
#include "geo.h"

struct StFigura
{
    int ID;
    char Tipo;
    Item Figura;
    int RefCount; // Necessário para não dar free() mais de uma vez
};

struct StCirculo
{
    int ID;
    double x, y, raio;
    char corb[TAM_VETOR], corp[TAM_VETOR];
};

struct StRetangulo
{
    int ID;
    double x, y, larg, alt, pont;
    char corb[TAM_VETOR], corp[TAM_VETOR];
};

struct StLinha
{
    int ID;
    double x1, x2, y1, y2;
    char cor[TAM_VETOR];
};

struct StTxtStyle
{
    char *fFamily, *fWeight, *fSize;
};

struct StTexto
{
    int ID;
    double x, y;
    char corb[TAM_VETOR], corp[TAM_VETOR], txto[TAM_VETOR], fFamily[TAM_VETOR], fWeight[TAM_VETOR], fSize[TAM_VETOR];
    char a[TAM_VETOR];
    char rotacao[TAM_VETOR];
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
    char comando[3];
    char *linha = NULL;
    EstiloTxt *style = calloc(1, sizeof(EstiloTxt));
    style->fFamily = my_strdup("arial");
    while (leLinha(fgeo, &linha))
    {
        linha[strcspn(linha, "\n")] = '\0';
        sscanf(linha, "%s", comando);
        if (strcmp(comando, "c") == 0)
        {
            Circulo *c = malloc(sizeof(Circulo));
            sscanf(linha, "%s %d %lf %lf %lf", comando, &c->ID, &c->x, &c->y, &c->raio);
            char *aux = getParametroI(linha, 5);
            strcpy(c->corb, aux);
            free(aux);
            aux = getParametroI(linha, 6);
            strcpy(c->corp, aux);
            free(aux);
            Figura *f = malloc(sizeof(Figura));
            f->ID = c->ID;
            f->Tipo = 'C';
            f->Figura = c;
            insertRadialT(All, c->x, c->y, f);
            f->RefCount = 1; // Pois foi inserido apenas na árvore
        }
        else if (strcmp(comando, "r") == 0)
        {
            Retangulo *r = malloc(sizeof(Retangulo));
            sscanf(linha, "%s %d %lf %lf %lf %lf", comando, &r->ID, &r->x, &r->y, &r->larg, &r->alt);
            char *aux = getParametroI(linha, 6);
            strcpy(r->corb, aux);
            free(aux);
            aux = getParametroI(linha, 7);
            strcpy(r->corp, aux);
            free(aux);
            r->pont = -1;
            Figura *f = malloc(sizeof(Figura));
            f->ID = r->ID;
            f->Tipo = 'R';
            f->Figura = r;
            insertRadialT(All, r->x, r->y, f);
            f->RefCount = 1; // Pois foi inserido apenas na árvore
        }
        else if (strcmp(comando, "l") == 0)
        {
            Linha *l = malloc(sizeof(Linha));
            sscanf(linha, "%s %d %lf %lf %lf %lf", comando, &l->ID, &l->x1, &l->y1, &l->x2, &l->y2);
            char *aux = getParametroI(linha, 6);
            strcpy(l->cor, aux);
            free(aux);
            Figura *f = malloc(sizeof(Figura));
            f->ID = l->ID;
            f->Tipo = 'L';
            f->Figura = l;
            insertRadialT(All, l->x1, l->y1, f);
            f->RefCount = 1; // Pois foi inserido apenas na árvore
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
            sscanf(linha, "%s %d %lf %lf %s %s %s %[^\n]", comando, &t->ID, &t->x, &t->y, t->corb, t->corp, t->a, t->txto);
            t->rotacao[0] = '\0';
            if (style->fFamily != NULL)
            {
                strcpy(t->fFamily, style->fFamily);
            }
            else
            {
                t->fFamily[0] = '\0';
            }
            if (style->fWeight != NULL)
            {
                strcpy(t->fWeight, style->fWeight);
            }
            else
            {
                t->fWeight[0] = '\0';
            }
            if (style->fSize != NULL)
            {
                strcpy(t->fSize, style->fSize);
            }
            else
            {
                t->fSize[0] = '\0';
            }
            Figura *f = malloc(sizeof(Figura));
            f->ID = t->ID;
            f->Tipo = 'T';
            f->Figura = t;
            insertRadialT(All, t->x, t->y, f);
            f->RefCount = 1; // Pois foi inserido apenas na árvore
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

void fechaGeo(ArqGeo fgeo)
{
    if (fgeo != NULL)
    {
        fclose(fgeo);
    }
}

void CriaRetanguloSvg(ArqSvg fsvg, Item info)
{
    Figura *f = info;
    Retangulo *r = f->Figura;
    char *deco = NULL;
    preparaDecoracao(&deco, 0, r->corb, r->corp, NULL, -1, -1, -1, r->pont);
    escreveRetanguloSvg(fsvg, r->x, r->y, r->larg, r->alt, deco);
    if (deco != NULL)
    {
        free(deco);
    }
}

void CriaCirculoSvg(ArqSvg fsvg, Item info)
{
    Figura *f = info;
    Circulo *c = f->Figura;
    char *deco = NULL;
    preparaDecoracao(&deco, 0, c->corb, c->corp, NULL, -1, -1, -1, -1);
    escreveCirculoSvg(fsvg, c->x, c->y, c->raio, deco);
    if (deco != NULL)
    {
        free(deco);
    }
}

void CriaLinhaSvg(ArqSvg fsvg, Item info)
{
    Figura *f = info;
    Linha *l = f->Figura;
    char *deco = NULL;
    preparaDecoracao(&deco, 0, l->cor, NULL, NULL, -1, -1, -1, -1);
    escreveLinhaSvg(fsvg, l->x1, l->y1, l->x2, l->y2, deco);
    if (deco != NULL)
    {
        free(deco);
    }
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
    preparaDecoracaoTexto(&deco, 0, t->fFamily, NULL, fontWeight, t->fSize, t->corb, t->corp, textAnchor, t->rotacao);
    escreveTextoSvg(fsvg, t->x, t->y, t->txto, deco);
    if (deco != NULL)
    {
        free(deco);
    }
    free(fontWeight);
    free(textAnchor);
}

void GetRetangulo(Info figura, double x, double y, void *aux)
{
    Lista L = aux;
    Figura *f = figura;
    if (f->Tipo == 'R')
    {
        insertLst(L, figura);
    }
}

void GetCirculo(Info figura, double x, double y, void *aux)
{
    Lista L = aux;
    Figura *f = figura;
    if (f->Tipo == 'C')
    {
        insertLst(L, figura);
    }
}

void GetLinha(Info figura, double x, double y, void *aux)
{
    Lista L = aux;
    Figura *f = figura;
    if (f->Tipo == 'L')
    {
        insertLst(L, figura);
    }
}

void GetTexto(Info figura, double x, double y, void *aux)
{
    Lista L = aux;
    Figura *f = figura;
    if (f->Tipo == 'T')
    {
        insertLst(L, figura);
    }
}

void FreeFigura(Info figura)
{
    Figura *f = (Figura *)figura;
    if (f->RefCount == 1)
    {
        switch (f->Tipo)
        {
        case 'R':
        {
            Retangulo *r = (Retangulo *)f->Figura;
            free(r);
            break;
        }
        case 'C':
        {
            Circulo *c = (Circulo *)f->Figura;
            free(c);
            break;
        }
        case 'L':
        {
            Linha *l = (Linha *)f->Figura;
            free(l);
            break;
        }
        case 'T':
        {
            Texto *t = (Texto *)f->Figura;
            free(t);
            break;
        }
        }
        free(f);
    }
    else
    {
        f->RefCount--;
    }
}

void MostraID(ArqSvg fsvg, Item info)
{
    Figura *F = info;
    char *deco = NULL;
    char cor[] = "#000000";
    char ancora[] = "middle";
    char size[] = "5";
    char weight[] = "bold";
    preparaDecoracaoTexto(&deco, 0, NULL, NULL, weight, size, NULL, cor, ancora, NULL);
    char txto[30];
    sprintf(txto, "%c%d", F->Tipo, F->ID);
    double x, y;
    if (F->Tipo == 'T')
    {
        Texto *t = F->Figura;
        x = t->x;
        y = t->y;
    }
    else if (F->Tipo == 'C')
    {
        Circulo *c = F->Figura;
        x = c->x;
        y = c->y;
    }
    else if (F->Tipo == 'R')
    {
        Retangulo *r = F->Figura;
        x = r->x;
        y = r->y;
    }
    else if (F->Tipo == 'L')
    {
        Linha *l = F->Figura;
        x = l->x1;
        y = l->y1;
    }
    else
    {
        free(deco);
        return;
    }
    escreveTextoSvg(fsvg, x, y, txto, deco);
    if (deco != NULL)
    {
        free(deco);
    }
}