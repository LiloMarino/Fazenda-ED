#ifndef QRY_H
#define QRY_H

#include "radialtree.h"

/*
 * Conjunto de funções que interpretam arquivos .qry e realizam operações nas figuras lidas pelos arquivos .geo
 */

typedef void *ArqQry;

/**
 * @brief Cria o arquivo "fn" para escrita. Substitui o arquivo, caso exista.
 * @note Retorna NULL, caso nao tenha sido possivel criar arquivo.
 * @param fn Nome do arquivo a ser aberto
 * @return Retorna o ponteiro para o arquivo aberto
 */
ArqQry abreLeituraQry(char *fn);

/**
 * @brief Fecha o arquivo fqry
 * @param fqry Ponteiro para o arquivo QRY aberto
 */
void fechaQry(ArqQry fqry);

#endif