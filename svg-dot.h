#ifndef SVG_DOT_H
#define SVG_DOT_H

#include "radialtree.h"
#include "Bibliotecas/geradores.h"

/**
 * Função que lê o dado da estrutura específica e gera o SVG
 */

typedef void *ArqDot;

extern FILE* ARQDOT;

/**
 * @brief Opera a árvore criando o SVG conforme o nome
 * @param nome Nome do arquivo SVG de saída
 * @param All Ponteiro para a árvore contendo os elementos do SVG
 */
void OperaSVG(char nome[], RadialTree All);

/**
 * @brief Inicializa o arquivo .dot já aberto
 * @param fdot Ponteiro para o arquivo .dot
 */
void InicializaDot(ArqDot fdot);

/**
 * @brief Termina e fecha o arquivo .dot
 * @param fdot Ponteiro para o arquivo .dot
 */
void TerminaDot(ArqDot fdot);

/**
 * @brief Liga o Pai->Filho
 * @param fdot Ponteiro para o arquivo .dot
 * @param All Ponteiro para a árvore radial
 * @param pai Ponteiro para o pai
 * @param filho Ponteiro para o filho
 */
void LigaNo(ArqDot fdot, RadialTree All, Node pai, Node filho);

/**
 * @brief Marca o nó como removido no .dot
 * @param fdot Ponteiro para o arquivo .dot
 * @param All Ponteiro para a árvore radial
 * @param removido Ponteiro para o nó removido
 */
void MarcaNoRemovido(ArqDot fdot, RadialTree All, Node removido);

void CriaPngDot(const char nome[]);

#endif