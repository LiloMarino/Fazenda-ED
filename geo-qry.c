#include "svg-dot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "geo.h"
#include "Bibliotecas/arqsvg.h"
#include "Bibliotecas/learquivo.h"
#include "Bibliotecas/utilities.h"
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
            f->RefCount = 1;
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
            f->RefCount = 1;
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
            f->RefCount = 1;
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
            f->RefCount = 1;
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

void fechaGeo(ArqGeo fgeo)
{
    if (fgeo != NULL)
    {
        fclose(fgeo);
    }
}

/*========================================================================================================== *
 * Funções DOT                                                                                               *
 *========================================================================================================== */

void LigaNo(ArqDot fdot, RadialTree All, Node pai, Node filho)
{
    if (pai == NULL)
    {
        char Forma = ((Figura *)getInfoRadialT(All, filho))->Tipo;
        int ID = ((Figura *)getInfoRadialT(All, filho))->ID;
        fprintf(fdot, "Raiz -> %c%d\n", Forma, ID);
    }
    else
    {
        char Forma1 = ((Figura *)getInfoRadialT(All, pai))->Tipo;
        int ID1 = ((Figura *)getInfoRadialT(All, pai))->ID;
        char Forma2 = ((Figura *)getInfoRadialT(All, filho))->Tipo;
        int ID2 = ((Figura *)getInfoRadialT(All, filho))->ID;
        fprintf(fdot, "%c%d -> %c%d\n", Forma1, ID1, Forma2, ID2);
    }
}

void MarcaNoRemovido(ArqDot fdot, RadialTree All, Node removido)
{
    char Forma = ((Figura *)getInfoRadialT(All, removido))->Tipo;
    int ID = ((Figura *)getInfoRadialT(All, removido))->ID;
    fprintf(fdot, "%c%d [shape=none, label=\"X\", color=red, fontcolor=red, fontsize=20, width=0.3, height=0.3];\n", Forma, ID);
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

struct StEntidade
{
    bool IsColheita; // Diz se é uma colheitadeira
    int ID;          // ID da colheitadeira
    Info Fig;        // Informação contida no nó da colheitadeira
    double Nox;      // Coordenada do nó da colheitadeira
    double Noy;      // Coordenada do nó da colheitadeira
};

struct StHortalica
{
    int ID;
    Info Fig;
    double Mult;
};

struct StContabiliza
{
    double abobora;
    double morango;
    double repolho;
    double cebola;
    double cenoura;
    double mato_linha;
    double mato_texto;
};

struct StProcAfetado
{
    Lista Atingido;         // Lista que conterá as informações dos nós atingidos
    double x, y, larg, alt; // Especificações da área afetada
};

typedef struct StProcID ProcID;
typedef struct StEntidade Entidade;
typedef struct StHortalica Hortalica;
typedef struct StContabiliza Contabiliza;
typedef struct StProcAfetado ProcAfetado;

ArqQry abreLeituraQry(char *fn)
{
    ArqQry fqry;
    fqry = fopen(fn, "r");
    return fqry;
}

void InterpretaQry(ArqQry fqry, RadialTree *All, FILE *log, char *PathOutput)
{
    char comando[3];
    char *linha = NULL;
    char nome[25]; // Remover depois
    int num = 0;   // Remover depois
    Lista Entidades = createLst(-1);
    Lista Colheita = createLst(-1);
    Lista Afetados = createLst(-1);
    while (leLinha(fqry, &linha))
    {
        sscanf(linha, "%s ", comando);
        if (strcmp(comando, "c") == 0)
        {
            int ID;
            sscanf(linha, "%s %d", comando, &ID);
            fprintf(log, "\n[*] %s %d\n", comando, ID);
            ProcID *I = ProcuraID(ID, *All);
            Entidade *C = malloc(sizeof(Entidade));
            C->ID = I->ID;
            C->Nox = I->Nox;
            C->Noy = I->Noy;
            C->Fig = I->NoInfo;
            C->IsColheita = true;
            insertLst(Entidades, C);
            ((Figura *)I->NoInfo)->RefCount++;
            free(I);
        }
        else if (strcmp(comando, "hvt") == 0)
        {
            int ID, Passos;
            char Direcao;
            sscanf(linha, "%s %d %d %c", comando, &ID, &Passos, &Direcao);
            fprintf(log, "\n[*] %s %d %d %c\n", comando, ID, Passos, Direcao);
            Harvest(ID, Passos, Direcao, log, Entidades, All, Colheita);
        }
        else if (strcmp(comando, "mv") == 0)
        {
            double dx, dy;
            int ID;
            sscanf(linha, "%s %d %lf %lf", comando, &ID, &dx, &dy);
            fprintf(log, "\n[*] %s %d %lf %lf\n", comando, ID, dx, dy);
            Move(ID, dx, dy, log, All);
        }
        else if (strcmp(comando, "ct") == 0)
        {
            double x, y, larg, alt, raio;
            sscanf(linha, "%s %lf %lf %lf %lf %lf", comando, &x, &y, &larg, &alt, &raio);
            fprintf(log, "\n[*] %s %lf %lf %lf %lf %lf\n", comando, x, y, larg, alt, raio);
            Praga(x, y, larg, alt, raio, Afetados, Entidades, All);
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
            int ID;
            sscanf(linha, "%s %d", comando, &ID);
            fprintf(log, "\n[*] %s %d\n", comando, ID);
            DadosI(ID, *All, log);
        }
        else if (strcmp(comando, "c?") == 0)
        {
            fprintf(log, "\n[*] %s\n", comando);
            InfoColheitadeiras(Entidades, log, *All);
        }
        else
        {
            printf("Comando desconhecido: %s\n", comando);
        }
        sprintf(nome, "%d-caso-de-teste.qry", num); // Remover depois
        num++;                                      // Remover depois
        OperaSVG(nome, *All);                       // Remover depois
    }
    if (linha != NULL)
    {
        free(linha);
    }
    while (!isEmptyLst(Entidades))
    {
        FreeEntidade(popLst(Entidades));
    }
    while (!isEmptyLst(Colheita))
    {
        FreeHortalica(popLst(Colheita));
    }
    while (!isEmptyLst(Afetados))
    {
        FreeHortalica(popLst(Afetados));
    }
    killLst(Entidades);
    killLst(Colheita);
    killLst(Afetados);
}

void Harvest(int ID, int Passos, char Direcao, FILE *log, Lista Entidades, RadialTree *All, Lista Colheita)
{
    /* Procura a Colheitadeira ID */
    Entidade *C;
    bool Encontrada = false;
    Iterador E = createIterador(Entidades, false);
    while (!isIteratorEmpty(Entidades, E))
    {
        C = getIteratorNext(Entidades, E);
        if (C->ID == ID && C->IsColheita)
        {
            Encontrada = true;
            break;
        }
    }
    killIterator(E);
    if (!Encontrada)
    {
        printf("Colheitadeira nao encontrada ID: %d\n", ID);
        fprintf(log, "Colheitadeira nao encontrada ID: %d\n", ID);
        return;
    }

    /* Obtém a distância a ser percorrida e obtém as coordenadas da área de colheita ambos baseados na direção*/
    Figura *F = C->Fig;
    Retangulo *R = F->Figura;
    double Xinicio, Yinicio, Xfim, Yfim;
    double dx = 0, dy = 0;
    if (Direcao == 'n')
    {
        dy = -(R->alt) * Passos;
        Xinicio = R->x;
        Yinicio = R->y + dy;
        Xfim = R->x + R->larg;
        Yfim = R->y + R->alt;
    }
    else if (Direcao == 's')
    {
        dy = (R->alt) * Passos;
        Xinicio = R->x;
        Yinicio = R->y;
        Xfim = R->x + R->larg;
        Yfim = R->y + dy + R->alt;
    }
    else if (Direcao == 'l')
    {
        dx = (R->larg) * Passos;
        Xinicio = R->x;
        Yinicio = R->y;
        Xfim = R->x + dx + R->larg;
        Yfim = R->y + R->alt;
    }
    else if (Direcao == 'o')
    {
        dx = -(R->larg) * Passos;
        Xinicio = R->x + dx;
        Yinicio = R->y;
        Xfim = R->x + R->larg;
        Yfim = R->y + R->alt;
    }

    /* Reporta os atributos de ID e suas posições */
    DadosI(ID, *All, log);
    fprintf(log, "\nPosicao Original:\n");
    fprintf(log, "X: %lf\n", R->x);
    fprintf(log, "Y: %lf\n", R->y);
    fprintf(log, "Posicao Final:\n");
    fprintf(log, "X: %lf\n", R->x + dx);
    fprintf(log, "Y: %lf\n\n", R->y + dy);

    /* Colhe os elementos na área e remove os nós da árvore sem remover a informação do nó inserindo na lista colheita apenas as hortaliças*/
    ColheElementos(All, Entidades, Colheita, Xinicio, Yinicio, Xfim, Yfim);

    /* Contabiliza a colheita e reporta */
    ContabilizaColheita(Colheita, log);
    fprintf(log, "\n");

    /* Realiza o movimento da colheitadeira e marca a área colhida para o svg */
    Move(ID, dx, dy, log, All);
    CriaArea(*All, Entidades, Xinicio, Yinicio, Xfim, Yfim);
}

void Move(int ID, double dx, double dy, FILE *log, RadialTree *All)
{
    ProcID *I = ProcuraID(ID, *All);
    if (getNodeRadialT(*All, I->Nox + dx, I->Noy + dy, EPSILON_PADRAO) == NULL)
    {
        Figura *F = I->NoInfo;
        char forma = F->Tipo;
        fprintf(log, "Moveu: ");
        if (forma == 'T')
        {
            Texto *t = F->Figura;
            fprintf(log, "Texto\n");
            fprintf(log, "ID: %d\n", t->ID);
            fprintf(log, "De\n");
            fprintf(log, "X:%f\nY:%f\n", t->x, t->y);
            t->x += dx;
            t->y += dy;
            fprintf(log, "Para\n");
            fprintf(log, "X:%f\nY:%f\n", t->x, t->y);
        }
        else if (forma == 'C')
        {
            Circulo *c = F->Figura;
            fprintf(log, "Circulo\n");
            fprintf(log, "ID: %d\n", c->ID);
            fprintf(log, "De\n");
            fprintf(log, "X:%f\nY:%f\n", c->x, c->y);
            c->x += dx;
            c->y += dy;
            fprintf(log, "Para\n");
            fprintf(log, "X:%f\nY:%f\n", c->x, c->y);
        }
        else if (forma == 'R')
        {
            Retangulo *r = F->Figura;
            fprintf(log, "Retangulo\n");
            fprintf(log, "ID: %d\n", r->ID);
            fprintf(log, "De\n");
            fprintf(log, "X:%f\nY:%f\n", r->x, r->y);
            r->x += dx;
            r->y += dy;
            fprintf(log, "Para\n");
            fprintf(log, "X:%f\nY:%f\n", r->x, r->y);
        }
        else if (forma == 'L')
        {
            Linha *l = F->Figura;
            fprintf(log, "Linha\n");
            fprintf(log, "ID:%d\n", l->ID);
            fprintf(log, "De\n");
            fprintf(log, "X1:%f\nY1:%f\nX2:%f\nY2:%f\n", l->x1, l->y1, l->x2, l->y2);
            l->x1 += dx;
            l->y1 += dy;
            l->x2 += dx;
            l->y2 += dy;
            fprintf(log, "Para\n");
            fprintf(log, "X1:%f\nY1:%f\nX2:%f\nY2:%f\n", l->x1, l->y1, l->x2, l->y2);
        }
        insertRadialT(*All, I->Nox + dx, I->Noy + dy, I->NoInfo);
        ((Figura *)I->NoInfo)->RefCount++;
        removeNoRadialT(All, getNodeRadialT(*All, I->Nox, I->Noy, EPSILON_PADRAO));
    }
    else
    {
        printf("Colisão de Nó evitada em: mv %d %lf %lf\n", ID, dx, dy);
        fprintf(log, "Colisão de Nó evitada\n");
    }
    free(I);
}

void Praga(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree *All)
{
    Lista Atingido = createLst(-1);
    ProcAfetado *A = malloc(sizeof(ProcAfetado));
    A->Atingido = Atingido;
    A->x = x;
    A->y = y;
    A->larg = largura;
    A->alt = altura;
    visitaLarguraRadialT(*All, ObjetoAtingido, A);
    free(A);

    /* Insere na lista Afetados apenas os itens que não são entidades e foram atingidos */
    while (!isEmptyLst(Atingido))
    {
        bool IsEntity = false;
        Figura *F = popLst(Atingido);
        Iterador E = createIterador(Entidades, false);
        while (!isIteratorEmpty(Entidades, E))
        {
            Entidade *Ent = getIteratorNext(Entidades, E);
            if (F->ID == Ent->ID)
            {
                IsEntity = true;
            }
        }
        killIterator(E);
        if (!IsEntity)
        {
            F->RefCount++; // Para não ser eliminado pelo removeNoRadialT()
            Hortalica *H = malloc(sizeof(Hortalica));
            H->ID = F->ID;
            H->Fig = F;
            insertLst(Afetados, H);
        }
    }
    killLst(Atingido);
}

void DadosI(int ID, RadialTree All, FILE *log)
{
    fprintf(log, "ID: %d\n", ID);
    ProcID *I = ProcuraID(ID, All);
    Figura *F = I->NoInfo;
    char Forma = F->Tipo;
    fprintf(log, "Tipo: ");
    if (Forma == 'T')
    {
        Texto *t = F->Figura;
        fprintf(log, "Texto\n");
        fprintf(log, "X: %lf\n", t->x);
        fprintf(log, "Y: %lf\n", t->y);
        fprintf(log, "Corb: %s\n", t->corb);
        fprintf(log, "Corp: %s\n", t->corp);
        fprintf(log, "Ancora: %s\n", t->a);
        fprintf(log, "Texto: %s\n", t->txto);
    }
    else if (Forma == 'C')
    {
        Circulo *c = F->Figura;
        fprintf(log, "Circulo\n");
        fprintf(log, "X: %lf\n", c->x);
        fprintf(log, "Y: %lf\n", c->y);
        fprintf(log, "Raio: %lf\n", c->raio);
        fprintf(log, "Corb: %s\n", c->corb);
        fprintf(log, "Corp: %s\n", c->corp);
    }
    else if (Forma == 'R')
    {
        Retangulo *r = F->Figura;
        fprintf(log, "Retangulo\n");
        fprintf(log, "X: %lf\n", r->x);
        fprintf(log, "Y: %lf\n", r->y);
        fprintf(log, "Largura: %lf\n", r->larg);
        fprintf(log, "Altura: %lf\n", r->alt);
        fprintf(log, "Corb: %s\n", r->corb);
        fprintf(log, "Corp: %s\n", r->corp);
    }
    else if (Forma == 'L')
    {
        Linha *l = F->Figura;
        fprintf(log, "Linha\n");
        fprintf(log, "X1: %lf\n", l->x1);
        fprintf(log, "Y1: %lf\n", l->y1);
        fprintf(log, "X2: %lf\n", l->x2);
        fprintf(log, "Y2: %lf\n", l->y2);
        fprintf(log, "Cor: %s\n", l->cor);
    }
    free(I);
}

void InfoColheitadeiras(Lista Entidades, FILE *log, RadialTree All)
{
    /* Procura todas as Colheitadeiras*/
    fprintf(log, "Colheitadeiras:\n");
    Iterador E = createIterador(Entidades, false);
    while (!isIteratorEmpty(Entidades, E))
    {
        Entidade *C = getIteratorNext(Entidades, E);
        if (C->IsColheita)
        {
            DadosI(C->ID, All, log);
        }
    }
    killIterator(E);
}

void fechaQry(ArqQry fqry)
{
    if (fqry != NULL)
    {
        fclose(fqry);
    }
}

void ColheElementos(RadialTree *All, Lista Entidades, Lista Colheita, double Xinicio, double Yinicio, double Xfim, double Yfim)
{
    Lista Nos = createLst(-1);
    getNodesDentroRegiaoRadialT(*All, Xinicio, Yinicio, Xfim, Yfim, Nos);

    /* Insere na lista Colheita apenas os itens que não são entidades e estão na área */
    while (!isEmptyLst(Nos))
    {
        bool IsEntity = false;
        Node N = popLst(Nos);
        Figura *F = getInfoRadialT(*All, N);
        Iterador E = createIterador(Entidades, false);
        while (!isIteratorEmpty(Entidades, E))
        {
            Entidade *Ent = getIteratorNext(Entidades, E);
            if (F->ID == Ent->ID)
            {
                IsEntity = true;
            }
        }
        killIterator(E);
        if (!IsEntity)
        {
            F->RefCount++; // Para não ser eliminado pelo removeNoRadialT()
            Hortalica *H = malloc(sizeof(Hortalica));
            H->ID = F->ID;
            H->Fig = F;
            insertLst(Colheita, H);
        }
    }
    killLst(Nos);

    /* Remove os itens que foram colhidos */
    Iterador Del = createIterador(Colheita, false);
    while (!isIteratorEmpty(Colheita, Del))
    {
        Hortalica *H = getIteratorNext(Colheita, Del);
        ProcID *I = ProcuraID(H->ID, *All);
        removeNoRadialT(All, getNodeRadialT(*All, I->Nox, I->Noy, EPSILON_PADRAO));
        free(I);
    }
    killIterator(Del);
}

void ContabilizaColheita(Lista Colheita, FILE *log)
{
    Contabiliza CONT = {0};
    Iterador Col = createIterador(Colheita, false);
    while (!isIteratorEmpty(Colheita, Col))
    {
        Hortalica *H = getIteratorNext(Colheita, Col);
        Figura *F = H->Fig;
        char Forma = F->Tipo;
        if (Forma == 'T')
        {
            /*Cebola, Morango, Cenoura ou Mato*/
            Texto *t = F->Figura;
            if (strncmp(t->txto, "@", 1) == 0)
            {
                CONT.cebola += 200; // 200g
            }
            else if (strncmp(t->txto, "*", 1) == 0)
            {
                CONT.morango += 20; // 20g
            }
            else if (strncmp(t->txto, "%", 1) == 0)
            {
                CONT.cenoura += 70; // 70g
            }
            else
            {
                CONT.mato_texto += 15; // 15g
            }
        }
        else if (Forma == 'C')
        {
            /*Abóbora*/
            CONT.abobora += 2000; // 2kg
        }
        else if (Forma == 'R')
        {
            /*Repolho*/
            CONT.repolho += 1000; // 1kg
        }
        else if (Forma == 'L')
        {
            /*Mato*/
            Linha *l = F->Figura;
            CONT.mato_linha += Distancia2Pontos(l->x1, l->y1, l->x2, l->y2) * 10; // 10g * Comprimento
        }
    }
    killIterator(Col);
    fprintf(log, "Contabilidade da Colheita\n");
    fprintf(log, "Abóbora: %.0lfg ou %.2lfkg\n", CONT.abobora, CONT.abobora / 1000);
    fprintf(log, "Morango: %.0lfg ou %.2lfkg\n", CONT.morango, CONT.morango / 1000);
    fprintf(log, "Repolho: %.0lfg ou %.2lfkg\n", CONT.repolho, CONT.repolho / 1000);
    fprintf(log, "Cebola: %.0lfg ou %.2lfkg\n", CONT.cebola, CONT.cebola / 1000);
    fprintf(log, "Cenoura: %.0lfg ou %.2lfkg\n", CONT.cenoura, CONT.cenoura / 1000);
    fprintf(log, "Mato(linha): %.0lfg ou %.2lfkg\n", CONT.mato_linha, CONT.mato_linha / 1000);
    fprintf(log, "Mato(texto): %.0lfg ou %.2lfkg\n", CONT.mato_texto, CONT.mato_texto / 1000);
}

int GetIDUnico(Lista Entidades, int ID)
{
    int IDunico = ID;
    bool JaExiste = true;
    Iterador E = createIterador(Entidades, false);
    while (JaExiste)
    {
        JaExiste = false;
        while (!isIteratorEmpty(Entidades, E))
        {
            Entidade *Ent = getIteratorNext(Entidades, E);
            if (Ent->ID == IDunico)
            {
                IDunico++;
                JaExiste = true;
            }
        }
    }
    killIterator(E);
    return IDunico;
}

void CriaArea(RadialTree All, Lista Entidades, double Xinicio, double Yinicio, double Xfim, double Yfim)
{
    Retangulo *r = malloc(sizeof(Retangulo));
    r->x = Xinicio;
    r->y = Yinicio;
    r->larg = Xfim - Xinicio;
    r->alt = Yfim - Yinicio;
    strcpy(r->corp, "#ffffff00"); // Branco Transparente via canal alpha 00
    strcpy(r->corb, "#ff0000");   // Vermelho
    r->pont = 3;
    r->ID = GetIDUnico(Entidades, 9999);
    Figura *f = malloc(sizeof(Figura));
    f->ID = r->ID;
    f->Tipo = 'R';
    f->Figura = r;
    Entidade *e = malloc(sizeof(Entidade));
    e->ID = r->ID;
    e->Fig = f;
    e->Nox = r->x;
    e->Noy = r->y;
    e->IsColheita = false;
    insertRadialT(All, r->x, r->y, f);
    insertLst(Entidades, e);
    f->RefCount = 2; // 2 pois foi inserido tanto na lista de entidades quanto na árvore
}

void FreeEntidade(void *Ent)
{
    Entidade *E = Ent;
    FreeFigura(E->Fig);
    free(E);
}

void FreeHortalica(void *Hor)
{
    Hortalica *H = Hor;
    FreeFigura(H->Fig);
    free(H);
}

void VerificaID(Info i, double x, double y, void *aux)
{
    Figura *F = i;
    if (F->ID == ((ProcID *)aux)->ID)
    {
        ((ProcID *)aux)->NoInfo = i;
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

void ObjetoAtingido(Info i, double x, double y, void *aux)
{
    ProcAfetado *A = aux;
    Lista Atingido = A->Atingido;
    if (VerificaAtingido(i, aux))
    {
        insertLst(Atingido, i);
    }
}

bool VerificaAtingido(Info i, void *aux)
{
    ProcAfetado *Atinge = aux;
    Figura *F = i;
    if (F->Tipo == 'T')
    {
        Texto *t = F->Figura;
        return VerificaPonto(Atinge->x, t->x, Atinge->x + Atinge->larg, Atinge->y, t->y, Atinge->y + Atinge->alt);
    }
    else if (F->Tipo == 'C')
    {
        return VerificaCirculoAtingido(aux, F->Figura);
    }
    else if (F->Tipo == 'R')
    {
        return VerificaRetanguloAtingido(aux, F->Figura);
    }
    else if (F->Tipo == 'L')
    {
        Linha *l = F->Figura;
        return VerificaPonto(Atinge->x, l->x1, Atinge->x + Atinge->larg, Atinge->y, l->y1, Atinge->y + Atinge->alt) ||
               VerificaPonto(Atinge->x, l->x2, Atinge->x + Atinge->larg, Atinge->y, l->y2, Atinge->y + Atinge->alt);
    }
    else
    {
        printf("Erro ao verificar forma da figura atingida!\n");
        return false;
    }
}

bool VerificaRetanguloAtingido(void *aux, void *Ret)
{
    Retangulo *r = Ret;
    ProcAfetado *Atinge = aux;
    /* Verifica se o retângulo está completamente dentro da área atingida */
    if (VerificaIntervalo(Atinge->x, r->x, Atinge->x + Atinge->larg) &&
        VerificaIntervalo(Atinge->y, r->y, Atinge->y + Atinge->alt))
    {
        return true; // O retângulo está totalmente dentro da área atingida
    }

    /* Verifica se há interseção entre a área atingida e o retângulo */
    if (VerificaIntervalo(r->x, Atinge->x, Atinge->x + Atinge->larg) &&
        VerificaIntervalo(r->y, Atinge->y, Atinge->y + Atinge->alt))
    {
        return true; // O retângulo tem interseção com a área atingida
    }

    return false; // O retângulo não foi atingido
}

bool VerificaCirculoAtingido(void *aux, void *Circ)
{
    Circulo *c = Circ;
    ProcAfetado *Atinge = aux;
    /* Verifica se o círculo está completamente dentro da área atingida */
    if (VerificaIntervalo(Atinge->x, c->x - c->raio, Atinge->x + Atinge->larg) &&
        VerificaIntervalo(Atinge->y, c->y - c->raio, Atinge->y + Atinge->alt))
    {
        return true; // O círculo está totalmente dentro da área atingida
    }

    /* Verifica se há interseção entre a área atingida e o círculo */
    if (VerificaPonto(Atinge->x, c->x, Atinge->x + Atinge->larg, Atinge->y, c->y, Atinge->y + Atinge->alt) ||
        (VerificaIntervalo(c->x, Atinge->x, Atinge->x + Atinge->larg) &&
         VerificaIntervalo(c->y, Atinge->y, Atinge->y + Atinge->alt)))
    {
        return true; // O círculo tem interseção com a área atingida
    }

    return false; // O círculo não foi atingido
}

double calcularAreaIntersecaoRetangulo(const struct StRetangulo *retangulo1, const struct StRetangulo *retangulo2)
{
    double intersecaoX = fmax(retangulo1->x, retangulo2->x);
    double intersecaoY = fmax(retangulo1->y, retangulo2->y);
    double intersecaoW = fmin(retangulo1->x + retangulo1->larg, retangulo2->x + retangulo2->larg) - intersecaoX;
    double intersecaoH = fmin(retangulo1->y + retangulo1->alt, retangulo2->y + retangulo2->alt) - intersecaoY;

    if (intersecaoW <= 0 || intersecaoH <= 0)
    {
        return 0.0; // Não há sobreposição, área de interseção é zero
    }

    return intersecaoW * intersecaoH;
}

// PROTOTIPO

double calcularAreaIntersecaoCirculoRetangulo(const struct StCirculo *circulo, const struct StRetangulo *retangulo)
{
    double circuloX = circulo->x;
    double circuloY = circulo->y;
    double retanguloX = retangulo->x;
    double retanguloY = retangulo->y;

    if (circuloX < retanguloX)
    {
        circuloX = retanguloX;
    }
    else if (circuloX > retanguloX + retangulo->larg)
    {
        circuloX = retanguloX + retangulo->larg;
    }

    if (circuloY < retanguloY)
    {
        circuloY = retanguloY;
    }
    else if (circuloY > retanguloY + retangulo->alt)
    {
        circuloY = retanguloY + retangulo->alt;
    }

    double distanciaX = circuloX - circulo->x;
    double distanciaY = circuloY - circulo->y;
    double distancia = sqrt(distanciaX * distanciaX + distanciaY * distanciaY);

    if (distancia > circulo->raio)
    {
        return 0.0; // Não há sobreposição, área de interseção é zero
    }

    double theta = acos(distancia / circulo->raio);
    double setorCircular = (0.5 * circulo->raio * circulo->raio * theta) - (0.5 * circulo->raio * circulo->raio * sin(theta));
    double retanguloIntersecao = distanciaX * distanciaY;

    return setorCircular + retanguloIntersecao;
}

double calcularAreaAtingidaPraga(const struct StRetangulo *praga, const struct StRetangulo *objeto)
{
    double areaIntersecao = calcularAreaIntersecaoRetangulo(praga, objeto);
    double areaObjeto = objeto->larg * objeto->alt;

    return areaIntersecao / areaObjeto * 100.0;
}

double calcularAreaAtingidaPragaCirculo(const struct StRetangulo *praga, const struct StCirculo *circulo)
{
    double areaIntersecao = calcularAreaIntersecaoCirculoRetangulo(circulo, praga);
    double areaCirculo = PI * circulo->raio * circulo->raio;

    return areaIntersecao / areaCirculo * 100.0;
}