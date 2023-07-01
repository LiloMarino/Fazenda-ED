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
void InterpretaQry(ArqQry fqry, RadialTree *All, FILE *log);

/**
 * @brief Move a colheitadeira ID e colhe todos os objetos em sua trajetória
 * @param ID ID da colheitadeira
 * @param Passos Número de passos a ser dado pela colheitadeira
 * @param Direcao Direção dos passos
 * @param log Ponteiro para o arquivo de registro
 * @param Entidades Lista que contém todas as entidades
 * @param All Endereço para a árvore radial com todos os elementos
 * @param Colheita Lista contendo todos as hortaliças colhidas
 * @param Afetados Lista contendo todos as hortaliças afetadas
 */
void Harvest(int ID, int Passos, char Direcao, FILE *log, Lista Entidades, RadialTree *All, Lista Colheita, Lista Afetados);

/**
 * @brief Move uma figura identificada por ID
 * @param ID ID da figura a ser movida
 * @param dx Variação horizontal da figura
 * @param dy Variação vertical da figura
 * @param log Ponteiro para o arquivo de registro
 * @param All Endereço para a árvore radial com todos os elementos
 */
void Move(int ID, double dx, double dy, FILE *log, RadialTree *All);

/**
 * @brief Pragas de raio r são uniformemente distribuídas na região retangular
 * @param x Coordenada x da região retangular
 * @param y Coordenada y da região retangular
 * @param largura Largura da região retangular
 * @param altura Altura da região retangular
 * @param raio Raio dos círculos uniformemente distribuídos
 * @param Afetados Lista contendo todos as hortaliças afetadas
 * @param Entidades Lista que contém todas as entidades
 * @param All Endereço para a árvore radial com todos os elementos
 * @param log Ponteiro para o arquivo de registro
 */
void Praga(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree *All, FILE *log);

/**
 * @brief Cura as hortaliças na mesma proporção dos círculos de raio r do defensivo.
 * @param x Coordenada x da região retangular
 * @param y Coordenada y da região retangular
 * @param largura Largura da região retangular
 * @param altura Altura da região retangular
 * @param raio Raio dos círculos uniformemente distribuídos
 * @param Afetados Lista contendo todos as hortaliças afetadas
 * @param Entidades Lista que contém todas as entidades
 * @param All Ponteiro para a árvore radial com todos os elementos
 * @param log Ponteiro para o arquivo de registro
 */
void Cura(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree All, FILE *log);

/**
 * @brief Aduba as hortaliças da região Sempre que a área integral da hortaliça for atingida pelo adubo, a produtividade da hortaliça é incrementada em 10%.
 * @param x Coordenada x da região retangular
 * @param y Coordenada y da região retangular
 * @param largura Largura da região retangular
 * @param altura Altura da região retangular
 * @param raio Raio dos círculos uniformemente distribuídos
 * @param All Ponteiro para a árvore radial com todos os elementos
 * @param Entidades Lista que contém todas as entidades
 * @param Afetados Lista contendo todos as hortaliças afetadas
 * @param log Ponteiro para o arquivo de registro
 */
void Aduba(double x, double y, double largura, double altura, double raio, Lista Afetados, Lista Entidades, RadialTree All, FILE *log);

/**
 * @brief Dispersa as sementes das hortaliças que estão dentro da região (x,y,w,h), a um deslocamentode dx, dy, com fator f, com identificadores a partir de j.
 * @param x Coordenada x da região retangular
 * @param y Coordenada y da região retangular
 * @param largura Largura da região retangular
 * @param altura Altura da região retangular
 * @param fator Fator de multiplicação das hortaliças
 * @param dx Variação horizontal das hortaliças
 * @param dy Variação vertical das hortaliças
 * @param j ID do qual será contabilizado as hortaliças na nova área
 * @param All Ponteiro para a árvore radial com todos os elementos
 * @param Entidades Lista que contém todas as entidades
 * @param log Ponteiro para o arquivo de registro
 */
void Semeia(double x, double y, double largura, double altura, int fator, double dx, double dy, int j, Lista Entidades, RadialTree All, FILE *log);

/**
 * @brief Reporta os dados da figura identificado por ID no arquivo de registro
 * @param ID ID da figura a ser reportada
 * @param All Ponteiro para a árvore radial com todos os elementos
 * @param log Ponteiro para o arquivo de registro
 */
void DadosI(int ID, RadialTree All, FILE *log);

/**
 * @brief Reporta os atributos de todas as colheitadeiras
 * @param Entidades Lista que contém todas as entidades
 * @param log Ponteiro para o arquivo de registro
 * @param All Ponteiro para a árvore radial com todos os elementos
 */
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
 * @brief Colhe os elementos na área e remove os nós da árvore sem remover a informação do nó e insere na lista colheita apenas as hortaliças
 * @param All Endereço para a árvore radial com todos os elementos
 * @param Entidades Lista contendo todas as entidades
 * @param Afetados Lista contendo todos as hortaliças afetadas
 * @param Colheita Lista contendo todos as hortaliças colhidos
 * @param Xinicio Coordenada de início da área de colheita
 * @param Yinicio Coordenada de início da área de colheita
 * @param Xfim Coordenada de fim da área de colheita
 * @param Yfim Coordenada de fim da área de colheita
 * @param reporta Determina se a contabilidade é parcial ou total
 */
void ColheElementos(RadialTree *All, Lista Entidades, Lista Afetados, Lista Colheita, FILE *log, double Xinicio, double Yinicio, double Xfim, double Yfim, bool parcial);

/**
 * @brief Contabiliza os elementos colhidos com base na Lista Colheita e os reporta no arquivo log
 * @param Colheita Lista contendo todos os objetos colhidos
 * @param log Ponteiro para o arquivo de registro
 */
void ContabilizaColheita(Lista Colheita, FILE *log);

/**
 * @brief Cola os nós na nova área determinada por dx e dy
 * @param j ID que a partir dele as novas figuras serão enumeradas
 * @param dx Variação horizontal da área
 * @param dy Variação vertical da área
 * @param proporcao Fator de proporção
 * @param All Ponteiro para a árvore radial
 * @param Nos Lista contendo os nós copiados
 * @param Entidades Lista contendo as entidades
 * @param log Ponteiro para o arquvio de registro
 */
void Paste(int j, double dx, double dy, int proporcao, RadialTree All, Lista Nos, Lista Entidades, FILE *log);

/**
 * @brief Copia as figuras da árvore e as insere na lista TempEnt proporcao vezes
 * @param Fig Ponteiro para a struct do tipo Figura
 * @param j ID que a partir dele as novas figuras serão enumeradas
 * @param dx Variação horizontal das figuras
 * @param dy Variação vertical das figuras
 * @param proporcao Fator de proporção das figuras
 * @param TempEnt Lista de entidades temporária para a "cola" das figuras
 */
void Copy(void *Fig, int j, double dx, double dy, int proporcao, Lista TempEnt);

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
 * @brief Cria uma marcação circular nas coordenadas (x,y) com raio r
 * @param All Ponteiro para a árvore radial
 * @param Entidades Lista contendo todas as entidades
 * @param x Coordenada x do círculo
 * @param y Coordenada y do círculo
 * @param raio Raio do círculo
 * @param corb Cor da borda do círculo
 * @param corp Cor do preenchimento do círculo
 */
void CriaMarcacaoCircular(RadialTree All, Lista Entidades, double x, double y, double raio, char corb[], char corp[]);

/**
 * @brief Calcula a área afetada da figura pelas gotículas
 * @param Fig Ponteiro para a figura
 * @param MatrizGoticulas Ponteiro para a matriz de gotículas
 * @param numLinhas Número de linhas da matriz
 * @param numColunas Número de colunas da matriz
 * @return Retorna a área afetada
 */
double CalculaAreaAfetada(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas);

/**
 * @brief Calcula a área afetada da figura pelas gotículas
 * @param Fig Ponteiro para a figura de texto
 * @param MatrizGoticulas Ponteiro para a matriz de gotículas
 * @param numLinhas Número de linhas da matriz
 * @param numColunas Número de colunas da matriz
 * @return Retorna a área afetada
 */
double VerificaGoticulaTexto(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas);

/**
 * @brief Calcula a área afetada da figura pelas gotículas
 * @param Fig Ponteiro para a figura de círculo
 * @param MatrizGoticulas Ponteiro para a matriz de gotículas
 * @param numLinhas Número de linhas da matriz
 * @param numColunas Número de colunas da matriz
 * @return Retorna a área afetada
 */
double VerificaGoticulaCirculo(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas);

/**
 * @brief Calcula a área afetada da figura pelas gotículas
 * @param Fig Ponteiro para a figura de retângulo
 * @param MatrizGoticulas Ponteiro para a matriz de gotículas
 * @param numLinhas Número de linhas da matriz
 * @param numColunas Número de colunas da matriz
 * @return Retorna a área afetada
 */
double VerificaGoticulaRetangulo(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas);

/**
 * @brief Calcula a área afetada da figura pelas gotículas
 * @param Fig Ponteiro para a figura de linha
 * @param MatrizGoticulas Ponteiro para a matriz de gotículas
 * @param numLinhas Número de linhas da matriz
 * @param numColunas Número de colunas da matriz
 * @return Retorna a área afetada
 */
double VerificaGoticulaLinha(void *Fig, void *MatrizGoticulas, int numLinhas, int numColunas);

/**
 * @brief Cria uma matriz de gotículas circulares distribuidas uniformemente em uma área retangular
 * @param x Coordenada x da área retangular
 * @param y Coordenada y da área retangular
 * @param larg Largura da área retangular
 * @param alt Altura da área retangular
 * @param r Raio da gotícula
 * @param numLinhas Endereço para o número de linhas da matriz
 * @param numColunas Endereço para o número de colunas da matriz
 * @return Retorna o ponteiro para a matriz de gotículas
 */
void *CriaMatrizDeGoticulas(double x, double y, double larg, double alt, double r, int *numLinhas, int *numColunas);

/**
 * @brief Realiza o free() da matriz de gotículas
 * @param MatrizGoticulas Ponteiro para a matriz de gotículas
 * @param numLinhas Número de linhas da matriz
 */
void FreeMatrizDeGoticulas(void *MatrizGoticulas, int numLinhas);

/**
 * @brief Verifica se a gotícula está totalmente contida no retângulo
 * @param Goticula Ponteiro para a gotícula
 * @param Ret Ponteiro para o retângulo
 * @return Retorna verdadeiro se caso esteja e falso caso não esteja
 */
bool GoticulaContidaNoRetangulo(void *Goticula, void *Ret);

/**
 * @brief Verifica se a gotícula está totalmente contida no círculo
 * @param Goticula Ponteiro para a gotícula
 * @param Circ Ponteiro para o círculo
 * @return Retorna verdadeiro se caso esteja e falso caso não esteja
 */
bool GoticulaContidaNoCirculo(void *Goticula, void *Circ);

/**
 * @brief Verifica se a linha está totalmente contida na gotícula
 * @param Lin Ponteiro para a linha
 * @param Goticula Ponteiro para a gotícula
 * @return Retorna verdadeiro se caso esteja e falso caso não esteja
 */
bool LinhaContidaNaGoticula(void *Lin, void *Goticula);

/**
 * @brief Verifica se o texto está totalmente contido na gotícula
 * @param Txto Ponteiro para o texto
 * @param Goticula Ponteiro para a gotícula
 * @return Retorna verdadeiro se caso esteja e falso caso não esteja
 */
bool TextoContidoNaGoticula(void *Txto, void *Goticula);

/**
 * @brief Função do tipo FvisitaNo que é utilizada para procurar na árvore os objetos 100% atingidos e inserí-los numa lista
 * @param i Conteúdo do nó atual
 * @param x Coordenada x do nó atual
 * @param y Coordenada y do nó atual
 * @param aux Estrutura que guarda as informações da área atingida e o ponteiro para a lista
 */
void ObjetoTotalAtingido(Info i, double x, double y, void *aux);

/**
 * @brief Verifica se determinado objeto foi 100% atingido pela área
 * @param i Conteúdo do nó atual
 * @param aux Estrutura que guarda as informações da área atingida e o ponteiro para a lista
 * @return Retorna verdadeiro caso tenha sido atingido e falso caso não tenha
 */
bool VerificaTotalAtingido(Info i, void *aux);

/**
 * @brief Faz o "replace" de uma figura por um X vermelho
 * @param All Endereço para a árvore radial
 * @param Entidades Lista contendo todas as entidades
 * @param Afetados Lista contendo todos as hortaliças afetadas
 * @param Hor Ponteiro para a struct do tipo Hortalica
 */
void ReplaceWithRedX(RadialTree *All, Lista Entidades, Lista Afetados, void *Hor);

/**
 * @brief Cria um X vermelho e o insere na árvore em (x,y) e também nas entidades
 * @param All Ponteiro para árvore radial
 * @param Entidades Lista contendo todas as entidades
 * @param x Coordenada x do X vermelho
 * @param y Coordenada y do X vermelho
 */
void CriaXVermelho(RadialTree All, Lista Entidades, double x, double y);

/**
 * @brief Reporta os atributos de uma hortaliça
 * @param All Ponteiro para árvore radial
 * @param log Ponteiro para o arquivo de registro
 * @param Hor Ponteiro para uma struct do tipo Hortalica
 */
void ReportaHortalica(RadialTree All, FILE *log, void *Hor);

/**
 * @brief Função do tipo FsearchNo que é utilizada para procurar na árvore um ID especificado e guardar suas informações em aux
 * @param i Conteúdo do nó atual
 * @param x Coordenada x do nó atual
 * @param y Coordenada y do nó atual
 * @param aux Estrutura que guarda as informações do nó que contenha o ID especificado
 * @return Retorna verdadeiro caso tenha encontrado o nó e falso caso não tenha
 */
bool VerificaID(Info i, double x, double y, void *aux);

/**
 * @brief Procura na árvore o ID especificado
 * @param ID ID a ser procurado na árvore
 * @param All Ponteiro para a árvore radial
 * @return Retorna informações sobre o nó como coordenadas do nó e seu conteúdo em uma estrutura do tipo ProcID
 * @warning É necessário dar free() na variável retornada por essa função
 */
void *ProcuraID(int ID, RadialTree All);

/**
 * @brief Obtém um ID único para entidades que não pertencem a árvore a partir do ID especificado
 * @param Entidades Lista contendo todas as entidades
 * @param ID ID especificado para ser único se houver um igual somará +1 e verificará novamente até ser único
 * @return Retorna um ID único baseado nos IDs existentes na lista de entidades
 * @note Esta função é necessária para que não haja conflitos entre entidades e objetos na árvore
 */
int GetIDUnico(Lista Entidades, int ID);

/**
 * @brief Função do tipo Check que é utilizada na filter para filtrar os itens que nunca foram atingidos
 * @param item Item da lista a ser filtrado
 * @param aux Lista Afetados
 * @return Retorna falso se o item já havia sido atingido antes e verdadeiro caso o item nunca tenha sido atingido
 */
bool FiltraAtingidos(Item item, void *aux);

/**
 * @brief Função do tipo Check que é utilizada na filter para filtrar os itens que não são entidades
 * @param item Item da lista a ser filtrado
 * @param aux Lista Entidades
 * @return Retorna falso se o item é uma entidade conhecida e verdadeiro caso o item não seja uma entidade
 */
bool FiltraEntidades(Item item, void *aux);

/**
 * @brief Função do tipo Apply que é utilizada na map para criar uma nova lista, possibilitando a edição das hortaliças afetadas
 * @param item Item da lista a ser aplicado a função
 * @param aux Lista Afetados
 * @return Retorna o ponteiro para hortaliça presente na lista Afetados, caso não esteja presente na lista Afetados retorna NULL
 */
Item TransformaAtingidos(Item item, void *aux);

/**
 * @brief Transforma a lista de nós em uma lista de figuras
 * @param All Ponteiro para a árvore radial
 * @param Atingido Lista dos objetos atingidos
 * @warning Cuidado esta função elimina a lista passado como parâmetro
 * @return Retorna a lista contendo as figuras
 */
Lista TransformaLista(RadialTree All, Lista Atingido);

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

#endif