#include "sort.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void SelectionSortInt(int V[], int n)
{
    int i, j, minimo;
    for (i = 0; i <= n - 2; i++)
    {
        minimo = i;
        for (j = i + 1; j <= n - 1; j++)
        {
            if (V[j] < V[minimo])
            {
                minimo = j;
            }
        }
        Swap(&V[minimo], &V[i], sizeof(int));
    }
}

void TiredBubbleSortInt(int V[], int n)
{
    for (int j = n - 2; j >= 1; j--)
    {
        for (int i = 0; i <= j; i++)
        {
            if (V[i] > V[i + 1])
            {
                Swap(&V[i], &V[i + 1], sizeof(int));
            }
        }
    }
}

void BubbleSortInt(int V[], int n)
{
    bool trocado;
    do
    {
        trocado = false;
        for (int i = 0; i < n - 1; i++)
        {
            if (V[i] > V[i + 1])
            {
                Swap(&V[i], &V[i + 1], sizeof(int));
                trocado = true;
            }
        }
        n = n - 1;
    } while (trocado);
}

void InsertionSortInt(int V[], int n)
{
    int i, j;
    int temp;
    for (i = 1; i <= n - 1; i++)
    {
        temp = V[i];
        j = i - 1;
        while (temp < V[j] && j >= 0)
        {
            V[j+1] = V[j];
            j = j - 1;
        }
        V[j+1] = temp;
    }
}

void ShellSortInt(int V[], int n)
{
    int i, j, k, temp;

    for (k = n / 2; k > 0; k /= 2)
    {
        for (i = k; i < n; i++)
        {
            temp = V[i];
            for (j = i; j >= k; j -= k)
            {
                if (temp < V[j - k])
                {
                    V[j] = V[j - k];
                }
                else
                {
                    break;
                }
            }
            V[j] = temp;
        }
    }
}


void QuickSortInt(int V[], int n)
{
    int i = 0,f = n;
    int pivo, j;
    if (i == f)
    {
        return;
    }

    /* Adquire o Pivo */
    pivo = V[(i+f)/2];

    /* Particiona */
    int e,d;
    e = i;
    d = f;
    while (e < d)
    {
        while ((d >= i) && (V[d] > pivo))
        {
            d--;
        }
        while ((e < f) && (V[e] < pivo))
        {
            e++;
        }
        if (e < d)
        {
            Swap(&V[d],&V[e],sizeof(int));
            e++;
            d--;
        }
        
    }
    j = d;

    /*Quick Sort*/
    QuickSortInt(V,i,j);
    QuickSortInt(V,j+1,f);
}

void HeapifyInt(int V[], int n, int i)
{
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && V[esq] > V[maior])
        maior = esq;

    if (dir < n && V[dir] > V[maior])
        maior = dir;

    if (maior != i)
    {
        Swap(&V[i], &V[maior], sizeof(int));
        HeapifyInt(V, n, maior);
    }
}

void HeapSortInt(int V[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        HeapifyInt(V, n, i);

    for (int i = n - 1; i >= 0; i--)
    {
        Swap(&V[0], &V[i], sizeof(int));
        HeapifyInt(V, i, 0);
    }
}

void MergeInt(int V[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays.
    int L[n1], R[n2];

    // Copy data to temp arrays L[] and R[].
    for (i = 0; i < n1; i++)
        L[i] = V[l + i];
    for (j = 0; j < n2; j++)
        R[j] = V[m + 1+ j];

    // Merge the temp arrays back into V[l..r].
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            V[k] = L[i];
            i++;
        } 
        else {
            V[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any.
    while (i < n1) {
        V[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any.
    while (j < n2) {
        V[k] = R[j];
        j++;
        k++;
    }
}

void MergeSortInt(int V[], int n) {
    if (n < 2) {
        return;
    }
    // Same as (l+r)/2, but avoids overflow for large l and h.
    int m = n/2;

    // Sort first and second halves.
    MergeSortInt(V, m);
    MergeSortInt(V + m, n - m);

    // Merge the sorted halves.
    MergeInt(V, 0, m - 1, n - 1);
}

void SelectionSortChar(char V[], int n)
{
    int i, j, minimo;
    for (i = 0; i <= n - 2; i++)
    {
        minimo = i;
        for (j = i + 1; j <= n - 1; j++)
        {
            if (V[j] < V[minimo])
            {
                minimo = j;
            }
        }
        Swap(&V[minimo], &V[i], sizeof(char));
    }
}

void TiredBubbleSortChar(char V[], int n)
{
    for (int j = n - 2; j >= 1; j--)
    {
        for (int i = 0; i <= j; i++)
        {
            if (V[i] > V[i + 1])
            {
                Swap(&V[i], &V[i + 1], sizeof(char));
            }
        }
    }
}

void BubbleSortChar(char V[], int n)
{
    bool trocado;
    do
    {
        trocado = false;
        for (int i = 0; i < n - 1; i++)
        {
            if (V[i] > V[i + 1])
            {
                Swap(&V[i], &V[i + 1], sizeof(char));
                trocado = true;
            }
        }
        n = n - 1;
    } while (trocado);
}

void InsertionSortChar(char V[], int n)
{
    int i, j;
    char temp;
    for (i = 1; i <= n - 1; i++)
    {
        temp = V[i];
        j = i - 1;
        while (temp < V[j] && j >= 0)
        {
            V[j+1] = V[j];
            j = j - 1;
        }
        V[j+1] = temp;
    }
}

void ShellSortChar(char V[], int n)
{
    int i, j, k;
    char temp;
    for (k = n / 2; k > 0; k /= 2)
    {
        for (i = k; i < n; i++)
        {
            temp = V[i];
            for (j = i; j >= k; j -= k)
            {
                if (temp < V[j - k])
                {
                    V[j] = V[j - k];
                }
                else
                {
                    break;
                }
            }
            V[j] = temp;
        }
    }
}


void QuickSortChar(char V[], int n)
{
    int i = 0,f = n;
    int j;
    char pivo;
    if (i == f)
    {
        return;
    }

    /* Adquire o Pivo */
    pivo = V[(i+f)/2];

    /* Particiona */
    int e,d;
    e = i;
    d = f;
    while (e < d)
    {
        while ((d >= i) && (V[d] > pivo))
        {
            d--;
        }
        while ((e < f) && (V[e] < pivo))
        {
            e++;
        }
        if (e < d)
        {
            Swap(&V[d],&V[e],sizeof(char));
            e++;
            d--;
        }
        
    }
    j = d;

    /*Quick Sort*/
    QuickSortChar(V,i,j);
    QuickSortChar(V,j+1,f);
}

void HeapifyChar(char V[], int n, int i)
{
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && V[esq] > V[maior])
        maior = esq;

    if (dir < n && V[dir] > V[maior])
        maior = dir;

    if (maior != i)
    {
        Swap(&V[i], &V[maior], sizeof(char));
        HeapifyChar(V, n, maior);
    }
}

void HeapSortChar(char V[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        HeapifyChar(V, n, i);

    for (int i = n - 1; i >= 0; i--)
    {
        Swap(&V[0], &V[i], sizeof(char));
        HeapifyChar(V, i, 0);
    }
}

void MergeChar(char V[], int inicio, int meio, int fim)
{
    int i, j, k;
    int tamEsq = meio - inicio + 1;
    int tamDir = fim - meio;

    // criar cópias dos vetores inicial e final
    char esq[tamEsq], dir[tamDir];
    
    for(i = 0; i < tamEsq; i++)
        esq[i] = V[inicio + i];
    
    for(j = 0; j < tamDir; j++)
        dir[j] = V[meio + 1 + j];

    // intercalar os dois subvetores na ordem correta
    i = j = 0;
    k = inicio;

    while(i < tamEsq && j < tamDir)
    {
        if(esq[i] <= dir[j])
            V[k++] = esq[i++];
        else
            V[k++] = dir[j++];
    }

    // copiar elementos restantes da primeira metade (se houver)
    while(i < tamEsq)
        V[k++] = esq[i++];
        
    // copiar elementos restantes da segunda metade (se houver)
    while(j < tamDir)
        V[k++] = dir[j++];
}

void MergeSortChar(char V[], int inicio, int fim)
{
    if(inicio < fim)
    {
        int meio = inicio + (fim - inicio) / 2;

        mergeSortChar(V, inicio, meio);
        mergeSortChar(V, meio + 1, fim);
        MergeChar(V, inicio, meio, fim);
    }
}

void Swap(Sort A, Sort B, size_t size)
{
    void *temp = malloc(size);
    memcpy(temp, A, size);
    memcpy(A, B, size);
    memcpy(B, temp, size);
    free(temp);
}
