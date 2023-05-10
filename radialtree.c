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
    struct StNodeTree **nodes;
    bool removido;
};

struct StRaiz
{
    int numSetores;
    double fd;
    struct StNodeTree *nodes;
};

typedef struct StNodeTree NodeTree;
typedef struct StRaiz Raiz;

RadialTree newRadialTree(int numSetores, double fd)
{
    if (0 <= fd && fd < 1.0)
    {
        Raiz *Tree = malloc(sizeof(Raiz));
        Tree->numSetores = numSetores;
        Tree->fd = fd;
        Tree->nodes = NULL;
        return Tree;
    }
    else
    {
        printf("Fator de degradação fora dos limites!");
        exit(1);
    }
}

Node insertRadialT(RadialTree t, double x, double y, Info i)
{
    Raiz *Tree = t;

    /*Inicializa o nó*/
    NodeTree *No = malloc(sizeof(NodeTree));
    No->x = x;
    No->y = y;
    No->info = i;
    No->removido = false;
    No->nodes = malloc(Tree->numSetores * sizeof(NodeTree *));
    for (int i = 0; i < Tree->numSetores; i++)
    {
        No->nodes[i] = NULL;
    }

    if (Tree->nodes == NULL)
    {
        /*Primeiro nó da árvore*/
        Tree->nodes = No;
    }
    else
    {
        /*Procura o setor do qual o nó pertence e o insere*/
        NodeTree *P = Tree->nodes;
        do
        {
            double x1 = P->x;
            double y1 = P->y;
            double x2 = x;
            double y2 = y;
            double Theta = atan2(y2 - y1, x2 - x1); // Ângulo do nó em relação ao ponto de referência (x1,y1)
            for (int i = 0; i < Tree->numSetores; i++)
            {
                double InclinacaoRetaInf = i * 360 / Tree->numSetores;
                double InclinacaoRetaSup = (i + 1) * 360 / Tree->numSetores;
                if (VerificaIntervalo(InclinacaoRetaInf, Theta, InclinacaoRetaSup))
                {
                    if (P->nodes[i] == NULL)
                    {
                        /*Nó não tem um filho pertencente ao setor*/
                        P->nodes[i] = No;
                        return No;
                    }
                    else
                    {
                        /*Nó já tem um filho pertencente ao setor*/
                        P = P->nodes[i];
                    }
                }
            }
        } while (P != NULL);
    }
    return No;
}

// Ideias de funções para dar free
void freeNode(NodeTree *node)
{
    if (node == NULL)
    {
        return;
    }

    for (int i = 0; i < node->numNodes; i++)
    {
        freeNode(node->nodes[i]);
    }

    free(node->nodes);
    free(node);
}

void freeRadialTree(Raiz *tree)
{
    for (int i = 0; i < tree->numSetores; i++)
    {
        freeNode(tree->nodes[i]);
    }

    free(tree->nodes);
    free(tree);
}
