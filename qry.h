#ifndef QRY_H
#define QRY_H

#include "radialtree.h"
#include "Bibliotecas/listadupla.h"
#include <stdio.h>

/*
 * Conjunto de funções que interpretam arquivos .qry e realizam operações nas figuras lidas pelos arquivos .geo
 */

typedef void *ArqQry;

/*========================================================================================================== *
 * Funções Principais                                                                                        *
 *========================================================================================================== */

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
 * @param All Endereço para a árvore radial com todos os elementos
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

void Praga(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree *All);

void Cura(double x, double y, double largura, double altura, double raio, Lista Afetados);

void Aduba(double x, double y, double largura, double altura, double raio, Lista Afetados);

void Semeia(double x, double y, double largura, double altura, double fator, double dx, double dy, int ID);

/**
 * @brief Reporta os dados da figura identificado por ID no arquivo de registro
 * @param ID ID da figura a ser reportada
 * @param All Ponteiro para a árvore radial com todos os elementos
 * @param log Ponteiro para o arquivo de registro
 */
void DadosI(int ID, RadialTree All, FILE *log);

void InfoColheitadeiras(Lista Entidades, FILE *log, RadialTree All);

/**
 * @brief Fecha o arquivo fqry
 * @param fqry Ponteiro para o arquivo QRY abertoi
 */
void fechaQry(ArqQry fqry);

/*========================================================================================================== *
 * Funções Auxiliares                                                                                        *
 *========================================================================================================== */

/**
 * @brief Função do tipo FvisitaNo que é utilizada para procurar na árvore um ID especificado e guardar suas informações em aux
 * @param i Conteúdo do nó atual
 * @param x Coordenada x do nó atual
 * @param y Coordenada y do nó atual
 * @param aux Estrutura que guarda as informações do nó que contenha o ID especificado
 */
void VerificaID(Info i, double x, double y, void *aux);

/**
 * @brief Procura na árvore o ID especificado
 * @param ID ID a ser procurado na árvore
 * @param All Ponteiro para a árvore radial
 * @return Retorna informações sobre o nó como coordenadas do nó e seu conteúdo em uma estrutura do tipo ProcID
 * @warning É necessário dar free() na variável retornada por essa função
 */
Info ProcuraID(int ID, RadialTree All);

/**
 * @brief Colhe os elementos na área e remove os nós da árvore sem remover a informação do nó e insere na lista colheita apenas as hortaliças
 * @param All Endereço para a árvore radial com todos os elementos
 * @param Entidades Lista contendo todas as entidades
 * @param Colheita Lista contendo todos os objetos colhidos
 * @param Xinicio Coordenada de início da área de colheita
 * @param Yinicio Coordenada de início da área de colheita
 * @param Xfim Coordenada de fim da área de colheita
 * @param Yfim Coordenada de fim da área de colheita
 */
void ColheElementos(RadialTree *All, Lista Entidades, Lista Colheita, double Xinicio, double Yinicio, double Xfim, double Yfim);

/**
 * @brief Contabiliza os elementos colhidos com base na Lista Colheita e os reporta no arquivo log
 * @param Colheita Lista contendo todos os objetos colhidos
 * @param log Ponteiro para o arquivo de registro
 */
void ContabilizaColheita(Lista Colheita, FILE *log);

/**
 * @brief Obtém um ID único para entidades que não pertencem a árvore a partir do ID especificado
 * @param Entidades Lista contendo todas as entidades
 * @param ID ID especificado para ser único se houver um igual somará +1 e verificará novamente até ser único
 * @return Retorna um ID único baseado nos IDs existentes na lista de entidades
 * @note Esta função é necessária para que não haja conflitos entre entidades e objetos na árvore
 */
int GetIDUnico(Lista Entidades, int ID);

/**
 * @brief Cria o retângulo de colheita/praga/adubo e o insere tanto na árvore quanto na lista de entidades
 * @param All Ponteiro para a árvore radial
 * @param Entidades Lista contendo todas as entidades
 * @param Xinicio Coordenada de início da área de colheita
 * @param Yinicio Coordenada de início da área de colheita
 * @param Xfim Coordenada de fim da área de colheita
 * @param Yfim Coordenada de fim da área de colheita
 */
void CriaArea(RadialTree All, Lista Entidades, double Xinicio, double Yinicio, double Xfim, double Yfim);

/**
 * @brief Faz o free() para a estrutura de entidades
 * @param Ent Ponteiro para uma estrutura do tipo Entidade
 */
void FreeEntidade(void *Ent);

/**
 * @brief Faz o free() para a estrutura de hortaliça
 * @param Hor Ponteiro para uma estrutura do tipo Hortalica
 */
void FreeHortalica(void *Hor);

/**
 * @brief Função do tipo FvisitaNo que é utilizada para procurar na árvore os objetos atingidos e inserí-los numa lista
 * @param i Conteúdo do nó atual
 * @param x Coordenada x do nó atual
 * @param y Coordenada y do nó atual
 * @param aux Estrutura que guarda as informações da área atingida e o ponteiro para a lista
 */
void ObjetoAtingido(Info i, double x, double y, void *aux);

/**
 * @brief Verifica se determinado objeto foi atingido pela área
 * @param i Conteúdo do nó atual
 * @param aux Estrutura que guarda as informações da área atingida e o ponteiro para a lista
 * @return Retorna verdadeiro caso tenha sido atingido e falso caso não tenha
 */
bool VerificaAtingido(Info i, void *aux);

double CalculaAreaAfetada(void *Fig, void *Afeta);

double CalculaAreaIntersecaoRetanguloRetangulo(void *Ret, void *Afeta);

double CalculaAreaIntersecaoCirculoRetangulo(void *Circ, void *Afeta);

void CriaMarcacaoCircular(RadialTree All, Lista Entidades, double x, double y, double raio, char corb []);

#endif