#include "radialtree.h"
#include "geo.h"
#include "svg.h"
#include "dot.h"
#include "Bibliotecas/utilities.h"
#include "Bibliotecas/geradores.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

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

struct StOrganiza
{
    double distancia;
    struct StNodeTree *node;
};

typedef struct StNodeTree NodeTree;
typedef struct StRaiz Raiz;
typedef struct StOrganiza Organiza;

/*========================================================================================================== *
 * Funções Auxiliares                                                                                        *
 *========================================================================================================== */

/**
 * @brief Desaloca a memória alocada pelo nó da árvore
 * @param n Nó da árvore radial
 */
void freeNode(Node n, bool ClearTotal)
{
    NodeTree *No = n;
    if (ClearTotal)
    {
        FreeFigura(No->info); // Atenção aqui, é necessário analisar esta parte dependendo da informação contida no nó
        No->info = NULL;
    }
    free(No->filhos);
    free(No);
}

void freeRadialTree(RadialTree t, bool ClearTotal)
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
            freeNode(Clear, ClearTotal);
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
 * @brief Função usada para o qsort ordenar o vetor de Nós conforme a distância
 * @param a Elemento A
 * @param b Elemento B
 * @return Retorna 1 se a distancia A > distancia B, -1 se distancia A < distancia B e 0 se distancia A = distancia B
 */
int ComparaDistancia(const void *a, const void *b)
{
    const Organiza *elem1 = (const Organiza *)a;
    const Organiza *elem2 = (const Organiza *)b;

    if (elem1->distancia > elem2->distancia)
    {
        return 1;
    }
    else if (elem1->distancia < elem2->distancia)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Percorre toda a árvore e retorna a lista reorganizada dos elementos não removidos
 * @param t Árvore radial
 * @return Retorna a lista reorganizada de elementos presentes na árvore
 */
Lista VerificaArvore(RadialTree t)
{
    Raiz *Tree = t;
    NodeTree *raiz = Tree->node;
    Lista Stack = createLst(-1);
    Lista Existe = createLst(-1);
    Lista Removidos = createLst(-1);
    insertLst(Stack, raiz);
    bool first = true;
    double xmin = DBL_MAX, xmax = -DBL_MAX, ymin = DBL_MAX, ymax = -DBL_MAX;

    while (!isEmptyLst(Stack))
    {
        NodeTree *No = popLst(Stack);

        for (int i = 0; i < Tree->numSetores; i++)
        {
            if (No->filhos[i] != NULL)
            {
                insertLst(Stack, No->filhos[i]);
            }
        }
        if (!(No->removido))
        {
            insertLst(Existe, No);
            if (first)
            {
                xmin = No->x;
                xmax = No->x;
                ymin = No->y;
                ymax = No->y;
                first = false;
            }

            /*Verifica se o nó existente é algum máximo ou mínimo*/
            if (No->x > xmax)
            {
                xmax = No->x;
            }
            else if (No->x < xmin)
            {
                xmin = No->x;
            }
            if (No->y > ymax)
            {
                ymax = No->y;
            }
            else if (No->y < ymin)
            {
                ymin = No->y;
            }
        }
        else
        {
            insertLst(Removidos, No);
        }
    }
    killLst(Stack);

    /*Remove as informações dos nós removidos*/
    while (!isEmptyLst(Removidos))
    {
        NodeTree *No = popLst(Removidos);
        if (No->info != NULL)
        {
            FreeFigura(No->info); // Atenção aqui, é necessário analisar esta parte dependendo da informação contida no nó
            No->info = NULL;
        }
    }
    killLst(Removidos);

    /*Reorganiza a árvore baseado no ponto médio*/
    double xmeio = (xmax + xmin) / 2;
    double ymeio = (ymax + ymin) / 2;

    /*Cria o vetor para a ordenação a partir da lista*/
    Organiza Aux[lengthLst(Existe)];
    Iterador I = createIterador(Existe, false);
    for (int i = 0; !isIteratorEmpty(Existe, I); i++)
    {
        Aux[i].node = getIteratorNext(Existe, I);
        Aux[i].distancia = Distancia2Pontos(xmeio, ymeio, Aux[i].node->x, Aux[i].node->y);
    }
    killIterator(I);

    /*Ordena utilizando qsort*/
    qsort(Aux, lengthLst(Existe), sizeof(Organiza), ComparaDistancia);

    /*Insere os elementos organizados na nova lista*/
    Lista Ordenado = createLst(-1);
    for (int i = 0; i < lengthLst(Existe); i++)
    {
        insertLst(Ordenado, Aux[i].node);
    }
    killLst(Existe);

    return Ordenado;
}

/*========================================================================================================== *
 * Funções Principais                                                                                        *
 *========================================================================================================== */

RadialTree newRadialTree(int numSetores, double fd)
{
    Raiz *Tree = calloc(1, sizeof(Raiz));
    Tree->numSetores = numSetores;
    Tree->limiar = fd;
    return Tree;
}

Node insertRadialT(RadialTree t, double x, double y, Info i)
{
    Raiz *Tree = t;

    /*Inicializa o nó*/
    Tree->numTotalNos += 1;
    NodeTree *No = calloc(1, sizeof(NodeTree));
    No->x = x;
    No->y = y;
    No->info = i;
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
                        LigaNo(ARQDOT, t, No->pai, No);
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
    LigaNo(ARQDOT, t, No->pai, No);
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
    /** @warning É necessário passar o endereço do ponteiro da árvore para esta função para a mudança da árvore*/
    Raiz *Tree = *((Raiz **)t);
    NodeTree *Rmv = n;
    Rmv->removido = true;
    Tree->numNosRemovidos++;
    double fd = ((double)Tree->numNosRemovidos) / Tree->numTotalNos;
    MarcaNoRemovido(ARQDOT, t, Rmv);
    /*Verifica se é necessário recriar a árvore*/
    if (fd > Tree->limiar)
    {
        TerminaDot(ARQDOT);
        ARQDOT = CriaLog(FNARQDOT, "dot");
        InicializaDot(ARQDOT);
        RadialTree NovaArvore = newRadialTree(Tree->numSetores, Tree->limiar);
        Lista Aux = VerificaArvore(Tree);
        while (!isEmptyLst(Aux))
        {
            NodeTree *No = popLst(Aux);
            insertRadialT(NovaArvore, No->x, No->y, No->info);
        }
        freeRadialTree(t, false);
        killLst(Aux);
        *(void **)t = NovaArvore;
    }
}

Info getInfoRadialT(RadialTree t, Node n)
{
    NodeTree *No = n;
    return No->info;
}

bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L)
{
    Raiz *Tree = t;
    NodeTree *No = Tree->node;
    bool Existe = false;
    /*Regulariza as coordenadas para o processamento*/
    /*Precisa-se x2 > x1 e y1 > y2*/
    if (x1 > x2)
    {
        double aux = x1;
        x1 = x2;
        x2 = aux;
    }
    if (y2 > y1)
    {
        double aux = y1;
        y1 = y2;
        y2 = aux;
    }

    /*Calcula o angulo das coordenadas em relação ao centro*/
    double Theta1;
    double Theta2;
    /*O Leste é um caso específico pois o ângulo é medido a partir do 0° que fica no Leste,
    e portanto não é possível obter o intervalo de ângulo a partir de Theta2 - Theta1, mas sim por 360° - Theta2 + Theta1*/
    bool Leste = false;
    if (No->x < x1 && No->x < x2)
    {
        /*Leste*/
        Leste = true;
        Theta1 = atan2(y1 - No->y, x1 - No->x);             // Ângulo do nó em relação ao ponto de referência (x1,y1)
        Theta2 = atan2(y2 - No->y, x2 - (x2 - x1) - No->x); // Ângulo do nó em relação ao ponto de referência (x2-largura,y2)
    }
    else if (No->x < y1 && No->x < y2)
    {
        /*Norte*/
        Theta1 = atan2(y2 - No->y, x2 - (x2 - x1) - No->x); // Ângulo do nó em relação ao ponto de referência (x2-largura,y2)
        Theta2 = atan2(y2 - No->y, x2 - No->x);             // Ângulo do nó em relação ao ponto de referência (x2,y2)
    }
    else if (No->x > x1 && No->x > x2)
    {
        /*Oeste*/
        Theta1 = atan2(y1 - No->y, x1 + (x2 - x1) - No->x); // Ângulo do nó em relação ao ponto de referência (x1+largura,y1)
        Theta2 = atan2(y2 - No->y, x2 - No->x);             // Ângulo do nó em relação ao ponto de referência (x2,y2)
    }
    else if (No->x > y1 && No->x > y2)
    {
        /*Sul*/
        Theta1 = atan2(y1 - No->y, x1 - No->x);             // Ângulo do nó em relação ao ponto de referência (x1,y1)
        Theta2 = atan2(y1 - No->y, x1 + (x2 - x1) - No->x); // Ângulo do nó em relação ao ponto de referência (x1+largura,y1)
    }
    else
    {
        /*Centro*/
        Theta1 = 0;
        Theta2 = 2 * PI;
    }

    Theta1 = RadianosParaGraus(Theta1);
    Theta2 = RadianosParaGraus(Theta2);
    if (Theta1 < 0)
    {
        Theta1 += 360;
    }
    if (Theta2 < 0)
    {
        Theta2 += 360;
    }
    /*Theta 2 deve ser maior que Theta 1*/
    if (Theta1 > Theta2)
    {
        double aux = Theta1;
        Theta1 = Theta2;
        Theta2 = aux;
    }

    /*Cria o Stack de verificação baseado na direção da área*/
    Lista Stack = createLst(-1);
    Lista Aux = createLst(-1);
    if (!(No->removido))
    {
        insertLst(Aux, No);
    }
    for (int i = 0; i < Tree->numSetores; i++)
    {
        double InclinacaoRetaInf = i * 360 / Tree->numSetores;
        double InclinacaoRetaSup = (i + 1) * 360 / Tree->numSetores;
        if (((InclinacaoRetaSup >= Theta2 || Theta1 >= InclinacaoRetaInf) && Leste) || ((VerificaIntervalo(Theta1, InclinacaoRetaInf, Theta2) || VerificaIntervalo(Theta1, InclinacaoRetaSup, Theta2)) && !Leste))
        {
            if (No->filhos[i] != NULL)
            {
                insertLst(Stack, No->filhos[i]);
            }
        }
    }

    /*Percorre o Stack de verificação e verifica se o nó tem algum filho dentro da região*/
    while (!isEmptyLst(Stack))
    {
        No = popLst(Stack);
        for (int i = 0; i < Tree->numSetores; i++)
        {
            if (No->filhos[i] != NULL)
            {
                insertLst(Stack, No->filhos[i]);
            }
        }

        if (!(No->removido))
        {
            insertLst(Aux, No);
        }
    }

    /*Insere na lista L quem retornar verdadeiro de Aux*/
    while (!isEmptyLst(Aux))
    {
        NodeTree *P = popLst(Aux);
        if (VerificaPonto(x1, P->x, x2, y1, P->y, y2))
        {
            insertLst(L, P);
            Existe = true;
        }
    }
    killLst(Stack);
    killLst(Aux);

    return Existe;
}

bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, FdentroDeRegiao f, Lista L)
{
    bool Existe = false;
    /*Cria o Stack de verificação baseado na área*/
    Lista Stack = createLst(-1);
    if (getNodesDentroRegiaoRadialT(t, x1, y1, x2, y2, Stack))
    {
        while (!isEmptyLst(Stack))
        {
            /*Aplica a função f para cada nó presente na área*/
            NodeTree *P = popLst(Stack);
            if (f(P->info, x1, y1, x2, y2))
            {
                insertLst(L, P);
                Existe = true;
            }
        }
        killLst(Stack);
    }
    else
    {
        /*Não existe nós presentes na região*/
        killLst(Stack);
    }
    return Existe;
}

bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L)
{
    bool Existe = false;
    NodeTree *P = getNodeRadialT(t, x, y, 1.0);
    if (P == NULL)
    {
        return Existe;
    }
    else
    {
        if (f(P->info, x, y))
        {
            insertLst(L, P);
            Existe = true;
        }
    }
    return Existe;
}

void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    Raiz *Tree = t;
    NodeTree *raiz = Tree->node;
    if (Tree->node == NULL)
    {
        return;
    }
    /*Cria o Stack de verificação, junto com um vetor para indicar se já foi verificado*/
    bool *visitado = calloc(Tree->numTotalNos, sizeof(bool));
    Lista Stack = createLst(-1);
    insertLst(Stack, raiz);
    while (!isEmptyLst(Stack))
    {
        NodeTree *Atual = popLst(Stack);

        if (!visitado[Atual - raiz])
        {
            visitado[Atual - raiz] = true;
            f(Atual->info, Atual->x, Atual->y, aux);

            for (int i = Tree->numSetores - 1; i >= 0; i--)
            {
                NodeTree *filho = Atual->filhos[i];
                if (filho != NULL && !visitado[filho - raiz])
                {
                    insertLst(Stack, filho);
                }
            }
        }
    }

    killLst(Stack);
    free(visitado);
}

void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux)
{
    Raiz *Tree = t;
    if (Tree->node == NULL)
    {
        return; // Árvore vazia, não há nada para visitar
    }

    NodeTree *No = Tree->node;
    Lista Stack = createLst(-1);
    insertLst(Stack, No);

    while (!isEmptyLst(Stack))
    {
        No = popLst(Stack);
        if (!No->removido)
        {
            f(No->info, No->x, No->y, aux);
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
}

Node procuraNoRadialT(RadialTree t, FsearchNo f, void *aux)
{
    Raiz *Tree = t;
    NodeTree *No = Tree->node;
    Lista Stack = createLst(-1);
    insertLst(Stack, No);

    while (!isEmptyLst(Stack))
    {
        No = popLst(Stack);
        if (!No->removido)
        {
            if (f(No->info, No->x, No->y, aux))
            {
                killLst(Stack);
                return No;
            }
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
    return NULL;
}