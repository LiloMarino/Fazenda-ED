#ifndef SVG_H
#define SVG_H

#include "radialtree.h"

/**
 * Função que lê o dado da estrutura específica e gera o SVG
 */

/**
 * @brief Opera a árvore criando o SVG conforme o nome
 * @param nome Nome do arquivo SVG de saída
 * @param All Ponteiro para a árvore contendo os elementos do SVG
 */
void OperaSVG(char nome[], RadialTree All);

/**
 * @brief Cria frames de cada ação do .qry em uma pasta
 * @param All Ponteiro para a árvore contendo os elementos do SVG
 * @param OutputGeoQry Caminho da saída do .qry
 * @param num Endereço para número do frame
 */
void CriaFrame(RadialTree All,char *OutputGeoQry, int *num);

#endif