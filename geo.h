#ifndef GEO_H
#define GEO_H

#include "arqsvg.h"
#include "radialtree.h"

/*
 * Conjunto de funções que interpretam arquivos .geo e criam figuras svg a partir dele
 */

typedef void *ArqGeo;

/**
 * @brief Cria o arquivo "fn" para escrita. Substitui o arquivo, caso exista.
 * @note Retorna NULL, caso nao tenha sido possivel criar arquivo.
 * @param fn Nome do arquivo a ser aberto
 * @return Retorna o ponteiro para o arquivo aberto
 */
ArqGeo abreLeituraGeo(char *fn);

/**
 * @brief A função recebe um arquivo .geo interpreta e acrescenta os elementos na árvore radial
 * @param fgeo Ponteiro para o arquivo GEO
 * @param All Ponteiro para a árvore radial com todos os elementos
 */
void InterpretaGeo(ArqGeo fgeo, RadialTree All);

/**
 * @brief Fecha o arquivo fgeo.
 * @param fgeo Ponteiro para o arquivo GEO aberto
 */
void fechaGeo(ArqGeo fgeo);

/**
 * @brief Interpreta o item dado pela lista e cria no arquivo svg o retângulo
 * @param fsvg Ponteiro para o arquivo SVG
 * @param info Ponteiro para a struct com as informações do Retângulo
 */
void CriaRetanguloSvg(ArqSvg fsvg, Item info);

/**
 * @brief Interpreta o item dado pela lista e cria no arquivo svg o circulo
 * @param fsvg Ponteiro para o arquivo SVG
 * @param info Ponteiro para a struct com as informações do Circulo
 */
void CriaCirculoSvg(ArqSvg fsvg, Item info);

/**
 * @brief Interpreta o item dado pela lista e cria no arquivo svg a linha*
 * @param fsvg Ponteiro para o arquivo SVG
 * @param info Ponteiro para a struct com as informações da Linha
 */
void CriaLinhaSvg(ArqSvg fsvg, Item info);

/**
 * @brief Interpreta o item dado pela lista e cria no arquivo svg o texto
 * @param fsvg Ponteiro para o arquivo SVG
 * @param info Ponteiro para a struct com as informações do Texto
 */
void CriaTextoSvg(ArqSvg fsvg, Item info);

#endif