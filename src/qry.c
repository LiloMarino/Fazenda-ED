#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Bibliotecas/geradores.h"
#include "Bibliotecas/learquivo.h"
#include "Bibliotecas/utilities.h"
#include "qry.h"
#include "geo.h"
#include "def.h"
#include "svg.h"

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

struct StTexto
{
    int ID;
    double x, y;
    char corb[TAM_VETOR], corp[TAM_VETOR], txto[TAM_VETOR], fFamily[TAM_VETOR], fWeight[TAM_VETOR], fSize[TAM_VETOR];
    char a[TAM_VETOR];
    char rotacao[TAM_VETOR];
};

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
    double Dano;
    double Prod;
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

struct StProcColhido
{
    Lista Atingido;         // Lista que conterá as informações dos nós colhidos
    double x, y, larg, alt; // Especificações da área de colheita
};

typedef struct StFigura Figura;
typedef struct StCirculo Circulo;
typedef struct StRetangulo Retangulo;
typedef struct StLinha Linha;
typedef struct StTexto Texto;
typedef struct StProcID ProcID;
typedef struct StEntidade Entidade;
typedef struct StHortalica Hortalica;
typedef struct StContabiliza Contabiliza;
typedef struct StProcColhido ProcColhido;

/*========================================================================================================== *
 * Funções Principais                                                                                        *
 *========================================================================================================== */

ArqQry abreLeituraQry(char *fn)
{
    ArqQry fqry;
    fqry = fopen(fn, "r");
    return fqry;
}

void InterpretaQry(ArqQry fqry, RadialTree *All, FILE *log)
{
    char comando[4];
    char *linha = NULL;
    char nome[40]; // Remover depois
    int num = 0;   // Remover depois
    Lista Entidades = createLst(-1);
    Lista Colheita = createLst(-1);
    Lista Afetados = createLst(-1);
    InicializaRand();
    while (leLinha(fqry, &linha))
    {
        sscanf(linha, "%s ", comando);
        if (strcmp(comando, "cl") == 0)
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
            ((Figura *)I->NoInfo)->RefCount++; // Pois foi inserido na lista Entidades
            free(I);
        }
        else if (strcmp(comando, "hvt") == 0)
        {
            int ID, Passos;
            char Direcao;
            sscanf(linha, "%s %d %d %c", comando, &ID, &Passos, &Direcao);
            fprintf(log, "\n[*] %s %d %d %c\n", comando, ID, Passos, Direcao);
            Harvest(ID, Passos, Direcao, log, Entidades, All, Colheita, Afetados);
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
            Praga(x, y, larg, alt, raio, Afetados, Entidades, All, log);
        }
        else if (strcmp(comando, "cr") == 0)
        {
            double x, y, larg, alt, raio;
            sscanf(linha, "%s %lf %lf %lf %lf %lf", comando, &x, &y, &larg, &alt, &raio);
            fprintf(log, "\n[*] %s %lf %lf %lf %lf %lf\n", comando, x, y, larg, alt, raio);
            Cura(x, y, larg, alt, raio, Afetados, Entidades, *All, log);
        }
        else if (strcmp(comando, "ad") == 0)
        {
            double x, y, larg, alt, raio;
            sscanf(linha, "%s %lf %lf %lf %lf %lf", comando, &x, &y, &larg, &alt, &raio);
            fprintf(log, "\n[*] %s %lf %lf %lf %lf %lf\n", comando, x, y, larg, alt, raio);
            Aduba(x, y, larg, alt, raio, Afetados, Entidades, *All, log);
        }
        else if (strcmp(comando, "st") == 0)
        {
            int fator, j;
            double x, y, larg, alt, dx, dy;
            sscanf(linha, "%s %lf %lf %lf %lf %d %lf %lf %d", comando, &x, &y, &larg, &alt, &fator, &dx, &dy, &j);
            fprintf(log, "\n[*] %s %lf %lf %lf %lf %d %lf %lf %d\n", comando, x, y, larg, alt, fator, dx, dy, j);
            Semeia(x, y, larg, alt, fator, dx, dy, j, Entidades, *All, log);
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
    fprintf(log, "\n[*] Terminada Execução do QRY\n");
    fprintf(log, "\nElementos Colhidos:\n");
    ContabilizaColheita(Colheita, log);
    fprintf(log, "\nElementos Não Colhidos:\n");
    ColheElementos(All, Entidades, Afetados, Colheita, log, SIZE_X1_Y1, SIZE_X1_Y1, SIZE_X2_Y2, SIZE_X2_Y2, false);
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

void Harvest(int ID, int Passos, char Direcao, FILE *log, Lista Entidades, RadialTree *All, Lista Colheita, Lista Afetados)
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
    else
    {
        printf("Direção Inválida!\n");
        return;
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
    ColheElementos(All, Entidades, Afetados, Colheita, log, Xinicio, Yinicio, Xfim, Yfim, true);

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
        ((Figura *)I->NoInfo)->RefCount++; // Pois foi inserido novamente na árvore
        removeNoRadialT(All, getNodeRadialT(*All, I->Nox, I->Noy, EPSILON_PADRAO));
    }
    else
    {
        printf("Colisão de Nó evitada em: mv %d %lf %lf\n", ID, dx, dy);
        fprintf(log, "Colisão de Nó evitada\n");
    }
    free(I);
    fflush(log);
}

void Praga(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree *All, FILE *log)
{
    Lista Atingido = createLst(-1);

    getNodesDentroRegiaoRadialT(*All, x, y, x + largura, y + altura, Atingido);
    Atingido = TransformaLista(*All, Atingido);

    /*Insere na lista NotEntity apenas as Hortaliças, ou seja remove as entidades*/
    Lista NotEntity = filter(Atingido, FiltraEntidades, Entidades);

    /*Insere na lista NotAtingidoBefore apenas as Hortaliças que nunca foram atingidas */
    Lista NotAtingidoBefore = filter(NotEntity, FiltraAtingidos, Afetados);

    /*Insere na lista AtingidoBefore as Hortaliças que já foram atingidas alguma vez, caso não tenha sido atingida é atribuído NULL*/
    Lista AtingidoBefore = map(NotEntity, TransformaAtingidos, Afetados);

    int numLinhas, numColunas;
    void *MatrizGoticulas = CriaMatrizDeGoticulas(x, y, largura, altura, raio, &numLinhas, &numColunas);

    while (!isEmptyLst(NotAtingidoBefore))
    {
        /*A hortaliça não havia sido afetada ainda*/
        Figura *F = popLst(NotAtingidoBefore);
        Hortalica *H = calloc(1, sizeof(Hortalica));
        H->ID = F->ID;
        H->Fig = F;
        double AreaAfetada = CalculaAreaAfetada(H->Fig, MatrizGoticulas, numLinhas, numColunas);
        H->Dano += AreaAfetada;
        ReportaHortalica(*All, log, H);
        if (H->Dano > 0.75)
        {
            fprintf(log, "Eliminada!\n");
            ReplaceWithRedX(All, Entidades, Afetados, H);
        }
        else
        {
            insertLst(Afetados, H);
            F->RefCount++; // Pois foi inserido na lista Afetados
        }
        fprintf(log, "\n");
    }

    while (!isEmptyLst(AtingidoBefore))
    {
        Hortalica *Hor = popLst(AtingidoBefore);
        if (Hor != NULL)
        {
            /*A hortaliça já foi afetada outra vez e está presente na lista Afetados*/
            double AreaAfetada = CalculaAreaAfetada(Hor->Fig, MatrizGoticulas, numLinhas, numColunas);
            Hor->Dano += AreaAfetada;
            ReportaHortalica(*All, log, Hor);
            if (Hor->Dano > 0.75)
            {
                fprintf(log, "Eliminada!\n");
                ReplaceWithRedX(All, Entidades, Afetados, Hor);
            }
            fprintf(log, "\n");
        }
    }

    FreeMatrizDeGoticulas(MatrizGoticulas, numLinhas);
    killLst(Atingido);
    killLst(NotEntity);
    killLst(NotAtingidoBefore);
    killLst(AtingidoBefore);

    /*Marca a área afetada para o svg e marca o círculo vermelho em (x,y)*/
    CriaArea(*All, Entidades, x, y, x + largura, y + altura);
    CriaMarcacaoCircular(*All, Entidades, x, y, raio, "red", "#ffffff00");
}

void Cura(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree All, FILE *log)
{
    Lista Atingido = createLst(-1);

    getNodesDentroRegiaoRadialT(All, x, y, x + largura, y + altura, Atingido);
    Atingido = TransformaLista(All, Atingido);

    /*Insere na lista NotEntity apenas as Hortaliças, ou seja remove as entidades*/
    Lista NotEntity = filter(Atingido, FiltraEntidades, Entidades);

    /*Insere na lista AtingidoBefore as Hortaliças que já foram atingidas alguma vez, caso não tenha sido atingida é atribuído NULL*/
    Lista AtingidoBefore = map(NotEntity, TransformaAtingidos, Afetados);

    int numLinhas, numColunas;
    void *MatrizGoticulas = CriaMatrizDeGoticulas(x, y, largura, altura, raio, &numLinhas, &numColunas);

    while (!isEmptyLst(AtingidoBefore))
    {
        Hortalica *Hor = popLst(AtingidoBefore);
        if (Hor != NULL)
        {
            /*A hortaliça já foi afetada outra vez e está presente na lista Afetados*/
            ReportaHortalica(All, log, Hor);
            if (Hor->Dano > 0)
            {
                double AreaAfetada = CalculaAreaAfetada(Hor->Fig, MatrizGoticulas, numLinhas, numColunas);
                Hor->Dano -= AreaAfetada;
                if (Hor->Dano < 0)
                {
                    Hor->Dano = 0;
                }
            }
            fprintf(log, "\n");
        }
    }

    FreeMatrizDeGoticulas(MatrizGoticulas, numLinhas);
    killLst(Atingido);
    killLst(NotEntity);
    killLst(AtingidoBefore);

    /*Marca a área afetada para o svg e marca o círculo amarelo em (x,y)*/
    CriaArea(All, Entidades, x, y, x + largura, y + altura);
    CriaMarcacaoCircular(All, Entidades, x, y, raio, "yellow", "#ffffff00");
}

void Aduba(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree All, FILE *log)
{
    Lista Atingido = createLst(-1);

    getNodesDentroRegiaoRadialT(All, x, y, x + largura, y + altura, Atingido);
    Atingido = TransformaLista(All, Atingido);

    /*Insere na lista NotEntity apenas as Hortaliças, ou seja remove as entidades*/
    Lista NotEntity = filter(Atingido, FiltraEntidades, Entidades);

    /*Insere na lista NotAtingidoBefore apenas as Hortaliças que nunca foram atingidas */
    Lista NotAtingidoBefore = filter(NotEntity, FiltraAtingidos, Afetados);

    /*Insere na lista AtingidoBefore as Hortaliças que já foram atingidas alguma vez, caso não tenha sido atingida é atribuído NULL*/
    Lista AtingidoBefore = map(NotEntity, TransformaAtingidos, Afetados);

    int numLinhas, numColunas;
    void *MatrizGoticulas = CriaMatrizDeGoticulas(x, y, largura, altura, raio, &numLinhas, &numColunas);

    while (!isEmptyLst(NotAtingidoBefore))
    {
        /*A hortaliça não havia sido afetada ainda*/
        Figura *F = popLst(NotAtingidoBefore);
        Hortalica *H = calloc(1, sizeof(Hortalica));
        H->ID = F->ID;
        H->Fig = F;
        H->Prod = CalculaAreaAfetada(H->Fig, MatrizGoticulas, numLinhas, numColunas);
        ReportaHortalica(All, log, H);
        insertLst(Afetados, H);
        F->RefCount++; // Pois foi inserido na lista Afetados
        fprintf(log, "\n");
    }

    while (!isEmptyLst(AtingidoBefore))
    {
        Hortalica *Hor = popLst(AtingidoBefore);
        if (Hor != NULL)
        {
            /*A hortaliça já foi afetada outra vez e está presente na lista Afetados*/
            Hor->Prod += CalculaAreaAfetada(Hor->Fig, MatrizGoticulas, numLinhas, numColunas);
            ReportaHortalica(All, log, Hor);
            fprintf(log, "\n");
        }
    }

    FreeMatrizDeGoticulas(MatrizGoticulas, numLinhas);
    killLst(Atingido);
    killLst(NotEntity);
    killLst(NotAtingidoBefore);
    killLst(AtingidoBefore);

    /*Marca a área afetada para o svg e marca o círculo verde em (x,y)*/
    CriaArea(All, Entidades, x, y, x + largura, y + altura);
    CriaMarcacaoCircular(All, Entidades, x, y, raio, "green", "#ffffff00");
}

void Semeia(double x, double y, double largura, double altura, int fator, double dx, double dy, int j, Lista Entidades, RadialTree All, FILE *log)
{
    /* "Copia" os nós dentro da área */
    Lista Nos = createLst(-1);
    getNodesDentroRegiaoRadialT(All, x, y, x + largura, y + altura, Nos);

    /* "Cola" os nós na área movida por dx e dy */
    Paste(j, dx, dy, fator, All, Nos, Entidades, log);

    /*Marca a área copiada para o svg e marca o círculo vermelho em (x,y)*/
    CriaArea(All, Entidades, x, y, x + largura, y + altura);
    CriaMarcacaoCircular(All, Entidades, x, y, RAIO_BASE, "#ffffff00", "red");

    /*Marca a área colada para o svg e marca o círculo vermelho em (x,y)*/
    CriaArea(All, Entidades, dx + x, dy + y, dx + x + largura, dy + y + altura);
    CriaMarcacaoCircular(All, Entidades, dx + x, dy + y, RAIO_BASE, "#ffffff00", "red");
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
    fflush(log);
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

/*========================================================================================================== *
 * Funções Auxiliares                                                                                        *
 *========================================================================================================== */

void ColheElementos(RadialTree *All, Lista Entidades, Lista Afetados, Lista Colheita, FILE *log, double Xinicio, double Yinicio, double Xfim, double Yfim, bool parcial)
{
    Lista Atingido = createLst(-1);
    Lista Conta = createLst(-1);

    ProcColhido *Area = malloc(sizeof(ProcColhido));
    Area->Atingido = Atingido;
    Area->x = Xinicio;
    Area->y = Yinicio;
    Area->larg = Xfim - Xinicio;
    Area->alt = Yfim - Yinicio;
    visitaLarguraRadialT(*All, ObjetoTotalAtingido, Area);

    /*Insere na lista NotEntity apenas as Hortaliças, ou seja remove as entidades*/
    Lista NotEntity = filter(Atingido, FiltraEntidades, Entidades);

    /*Insere na lista NotAtingidoBefore apenas as Hortaliças que nunca foram atingidas */
    Lista NotAtingidoBefore = filter(NotEntity, FiltraAtingidos, Afetados);

    /*Insere na lista AtingidoBefore as Hortaliças que já foram atingidas alguma vez, caso não tenha sido atingida é atribuído NULL*/
    Lista AtingidoBefore = map(NotEntity, TransformaAtingidos, Afetados);

    while (!isEmptyLst(NotAtingidoBefore))
    {
        /*A hortaliça nunca foi atingida e não é uma entidade*/
        Figura *F = popLst(NotAtingidoBefore);
        Hortalica *H = calloc(1, sizeof(Hortalica));
        H->ID = F->ID;
        H->Fig = F;
        insertLst(Colheita, H);
        F->RefCount++; // Pois foi inserido na lista Colheita
        insertLst(Conta, H);
        if (parcial)
        {
            ReportaHortalica(*All, log, H);
            fprintf(log, "\n");
        }
    }

    while (!isEmptyLst(AtingidoBefore))
    {
        Hortalica *Hor = popLst(AtingidoBefore);
        if (Hor != NULL)
        {
            /*A hortaliça já foi atingida e não é uma entidade*/
            Posic Del = getFirstLst(Afetados);
            while (Del != NULL)
            {
                Hortalica *Afe = getLst(Del);
                if (Afe->ID == Hor->ID)
                {
                    removeLst(Afetados, Del);
                    Figura *F = Hor->Fig;
                    F->RefCount--; // Pois foi removido da lista Afetados
                    insertLst(Colheita, Hor);
                    F->RefCount++; // Pois foi inserido na lista Colheita
                    insertLst(Conta, Hor);
                    break;
                }
                Del = getNextLst(Afetados, Del);
            }
            if (parcial)
            {
                ReportaHortalica(*All, log, Hor);
                fprintf(log, "\n");
            }
        }
    }

    /* Contabiliza a colheita e reporta */
    if (parcial)
    {
        fprintf(log, "Contabilidade Parcial da Colheita\n");
    }
    ContabilizaColheita(Conta, log);

    killLst(Atingido);
    killLst(NotEntity);
    killLst(NotAtingidoBefore);
    killLst(AtingidoBefore);
    free(Area);

    /* Remove os itens que foram colhidos da árvore */
    if (parcial)
    {
    Iterador Del = createIterador(Conta, false);
    while (!isIteratorEmpty(Conta, Del))
    {
        Hortalica *H = getIteratorNext(Conta, Del);
        ProcID *I = ProcuraID(H->ID, *All);
        removeNoRadialT(All, getNodeRadialT(*All, I->Nox, I->Noy, EPSILON_PADRAO));
        free(I);
    }
    killIterator(Del);
    }
    killLst(Conta);
}

void ContabilizaColheita(Lista Colheita, FILE *log)
{
    Contabiliza CONT = {0};
    Iterador Col = createIterador(Colheita, false);
    while (!isIteratorEmpty(Colheita, Col))
    {
        Hortalica *H = getIteratorNext(Colheita, Col);
        Figura *F = H->Fig;
        double Modificador = 1;
        Modificador *= 1 + (int) H->Prod / 10.0;
        Modificador *= 1 - H->Dano;
        char Forma = F->Tipo;
        if (Forma == 'T')
        {
            /*Cebola, Morango, Cenoura ou Mato*/
            Texto *t = F->Figura;
            if (strncmp(t->txto, "@", 1) == 0)
            {
                CONT.cebola += 200 * Modificador; // 200g
            }
            else if (strncmp(t->txto, "*", 1) == 0)
            {
                CONT.morango += 20 * Modificador; // 20g
            }
            else if (strncmp(t->txto, "%", 1) == 0)
            {
                CONT.cenoura += 70 * Modificador; // 70g
            }
            else
            {
                CONT.mato_texto += 15 * Modificador; // 15g
            }
        }
        else if (Forma == 'C')
        {
            /*Abóbora*/
            CONT.abobora += 2000 * Modificador; // 2kg
        }
        else if (Forma == 'R')
        {
            /*Repolho*/
            CONT.repolho += 1000 * Modificador; // 1kg
        }
        else if (Forma == 'L')
        {
            /*Mato*/
            Linha *l = F->Figura;
            CONT.mato_linha += Distancia2Pontos(l->x1, l->y1, l->x2, l->y2) * 10 * Modificador; // 10g * Comprimento
        }
    }
    killIterator(Col);
    fprintf(log, "Abóbora: %.0lfg ou %.2lfkg\n", CONT.abobora, CONT.abobora / 1000);
    fprintf(log, "Morango: %.0lfg ou %.2lfkg\n", CONT.morango, CONT.morango / 1000);
    fprintf(log, "Repolho: %.0lfg ou %.2lfkg\n", CONT.repolho, CONT.repolho / 1000);
    fprintf(log, "Cebola: %.0lfg ou %.2lfkg\n", CONT.cebola, CONT.cebola / 1000);
    fprintf(log, "Cenoura: %.0lfg ou %.2lfkg\n", CONT.cenoura, CONT.cenoura / 1000);
    fprintf(log, "Mato(linha): %.0lfg ou %.2lfkg\n", CONT.mato_linha, CONT.mato_linha / 1000);
    fprintf(log, "Mato(texto): %.0lfg ou %.2lfkg\n\n", CONT.mato_texto, CONT.mato_texto / 1000);
    fflush(log);
}

void Paste(int j, double dx, double dy, int proporcao, RadialTree All, Lista Nos, Lista Entidades, FILE *log)
{
    Lista TempEnt = createLst(-1);
    /* Filtra a Lista dos Nós copiando apenas os nós que não são entidades*/
    fprintf(log, "Figuras:\n\n");
    while (!isEmptyLst(Nos))
    {
        bool IsEntity = false;
        Node N = popLst(Nos);
        Figura *F = getInfoRadialT(All, N);
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
            DadosI(F->ID, All, log);
            Copy(F, j, dx, dy, proporcao, TempEnt);
        }
    }
    killLst(Nos);

    /*Coloca na árvore as figuras copiadas*/
    fprintf(log, "Figuras Clonadas:\n\n");
    while (!isEmptyLst(TempEnt))
    {
        Entidade *Ent = popLst(TempEnt);
        Figura *F = Ent->Fig;
        insertRadialT(All, Ent->Nox, Ent->Noy, Ent->Fig);
        DadosI(F->ID, All, log);
        F->RefCount++; // Pois foi inserida na árvore
        free(Ent);
    }
    killLst(TempEnt);
}

void Copy(void *Fig, int j, double dx, double dy, int proporcao, Lista TempEnt)
{
    Figura *F = (Figura *)Fig;
    for (int i = 0; i < proporcao; i++)
    {
        Entidade *Ent = malloc(sizeof(Entidade));
        Figura *F2 = malloc(sizeof(Figura));
        F2->ID = GetIDUnico(TempEnt, j);
        F2->RefCount = 0; // Pois não foi inserido em nada ainda
        F2->Tipo = F->Tipo;
        if (F->Tipo == 'T')
        {
            Texto *t = F->Figura;
            Texto *t2 = malloc(sizeof(Texto));
            strcpy(t2->a, t->a);
            strcpy(t2->corb, t->corb);
            strcpy(t2->corp, t->corp);
            strcpy(t2->fFamily, t->fFamily);
            strcpy(t2->fWeight, t->fWeight);
            strcpy(t2->fSize, t->fSize);
            strcpy(t2->rotacao, t->rotacao);
            strcpy(t2->txto, t->txto);
            t2->x = t->x + dx;
            t2->y = t->y + dy;
            t2->ID = F2->ID;
            F2->Figura = t2;
            Ent->Nox = t2->x;
            Ent->Noy = t2->y;
            if (i > 0)
            {
                Ent->Nox += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                Ent->Noy += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                t2->x = Ent->Nox;
                t2->y = Ent->Noy;
            }
        }
        else if (F->Tipo == 'C')
        {
            Circulo *c = F->Figura;
            Circulo *c2 = malloc(sizeof(Circulo));
            strcpy(c2->corb, c->corb);
            strcpy(c2->corp, c->corp);
            c2->raio = c->raio;
            c2->ID = F2->ID;
            c2->x = c->x + dx;
            c2->y = c->y + dy;
            F2->Figura = c2;
            Ent->Nox = c2->x;
            Ent->Noy = c2->y;
            if (i > 0)
            {
                Ent->Nox += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                Ent->Noy += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                c2->x = Ent->Nox;
                c2->y = Ent->Noy;
            }
        }
        else if (F->Tipo == 'R')
        {
            Retangulo *r = F->Figura;
            Retangulo *r2 = malloc(sizeof(Retangulo));
            strcpy(r2->corb, r->corb);
            strcpy(r2->corp, r->corp);
            r2->pont = r->pont;
            r2->alt = r->alt;
            r2->larg = r->larg;
            r2->ID = F2->ID;
            r2->x = r->x + dx;
            r2->y = r->y + dy;
            F2->Figura = r2;
            Ent->Nox = r2->x;
            Ent->Noy = r2->y;
            if (i > 0)
            {
                Ent->Nox += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                Ent->Noy += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                r2->x = Ent->Nox;
                r2->y = Ent->Noy;
            }
        }
        else if (F->Tipo == 'L')
        {
            Linha *l = F->Figura;
            Linha *l2 = malloc(sizeof(Linha));
            strcpy(l2->cor, l->cor);
            l->ID = F2->ID;
            l2->x1 = l->x1 + dx;
            l2->y1 = l->y1 + dy;
            l2->x2 = l->x2 + dx;
            l2->y2 = l->y2 + dy;
            F2->Figura = l2;
            Ent->Nox = l2->x1;
            Ent->Noy = l2->y1;
            if (i > 0)
            {
                Ent->Nox += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                Ent->Noy += GerarNumeroDouble(DISPERCAO_MIN, DISPERCAO_MAX);
                l2->x1 = Ent->Nox;
                l2->y1 = Ent->Noy;
            }
        }
        else
        {
            printf("Erro ao verificar forma da figura ao copiar!\n");
            free(F2);
            return;
        }

        /* Cria uma entidade temporária para a organização dos IDs */
        Ent->Fig = F2;
        Ent->ID = F2->ID;
        Ent->IsColheita = false;
        insertLst(TempEnt, Ent);
    }
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
    r->ID = GetIDUnico(Entidades, -1);
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

void CriaMarcacaoCircular(RadialTree All, Lista Entidades, double x, double y, double raio, char corb[], char corp[])
{
    Circulo *c = malloc(sizeof(Circulo));
    c->x = x;
    c->y = y;
    c->raio = raio;
    c->ID = GetIDUnico(Entidades, -1);
    strcpy(c->corp, corp);
    strcpy(c->corb, corb);
    Figura *f = malloc(sizeof(Figura));
    f->ID = c->ID;
    f->Tipo = 'C';
    f->Figura = c;
    Entidade *e = malloc(sizeof(Entidade));
    e->ID = c->ID;
    e->Fig = f;
    e->Nox = c->x;
    e->Noy = c->y;
    e->IsColheita = false;
    insertRadialT(All, c->x, c->y, f);
    insertLst(Entidades, e);
    f->RefCount = 2; // 2 pois foi inserido tanto na lista de entidades quanto na árvore
}

double CalculaAreaAfetada(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas)
{
    Figura *F = Fig;
    if (F->Tipo == 'T')
    {
        return VerificaGoticulaTexto(Fig, MatrizGoticulas, numLinhas, numColunas);
    }
    else if (F->Tipo == 'C')
    {
        return VerificaGoticulaCirculo(Fig, MatrizGoticulas, numLinhas, numColunas);
    }
    else if (F->Tipo == 'R')
    {
        return VerificaGoticulaRetangulo(Fig, MatrizGoticulas, numLinhas, numColunas);
    }
    else if (F->Tipo == 'L')
    {
        return VerificaGoticulaLinha(Fig, MatrizGoticulas, numLinhas, numColunas);
    }
    else
    {
        printf("Erro ao verificar forma da figura afetada!\n");
        return 0.0;
    }
}

double VerificaGoticulaTexto(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas)
{
    Circulo **G = MatrizGoticulas;
    Figura *F = Fig;

    for (int i = 0; i < numLinhas; i++)
    {
        for (int j = 0; j < numColunas; j++)
        {
            if (TextoContidoNaGoticula(F->Figura, &G[i][j]))
            {
                return 0.1; // Proporção fixa em 10%
            }
        }
    }

    return 0.0; // Nenhum círculo contém o texto
}

double VerificaGoticulaCirculo(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas)
{
    Circulo **G = MatrizGoticulas;
    Figura *F = Fig;
    Circulo *c = F->Figura;

    double AreaAfetada = 0;
    double AreaCirculo = PI * c->raio * c->raio;
    for (int i = 0; i < numLinhas; i++)
    {
        for (int j = 0; j < numColunas; j++)
        {
            if (GoticulaContidaNoCirculo(&G[i][j], F->Figura))
            {
                double AreaGoticula = PI * G[i][j].raio * G[i][j].raio;
                AreaAfetada += AreaGoticula / AreaCirculo;
            }
        }
    }
    return AreaAfetada;
}

double VerificaGoticulaRetangulo(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas)
{
    Circulo **G = MatrizGoticulas;
    Figura *F = Fig;
    Retangulo *r = F->Figura;

    double AreaAfetada = 0;
    double AreaRetangulo = r->larg * r->alt;
    for (int i = 0; i < numLinhas; i++)
    {
        for (int j = 0; j < numColunas; j++)
        {
            if (GoticulaContidaNoRetangulo(&G[i][j], F->Figura))
            {
                double AreaGoticula = PI * G[i][j].raio * G[i][j].raio;
                AreaAfetada += AreaGoticula / AreaRetangulo;
            }
        }
    }
    return AreaAfetada;
}

double VerificaGoticulaLinha(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas)
{
    Circulo **G = MatrizGoticulas;
    Figura *F = Fig;

    for (int i = 0; i < numLinhas; i++)
    {
        for (int j = 0; j < numColunas; j++)
        {
            if (LinhaContidaNaGoticula(F->Figura, &G[i][j]))
            {
                return 0.1; // Proporção fixa em 10%
            }
        }
    }

    return 0.0; // Nenhum círculo contém a linha
}

void *CriaMatrizDeGoticulas(double x, double y, double larg, double alt, double r, int *numLinhas, int *numColunas)
{
    *numLinhas = round(alt / (2 * r));
    *numColunas = round(larg / (2 * r));

    Circulo **goticulas = calloc(*numLinhas, sizeof(Circulo *)); // Vetor de ponteiros "Linhas"
    for (int i = 0; i < *numLinhas; i++)
    {
        goticulas[i] = calloc(*numColunas, sizeof(Circulo)); // Colunas ou Vetor da linha

        for (int j = 0; j < *numColunas; j++)
        {
            goticulas[i][j].x = x + (2 * r * i) + r; // Atribui o valor de x para o círculo atual
            goticulas[i][j].y = y + (2 * r * j) + r; // Atribui o valor de y para o círculo atual
            goticulas[i][j].raio = r;                // Atribui o valor de raio para o círculo atual
        }
    }

    return goticulas;
}

void FreeMatrizDeGoticulas(void *MatrizGoticulas, int numLinhas)
{
    Circulo **goticulas = MatrizGoticulas;
    for (int i = 0; i < numLinhas; i++)
    {
        free(goticulas[i]); // Libera o vetor de círculos da linha
    }
    free(goticulas); // Libera o vetor de ponteiros para linhas
}

bool GoticulaContidaNoRetangulo(void *Goticula, void *Ret)
{
    Retangulo *r = Ret;
    Circulo *g = Goticula;

    return (g->x - g->raio >= r->x && g->x + g->raio <= r->x + r->larg &&
            g->y - g->raio >= r->y && g->y + g->raio <= r->y + r->alt);
}

bool GoticulaContidaNoCirculo(void *Goticula, void *Circ)
{
    Circulo *c = Circ;
    Circulo *g = Goticula;
    double distanciaCentros = Distancia2Pontos(c->x, c->y, g->x, g->y);
    return (distanciaCentros + g->raio <= c->raio);
}

bool LinhaContidaNaGoticula(void *Lin, void *Goticula)
{
    Linha *l = Lin;
    Circulo *g = Goticula;
    double distanciaPonto1 = Distancia2Pontos(g->x, g->y, l->x1, l->y1);
    double distanciaPonto2 = Distancia2Pontos(g->x, g->y, l->x2, l->y2);
    return (distanciaPonto1 <= g->raio && distanciaPonto2 <= g->raio);
}

bool TextoContidoNaGoticula(void *Txto, void *Goticula)
{
    Texto *t = Txto;
    Circulo *g = Goticula;
    double distanciaPonto = Distancia2Pontos(g->x, g->y, t->x, t->y);
    return (distanciaPonto <= g->raio);
}

void ObjetoTotalAtingido(Info i, double x, double y, void *aux)
{
    ProcColhido *A = aux;
    Lista Atingido = A->Atingido;
    if (VerificaTotalAtingido(i, aux))
    {
        insertLst(Atingido, i);
    }
}

bool VerificaTotalAtingido(Info i, void *aux)
{
    ProcColhido *Atinge = aux;
    Figura *F = i;
    if (F->Tipo == 'T')
    {
        Texto *t = F->Figura;
        return VerificaPonto(Atinge->x, t->x, Atinge->x + Atinge->larg, Atinge->y + Atinge->alt, t->y, Atinge->y);
    }
    else if (F->Tipo == 'C')
    {
        Circulo *c = F->Figura;
        return (c->x - c->raio >= Atinge->x && c->x + c->raio <= Atinge->x + Atinge->larg &&
                c->y - c->raio >= Atinge->y && c->y + c->raio <= Atinge->y + Atinge->alt);
    }
    else if (F->Tipo == 'R')
    {
        Retangulo *r = F->Figura;
        return (r->x >= Atinge->x && r->x + r->larg <= Atinge->x + Atinge->larg &&
                r->y >= Atinge->y && r->y + r->alt <= Atinge->y + Atinge->alt);
    }
    else if (F->Tipo == 'L')
    {
        Linha *l = F->Figura;
        return (l->x1 >= Atinge->x && l->x1 <= Atinge->x + Atinge->larg &&
                l->x2 >= Atinge->x && l->x2 <= Atinge->x + Atinge->larg &&
                l->y1 >= Atinge->y && l->y1 <= Atinge->y + Atinge->alt &&
                l->y2 >= Atinge->y && l->y2 <= Atinge->y + Atinge->alt);
    }
    else
    {
        printf("Erro ao verificar forma do objeto!\n");
        return false;
    }
}

void ReplaceWithRedX(RadialTree *All, Lista Entidades, Lista Afetados, void *Hor)
{
    Hortalica *H = Hor;
    ProcID *P = ProcuraID(H->ID, *All);
    Posic Del = getFirstLst(Afetados);
    bool Encontrado = false;
    while (Del != NULL)
    {
        if (Del != NULL && ((Hortalica *)getLst(Del))->ID == H->ID)
        {
            Encontrado = true;
            break;
        }
        Del = getNextLst(Afetados, Del);
    }
    if (Encontrado)
    {
        removeLst(Afetados, Del);
        Figura *F = H->Fig;
        F->RefCount--; // Pois foi removido da lista Afetados
    }
    removeNoRadialT(All, getNodeRadialT(*All, P->Nox, P->Noy, EPSILON_PADRAO));
    CriaXVermelho(*All, Entidades, P->Nox, P->Noy);
    free(P);
    free(H);
}

void CriaXVermelho(RadialTree All, Lista Entidades, double x, double y)
{
    Texto *t = malloc(sizeof(Texto));
    t->x = x;
    t->y = y;
    strcpy(t->txto, "X");
    strcpy(t->a, "m");
    t->rotacao[0] = '\0';
    t->corb[0] = '\0';
    strcpy(t->corp, "red");
    t->fFamily[0] = '\0';
    strcpy(t->fWeight, "b+");
    strcpy(t->fSize, "25");
    t->ID = GetIDUnico(Entidades, -1);
    Figura *f = malloc(sizeof(Figura));
    f->ID = t->ID;
    f->Tipo = 'T';
    f->Figura = t;
    Entidade *e = malloc(sizeof(Entidade));
    e->ID = t->ID;
    e->Fig = f;
    e->Nox = x;
    e->Noy = y;
    e->IsColheita = false;
    insertRadialT(All, x, y, f);
    insertLst(Entidades, e);
    f->RefCount = 2; // 2 pois foi inserido tanto na lista de entidades quanto na árvore
}

void ReportaHortalica(RadialTree All, FILE *log, void *Hor)
{
    Hortalica *H = Hor;
    DadosI(H->ID, All, log);
    fprintf(log, "Dano: %.2lf %%\n", H->Dano * 100);
    fprintf(log, "Produtividade: %.2lf %%\n", H->Prod * 100);
    fflush(log);
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

void *ProcuraID(int ID, RadialTree All)
{
    ProcID *aux = malloc(sizeof(ProcID));
    aux->ID = ID;
    visitaLarguraRadialT(All, VerificaID, aux);
    return aux;
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
                if (IDunico < 0)
                {
                    IDunico--;
                }
                else
                {
                    IDunico++;
                }
                JaExiste = true;
                break;
            }
        }
    }
    killIterator(E);
    return IDunico;
}

bool FiltraAtingidos(Item item, void *aux)
{
    Figura *F = (Figura *)item;
    Lista Afetados = (Lista)aux;

    Iterador A = createIterador(Afetados, false);
    while (!isIteratorEmpty(Afetados, A))
    {
        Hortalica *Hor = getIteratorNext(Afetados, A);
        if (F->ID == Hor->ID)
        {
            killIterator(A);
            return false;
        }
    }
    killIterator(A);
    return true;
}

bool FiltraEntidades(Item item, void *aux)
{
    Figura *F = (Figura *)item;
    Lista Entidades = (Lista)aux;

    Iterador E = createIterador(Entidades, false);
    while (!isIteratorEmpty(Entidades, E))
    {
        Entidade *Ent = getIteratorNext(Entidades, E);
        if (F->ID == Ent->ID)
        {
            killIterator(E);
            return false;
        }
    }
    killIterator(E);
    return true;
}

Item TransformaAtingidos(Item item, void *aux)
{
    Figura *F = (Figura *)item;
    Lista Afetados = (Lista)aux;

    Iterador A = createIterador(Afetados, false);
    while (!isIteratorEmpty(Afetados, A))
    {
        Hortalica *Hor = getIteratorNext(Afetados, A);
        if (F->ID == Hor->ID)
        {
            killIterator(A);
            return Hor;
        }
    }
    killIterator(A);
    return NULL;
}

Lista TransformaLista(RadialTree All, Lista Atingido)
{
    Lista Aux = createLst(-1);

    while (!isEmptyLst(Atingido))
    {
        Figura *F = getInfoRadialT(All, popLst(Atingido));
        insertLst(Aux, F);
    }
    killLst(Atingido);

    return Aux;
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