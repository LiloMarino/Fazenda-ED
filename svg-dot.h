#ifndef SVG_DOT_H
#define SVG_DOT_H

#include "radialtree.h"
#include "Bibliotecas/geradores.h"

/**
 * Função que lê o dado da estrutura específica e gera o SVG
 */

typedef void *ArqDot;

FILE* ARVORE = CriaLog("../logs/Arvore","dot");

/**
 * @brief Opera a árvore criando o SVG conforme o nome
 * @param nome Nome do arquivo SVG de saída
 * @param All Ponteiro para a árvore contendo os elementos do SVG
 */
void OperaSVG(char nome[], RadialTree All);

void InicializaDot(ArqDot fdot);

void LigaNo(ArqDot fdot, RadialTree All, Node pai, Node filho);

void MarcaNoRemovido(ArqDot fdot, RadialTree All, Node removido);

void TerminaDot(ArqDot fdot);

#endif