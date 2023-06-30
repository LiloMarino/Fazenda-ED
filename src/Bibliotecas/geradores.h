#ifndef GERADORES_H
#define GERADORES_H

/**
 * @file geradores.h
 * @brief Biblioteca de funções geradoras de dados aleatórios
 * @warning É necessário utilizar InicializaRand() antes de usar qualquer função de geração aleatória
 */

#include <stdio.h>

/**
 * @brief Inicializa a semente do gerador de números aleatórios com o valor do tempo atual
 * A função InicializaRand() utiliza a função time(NULL) para obter o tempo atual em segundos desde 1º de janeiro de 1970.
 * Esse valor é utilizado como semente para a função srand(), que inicializa o gerador de números aleatórios do programa.
 */
void InicializaRand();

/**
 * @brief Gera uma string de caracteres aleatórios.
 * @param n Tamanho da string gerada.
 * @param string Vetor onde a string será armazenada.
 *
 */
void GerarString(int n, char string[]);

/**
 * @brief Gera um número float aleatório dentro do intervalo determinado
 * @param min Menor número que pode ser gerado
 * @param max Maior número que pode ser gerado
 * @return Retorna o número gerado
 *
 */
float GerarNumeroFloat(int min, int max);

/**
 * @brief Gera um número double aleatório dentro do intervalo determinado
 * @param min Menor número que pode ser gerado
 * @param max Maior número que pode ser gerado
 * @return Retorna o número gerado
 */
double GerarNumeroDouble(double min, double max);

/**
 * @brief Gera um número int aleatório dentro do intervalo determinado
 * @param min Menor número que pode ser gerado
 * @param max Maior número que pode ser gerado
 * @return Retorna o número gerado
 *
 */
int GerarNumeroInt(int min, int max);

/**
 * @brief Gera um vetor de inteiros aleatórios.
 * @param min Valor mínimo que pode ser gerado.
 * @param max Valor máximo que pode ser gerado.
 * @param v Vetor onde os valores serão armazenados.
 * @param n Tamanho do vetor gerado.
 *
 */
void GerarVetorInt(int min, int max, int v[], int n);

/**
 * @brief Printa na tela o vetor
 * @param vetor_char Vetor de caracteres
 * @param nc Tamanho do vetor de char
 */
void ImprimeVetorChar(const char vetor_char[], const int nc);

/**
 * @brief Printa na tela o vetor
 * @param vetor_int Vetor de inteiros
 * @param ni Tamanho do vetor de inteiros
 */
void ImprimeVetorInt(const int vetor_int[], const int ni);

/**
 * @brief Gera uma matriz de caracteres aleatórios e salva em um arquivo.
 * @param caps Tipo de caracteres a ser gerado, pode ser "Mm", "M" ou "m".
 * @param linha Número de linhas da matriz gerada.
 * @param coluna Número de colunas da matriz gerada.
 * @param nomearq Nome do arquivo onde a matriz será salva.
 *
 */
void GerarMatrizChar(char caps[], int linha, int coluna, char nomearq[]);

/**
 * @brief Gera uma matriz de inteiros aleatórios e salva em um arquivo.
 * @param min Valor mínimo que pode ser gerado.
 * @param max Valor máximo que pode ser gerado.
 * @param linha Número de linhas da matriz gerada.
 * @param coluna Número de colunas da matriz gerada.
 * @param nomearq Nome do arquivo onde a matriz será salva.
 *
 */
void GerarMatrizInt(int min, int max, int linha, int coluna, char nomearq[]);

/**
 * @brief Gera uma matriz esparsa de inteiros aleatórios e salva em um arquivo.
 * @param min Valor mínimo que pode ser gerado.
 * @param max Valor máximo que pode ser gerado.
 * @param linha Número de linhas da matriz gerada.
 * @param coluna Número de colunas da matriz gerada.
 * @param nomearq Nome do arquivo onde a matriz será salva.
 *
 */
void GerarMatrizEsparsa(int min, int max, int linha, int coluna, char nomearq[]);

/**
 * @brief Função que lê uma matriz de caracteres de um arquivo e imprime a matriz na tela.
 *
 * @param nomearq Nome do arquivo a ser lido.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @return Retorna a matriz alocada dinamicamente
 *
 * @note A matriz é alocada dinamicamente.
 *
 */
char **LerMatrizChar(char nomearq[], int linha, int coluna);

/**
 * @brief Função que lê uma matriz de inteiros de um arquivo e imprime a matriz na tela.
 *
 * @param nomearq Nome do arquivo a ser lido.
 * @param linha Número de linhas da matriz.
 * @param coluna Número de colunas da matriz.
 * @return Retorna a matriz alocada dinamicamente
 * 
 * @note A matriz é alocada dinamicamente.
 *
 */
int **LerMatrizInt(char nomearq[], int linha, int coluna);

/**
 * @brief Função que libera a memória alocada para uma matriz dinâmica.
 *
 * @param mat Matriz dinâmica a ser liberada.
 * @param linha Número de linhas da matriz.
 *
 * @note A matriz deve ter sido alocada dinamicamente usando malloc() ou calloc().
 *
 */
void LiberarMatriz(void **mat, int linha);

/**
 * @brief Cria um arquivo de log com o nome especificado.
 *
 * @param nome Nome base para o arquivo de log.
 * @param ext Extensão do arquivo sem o .
 * @return Ponteiro para o arquivo de log criado.
 *
 * @note A função adiciona a extensão ao nome do arquivo e verifica se já existe
 * um arquivo com o mesmo nome. Se já existir, adiciona um número ao nome para evitar
 * sobrescrever o arquivo existente. É necessário fechar o arquivo com fclose().
 *
 * @warning Se ocorrer um erro ao criar o arquivo, a função imprime uma mensagem de erro
 * na tela e retorna um ponteiro nulo.
 *
 */
FILE *CriaLog(char nome[], char ext[]);

#endif