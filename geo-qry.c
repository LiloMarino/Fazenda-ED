#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "geo.h"
#include "Bibliotecas/arqsvg.h"
#include "Bibliotecas/learquivo.h"
#include "radialtree.h"
#include "qry.h"
#include "def.h"

/*========================================================================================================== *
 * Funções GEO                                                                                               *
 *========================================================================================================== */

#define TAM_VETOR 50 // Necessário para tirar os leaks de memória derivados das structs

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
    char comando[2];
    char *linha = NULL;
    EstiloTxt *style = malloc(sizeof(EstiloTxt));
    style->fFamily = my_strdup("arial");
    style->fWeight = NULL;
    style->fSize = NULL;
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

void FreeFigura(Info figura)
{
    Figura *f = figura;
    if (f->Tipo == 'R')
    {
        Retangulo *r = f->Figura;
        free(r);
    }
    else if (f->Tipo == 'C')
    {
        Circulo *c = f->Figura;
        free(c);
    }
    else if (f->Tipo == 'L')
    {
        Linha *l = f->Figura;
        free(l);
    }
    else if (f->Tipo == 'T')
    {
        Texto *t = f->Figura;
        free(t);
    }
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

struct StProcID
{
    int ID;
    Info NoInfo;
    double Nox;
    double Noy;
};

typedef struct StProcID ProcID;

ArqQry abreLeituraQry(char *fn)
{
    ArqQry fqry;
    fqry = fopen(fn, "r");
    return fqry;
}

void VerificaID(Info i, double x, double y, void *aux)
{
    Figura *F = i;
    if (F->ID == ((ProcID *)aux)->ID)
    {
        ((ProcID *)aux)->I = i;
        ((ProcID *)aux)->Nox = x;
        ((ProcID *)aux)->Noy = y;
    }
}

Info ProcuraID(int ID, RadialTree All)
{
    ProcID *aux = malloc(sizeof(ProcID));
    aux->ID = ID;
    visitaLarguraRadialT(All, VerificaID, aux);
    return aux;
}

void InterpretaQry(ArqQry fqry, RadialTree All, FILE *log, char *PathOutput)
{
    char comando[3];
    char *linha = NULL;
    int ID;
    while (leLinha(fqry, &linha))
    {
        sscanf(linha, "%s ", comando);
        if (strcmp(comando, "c") == 0)
        {
        }
        else if (strcmp(comando, "hvt") == 0)
        {
        }
        else if (strcmp(comando, "mv") == 0)
        {
            double dx, dy;
            int ID;
            sscanf(linha, "%s %d %f %f", comando, &ID, &dx, &dy);
            fprintf(log,"[*] %s %d %f %f\n", comando, ID, dx, dy);
            ProcID *I = ProcuraID(ID, All);
            Move(I->NoInfo, dx, dy, log);
            insertRadialT(All, I->Nox + dx, I->Noy + dy, I->NoInfo);
            removeNoRadialT(All, getNodeRadialT(All, I->Nox, I->Noy, EPSILON_PADRAO));
            free(I);
        }
        else if (strcmp(comando, "ct") == 0)
        {
        }
        else if (strcmp(comando, "cr") == 0)
        {
        }
        else if (strcmp(comando, "ad") == 0)
        {
        }
        else if (strcmp(comando, "st") == 0)
        {
        }
        else if (strcmp(comando, "d?") == 0)
        {
        }
        else if (strcmp(comando, "c?") == 0)
        {
        }
        else
        {
            printf("Comando desconhecido: %s\n", comando);
        }
    }
    if (linha != NULL)
    {
        free(linha);
    }
}

void Move(Info I, double dx, double dy, FILE *log)
{
    Figura *F = I;
    char forma = F->Tipo;
    fprintf(log, "Moveu:\n");
    if (forma == 'T')
    {
        Texto *t = F->Figura;
        fprintf(log, "Texto\n");
        fprintf(log, "ID: %d\n", t->ID);
        fprintf(log, "De\n");
        fprintf(log, "x:%f y:%f\n", t->x, t->y);
        t->x += dx;
        t->y += dy;
        fprintf(log, "Para\n");
        fprintf(log, "x:%f y:%f\n", t->x, t->y);
    }
    else if (forma == 'C')
    {
        Circulo *c = F->Figura;
        fprintf(log, "Circulo\n");
        fprintf(log, "ID: %d\n", c->ID);
        fprintf(log, "De\n");
        fprintf(log, "x:%f y:%f\n", c->x, c->y);
        c->x += dx;
        c->y += dy;
        fprintf(log, "Para\n");
        fprintf(log, "x:%f y:%f\n", c->x, c->y);
    }
    else if (forma == 'R')
    {
        Retangulo *r = F->Figura;
        fprintf(log, "Retangulo\n");
        fprintf(log, "ID: %d\n", r->ID);
        fprintf(log, "De\n");
        fprintf(log, "x:%f y:%f\n", r->x, r->y);
        r->x += dx;
        r->y += dy;
        fprintf(log, "Para\n");
        fprintf(log, "x:%f y:%f\n", r->x, r->y);
    }
    else if (forma == 'L')
    {
        Linha *l = F->Figura;
        fprintf(log, "Linha\n");
        fprintf(log, "ID:%d\n", l->ID);
        fprintf(log, "De\n");
        fprintf(log, "x1:%f y1:%f x2:%f y2:%f\n", l->x1, l->y1, l->x2, l->y2);
        l->x1 += dx;
        l->y1 += dy;
        l->x2 += dx;
        l->y2 += dy;
        fprintf(log, "Para\n");
        fprintf(log, "x1:%f y1:%f x2:%f y2:%f\n", l->x1, l->y1, l->x2, l->y2);
    }
    else
    {
        return;
    }
}

void fechaQry(ArqQry fqry)
{
    if (fqry != NULL)
    {
        fclose(fqry);
    }
}