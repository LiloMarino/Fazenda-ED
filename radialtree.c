#include "radialtree.h"
#include "Bibliotecas/utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct StNodeTree
{
    double x;
    double y;
    Info info;
    struct StNodeTree *pai;
    struct StNodeTree **filhos;
    bool removido;
};

struct StRaiz
{
    int numSetores;
    double fd;
    int numNos;
    int numNosRemovidos;
    struct StNodeTree *node;
};

typedef struct StNodeTree NodeTree;
typedef struct StRaiz Raiz;

RadialTree newRadialTree(int numSetores, double fd)
{
    Raiz *Tree = malloc(sizeof(Raiz));
    Tree->numSetores = numSetores;
    Tree->numNos = 0;
    Tree->numNosRemovidos = 0;
    Tree->fd = fd;
    Tree->node = NULL;
    return Tree;
}

Node insertRadialT(RadialTree t, double x, double y, Info i)
{
    Raiz *Tree = t;

    /*Inicializa o nó*/
    Tree->numNos += 1;
    NodeTree *No = malloc(sizeof(NodeTree));
    No->x = x;
    No->y = y;
    No->info = i;
    No->removido = false;
    No->pai = NULL;
    No->filhos = malloc(Tree->numSetores * sizeof(NodeTree *));
    for (int i = 0; i < Tree->numSetores; i++)
    {
        No->filhos[i] = NULL;
    }

    /*Atribui o nó a árvore*/
    if (Tree->node == NULL)
    {
        /*Primeiro nó da árvore*/
        Tree->node = No;
    }
    else
    {
        /*Procura o setor do qual o nó pertence e o insere*/
        NodeTree *P = Tree->node;
        do
        {
            double x1 = P->x;
            double y1 = P->y;
            double x2 = x;
            double y2 = y;
            double Theta = atan2(y2 - y1, x2 - x1); // Ângulo do nó em relação ao ponto de referência (x1,y1)
            Theta = RadianosParaGraus(Theta);
            if (Theta < 0)
            {
                Theta += 360;
            }
            for (int i = 0; i < Tree->numSetores; i++)
            {
                double InclinacaoRetaInf = i * 360 / Tree->numSetores;
                double InclinacaoRetaSup = (i + 1) * 360 / Tree->numSetores;
                if (VerificaIntervalo(InclinacaoRetaInf, Theta, InclinacaoRetaSup))
                {
                    if (P->filhos[i] == NULL)
                    {
                        /*Nó não tem um filho pertencente ao setor*/
                        P->filhos[i] = No;
                        No->pai = P;
                        return No;
                    }
                    else
                    {
                        /*Nó já tem um filho pertencente ao setor*/
                        P = P->filhos[i];
                        break;
                    }
                }
            }
        } while (P != NULL);
    }
    return No;
}

Node getNodeRadialT(RadialTree t, double x, double y, double epsilon)
{
    /*Procura o nó com o ponto (x,y) e o epsilon (erro)*/
    Raiz *Tree = t;
    NodeTree *P = Tree->node;
    do
    {
        double Theta = atan2(y - P->y, x - P->x); // Ângulo do nó em relação ao ponto de referência (x1,y1)
        Theta = RadianosParaGraus(Theta);
        if (Theta < 0)
        {
            Theta += 360;
        }
        for (int i = 0; i < Tree->numSetores; i++)
        {
            double InclinacaoRetaInf = i * 360 / Tree->numSetores;
            double InclinacaoRetaSup = (i + 1) * 360 / Tree->numSetores;
            if (VerificaIntervalo(InclinacaoRetaInf, Theta, InclinacaoRetaSup))
            {
                if (fabs(P->x - x) < epsilon && fabs(P->y - y) < epsilon)
                {
                    return P;
                }
                else
                {
                    P = P->filhos[i];
                    break;
                }
            }
        }
    } while (P != NULL);
    return NULL;
}

void removeNoRadialT(RadialTree t, Node n)
{
    Raiz *Tree = t;
    NodeTree *Rmv = n;
    Rmv->removido = true;
}

Info getInfoRadialT(RadialTree t, Node n)
{
    return t;
}

bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L)
{
    return false;
}

bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, FdentroDeRegiao f, Lista L)
{
    return false;
}

bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L)
{
    return false;
}

void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux)
{
}

void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux)
{
}

// Ideias de funções para dar free
void freeNode(Node n)
{
    NodeTree *No = n;
    free(No->info);
    free(No->filhos);
    free(No);
}

void freeRadialTree(RadialTree t)
{
    Raiz *Tree = t;
    NodeTree *No = Tree->node;
    NodeTree *Clear = NULL;
    int i;
    while (Tree->numNos != 0)
    {
        bool Vazio = true;
        for (i = 0; i < Tree->numSetores; i++)
        {
            if (No->filhos[i] != NULL)
            {
                Vazio = false;
                break;
            }
        }
        if (Vazio)
        {
            /*Nó não tem filhos*/
            Clear = No;
            No = No->pai;
            freeNode(Clear);
            if (No == NULL)
            {
                /*Nó raiz*/
                Tree->node = NULL;
                free(Tree);
                return;
            }
            No->filhos[i] = NULL;
        }
        else
        {
            /*Primeiro filho encontrado do nó*/
            No = No->filhos[i];
        }
    }
}
