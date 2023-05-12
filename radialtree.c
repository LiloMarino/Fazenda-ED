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
    double limiar;
    int numTotalNos;
    int numNosRemovidos;
    struct StNodeTree *node;
};

typedef struct StNodeTree NodeTree;
typedef struct StRaiz Raiz;

RadialTree newRadialTree(int numSetores, double fd)
{
    Raiz *Tree = malloc(sizeof(Raiz));
    Tree->numSetores = numSetores;
    Tree->numTotalNos = 0;
    Tree->numNosRemovidos = 0;
    Tree->limiar = fd;
    Tree->node = NULL;
    return Tree;
}

Node insertRadialT(RadialTree t, double x, double y, Info i)
{
    Raiz *Tree = t;

    /*Inicializa o nó*/
    Tree->numTotalNos += 1;
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

/**
 * @brief Desaloca a memória alocada pelo nó da árvore
 * @param n Nó da árvore radial
 */
void freeNode(Node n)
{
    NodeTree *No = n;
    free(No->info); // Tem que analisar esta parte dependendo da info
    free(No->filhos);
    free(No);
}

/**
 * @brief Desaloca toda a memória da árvore
 * @param t Árvore radial
 */
void freeRadialTree(RadialTree t)
{
    /** @warning É necessário passar o endereço do ponteiro da árvore para esta função*/
    Raiz *Tree = *((Raiz **)t);
    NodeTree *No = Tree->node;
    NodeTree *Clear = NULL;
    int i;
    while (Tree->numTotalNos != 0)
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
            if (No != NULL)
            {
                /*Atribui NULL ao filho desalocado*/
                for (i = 0; i < Tree->numSetores; i++)
                {
                    if (No->filhos[i] != NULL)
                    {
                        No->filhos[i] = NULL;
                        break;
                    }
                }
            }
            else
            {
                /*Nó raiz*/
                Tree->node = NULL;
                free(Tree);
                *(void **)t = NULL;
                return;
            }
        }
        else
        {
            /*Primeiro filho diferente de NULL encontrado do nó*/
            No = No->filhos[i];
        }
    }
}

/**
 * @brief Percorre toda a árvore e retorna a lista dos elementos não removidos
 * @param t Árvore radial
 * @return Retorna a lista de elementos presentes na árvore
 */
Lista VerificaArvore(RadialTree t)
{
    Raiz *Tree = t;
    NodeTree *raiz = Tree->node;
    Lista Stack = createLst(-1);
    Lista Existe = createLst(-1);

    insertLst(Stack, raiz);

    while (!isEmptyLst(Stack))
    {
        NodeTree *No = popLst(Stack);

        if (!(No->removido))
        {
            insertLst(Existe, No);
        }

        for (int i = 0; i < Tree->numSetores; i++)
        {
            if (No->filhos[i] != NULL)
            {
                insertLst(Stack, No->filhos[i]);
            }
        }
    }
    killLst(Stack);
    return Existe;
}

void removeNoRadialT(RadialTree t, Node n)
{
    /** @warning É necessário passar o endereço do ponteiro da árvore para esta função para a mudança da árvore*/
    Raiz *Tree = *((Raiz **)t);
    NodeTree *Rmv = n;
    Rmv->removido = true;
    Tree->numNosRemovidos++;
    double fd = ((double) Tree->numNosRemovidos) / Tree->numTotalNos ;

    /*Verifica se é necessário recriar a árvore*/
    if (fd > Tree->limiar)
    {
        RadialTree NovaArvore = newRadialTree(Tree->numSetores, Tree->limiar);
        Lista Aux = VerificaArvore(Tree);
        while (!isEmptyLst(Aux))
        {
           NodeTree *No = popLst(Aux);
           insertRadialT(NovaArvore,No->x,No->y,No->info);
        }
        freeRadialTree(t);
        killLst(Aux);
        *(void **)t = NovaArvore;
    }
}