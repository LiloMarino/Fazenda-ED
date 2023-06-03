#ifndef QRY_H
#define QRY_H

#include "radialtree.h"
#include <stdio.h>

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
 * @brief A função recebe um arquivo .qry interpreta e opera os elementos na árvore radial
 * @param fqry Ponteiro para o arquivo QRY aberto
 * @param All Endereço para a árvore radial com todos os elementos
 * @param log Ponteiro para o arquivo de registro
 * @param PathOutput Ponteiro contendo o caminho de saída dos arquivos
 */
void InterpretaQry(ArqQry fqry, RadialTree *All, FILE *log, char *PathOutput);

void Harvest(int ID, int Passos, char Direcao, FILE *log);

void Move(Info I, double dx, double dy, FILE *log);

void Praga(double x, double y, double largura, double altura, double raio);

void Cura(double x, double y, double largura, double altura, double raio);

void Aduba(double x, double y, double largura, double altura, double raio);

void Semeia(double x, double y, double largura, double altura, double fator, double dx, double dy, int ID);

void DadosI(int ID, RadialTree All, FILE *log);

void InfoColheitadeiras();

/**
 * @brief Fecha o arquivo fqry
 * @param fqry Ponteiro para o arquivo QRY aberto
 */
void fechaQry(ArqQry fqry);

#endif