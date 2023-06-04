#ifndef QRY_H
#define QRY_H

#include "radialtree.h"
#include "Bibliotecas/listadupla.h"
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

/**
 * @brief Move a colheitadeira ID e colhe todos os objetos em sua trajetória
 * @param ID ID da colheitadeira
 * @param Passos Número de passos a ser dado pela colheitadeira
 * @param Direcao Direção dos passos
 * @param log Ponteiro para o arquivo de registro 
 * @param Entidades Lista que contém todas as informações de todas as colheitadeiras
 * @param All ndereço para a árvore radial com todos os elementos
 */
void Harvest(int ID, int Passos, char Direcao, FILE *log, Lista Entidades, RadialTree *All, Lista Colheita);

/**
 * @brief Move uma figura identificada por ID
 * @param ID ID da figura a ser movida
 * @param dx Variação horizontal da figura
 * @param dy Variação vertical da figura
 * @param log Ponteiro para o arquivo de registro 
 * @param All Endereço para a árvore radial com todos os elementos
 */
void Move(int ID, double dx, double dy, FILE *log, RadialTree *All);

void Praga(double x, double y, double largura, double altura, double raio);

void Cura(double x, double y, double largura, double altura, double raio);

void Aduba(double x, double y, double largura, double altura, double raio);

void Semeia(double x, double y, double largura, double altura, double fator, double dx, double dy, int ID);

/**
 * @brief Reporta os dados da figura identificado por ID no arquivo de registro
 * @param ID ID da figura a ser reportada
 * @param All Ponteiro para a árvore radial com todos os elementos
 * @param log Ponteiro para o arquivo de registro 
 */
void DadosI(int ID, RadialTree All, FILE *log);

void InfoColheitadeiras();

void ContabilizaColheita(Lista Colheita);

/**
 * @brief Fecha o arquivo fqry
 * @param fqry Ponteiro para o arquivo QRY aberto
 */
void fechaQry(ArqQry fqry);

#endif