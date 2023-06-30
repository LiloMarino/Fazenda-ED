#ifndef SORT_H
#define SORT_H

#include <stddef.h>

typedef void *Sort;

void SelectionSortInt(int V[], int n);

void TiredBubbleSortInt(int V[], int n);

void BubbleSortInt(int V[], int n);

void InsertionSortInt(int V[], int n);

void ShellSortInt(int V[], int n);

void QuickSortInt(int V[], int n);

void HeapSortInt(int V[], int n);

void MergeSortInt(int V[], int n);

void SelectionSortChar(char V[], int n);

void TiredBubbleSortChar(char V[], int n);

void BubbleSortChar(char V[], int n);

void InsertionSortChar(char V[], int n);

void ShellSortChar(char V[], int n);

void QuickSortChar(char V[], int n);

void HeapSortChar(char V[], int n);

void MergeSortChar(char V[], int inicio, int fim);

/**
 * @brief Troca os valores de dois elementos de um array ou de qualquer tipo de dado.
 *
 * @param A Ponteiro para o primeiro elemento.
 * @param B Ponteiro para o segundo elemento.
 * @param size Tamanho dos elementos em bytes, pode ser obtido pela função sizeof().
 *
 * @note É importante garantir que os ponteiros passados como argumentos apontem para o tipo de dado correto.
 * Caso contrário, a função Swap pode produzir comportamentos inesperados ou resultar em erros de compilação.
 */
void Swap(Sort A, Sort B, size_t size);

#endif