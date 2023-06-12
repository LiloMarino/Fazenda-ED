#ifndef GEO_H
#define GEO_H

#include "Bibliotecas/arqsvg.h"
#include "radialtree.h"

#define TAM_VETOR 50 // Necessário para tirar os leaks de memória derivados das structs, define o tamanho de todos os arrays de caracteres

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

/**
 * @brief Verifica se a figura é um retângulo
 * @param Figura Ponteiro para a estrutura Figura que contém o tipo
 * @note Os parâmetros x1,y1,x2,y2 não são usados, visto que a função é somente para verificar o tipo do item
 * @return Retorna verdadeiro se a figura é equivalente a figura associada a função
 */
bool GetRetangulo(Info figura, double x1, double y1, double x2, double y2);

/**
 * @brief Verifica se a figura é um círculo
 * @param Figura Ponteiro para a estrutura Figura que contém o tipo
 * @note Os parâmetros x1,y1,x2,y2 não são usados, visto que a função é somente para verificar o tipo do item
 * @return Retorna verdadeiro se a figura é equivalente a figura associada a função
 */
bool GetCirculo(Info figura, double x1, double y1, double x2, double y2);

/**
 * @brief Verifica se a figura é uma linha
 * @param Figura Ponteiro para a estrutura Figura que contém o tipo
 * @note Os parâmetros x1,y1,x2,y2 não são usados, visto que a função é somente para verificar o tipo do item
 * @return Retorna verdadeiro se a figura é equivalente a figura associada a função
 */
bool GetLinha(Info figura, double x1, double y1, double x2, double y2);

/**
 * @brief Verifica se a figura é um texto
 * @param Figura Ponteiro para a estrutura Figura que contém o tipo
 * @note Os parâmetros x1,y1,x2,y2 não são usados, visto que a função é somente para verificar o tipo do item
 * @return Retorna verdadeiro se a figura é equivalente a figura associada a função
 */
bool GetTexto(Info figura, double x1, double y1, double x2, double y2);

/**
 * @brief Analisa o tipo de figura e faz o free()
 * @param figura Ponteiro para a estrutura Figura que contém o tipo
 */
void FreeFigura(Info figura);

/**
 * @note Esta função é feita apenas para fins de depuração 
 * @brief Ela analisa o tipo da figura e escreve um texto no svg mostrando o ID da figura junto com o seu tipo
 * @param fsvg Ponteiro para o arquivo SVG
 * @param info Ponteiro para a struct com a figura
 */
void MostraID(ArqSvg fsvg, Item info);

#endif