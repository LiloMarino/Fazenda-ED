#ifndef _RADIAL_TREE__
#define _RADIAL_TREE__

#include "Bibliotecas/listadupla.h"
#include <stdbool.h>

/*
 * Uma Arvore Radial e' uma arvore n-aria, espacial, nao balanceada.
 * A cada no' r da Arvore e' associado um ponto de 2 dimensoes (r.x e r.y) denominado ancora
 * do no' e uma informacao relacionada tal ponto.
 * Um no' divide o plano simetricamente em n regioes (0..n-1), tambem denomindadas setores,
 * centradas no ponto (r.x e r.y),  cada regiao associada a sua respectiva subarvore.
 * Cada setor Si e' delimitado por duas semi-retas, Rinf e Rsup, com origem na ancora e
 * com inclinacao, respectivamente:
 *
 *      InclinacaoRetaInf = i * 360/n
 *      InclinacaoRetaSup = (i + 1) * 360/n
 *
 * e compreende a regiao `a esquerda de Rinf e `a direita de Rsup, orientadas da ancora
 * para infinito.
 *
 * A arvore Radial implementa 2 percursos: em largura e em profundidade.
 * Tais percursos recebem como parametro uma funcao que processa a
 * informacao associada ao no' visitado.
 *
 * Tambem sao providas operacoes de selecao espacial:
 *   A PRIMEIRA seleciona os nos da arvore cujas ancoras
 * sao internas a uma regiao retangular.
 *   A SEGUNDA seleciona os nos da Arvore cujas informacoes associadas
 * podem ser consideradas internas a uma regiao retangular.
 * Esta operacao recebe por parametro uma funcao que decide se
 * tal informacao e' considerada (ou nao) interna.
 *   A TERCEIRA, similarmente, seleciona os nos da arvore cujo um dado
 * ponto e' interno 'a informacao associada ao no'.
 *
 * Nos removidos nao sao desalocados imediatamente, mas apenas
 * marcados como delidos.
 * Eventualmente, uma sub-arvore e' reorganizada sem os nos marcados como removidos.
 * Isto ocorre, imediatamente apos o no ser marcado como removido,  quando o fator de
 * degradacao fd da sub-arvore exceder a um dado limiar definido na criacao da arvore.
 *
 *     fd = #removidos/#total_nos > limiar.
 *
 */

typedef void *RadialTree;
typedef void *Node;
typedef void *Info;

/*
 * Uma funcao deste tipo deve retornar verdadeiro se a informacao i esta'
 * "dentro" da regiao retangular delimitada pelos pontos opostos (x1,y1) e (x2,y2).
 * Retorna falso, caso contrario.
 */
typedef bool (*FdentroDeRegiao)(Info i, double x1, double y1, double x2, double y2);

/*
 * Uma funcao deste tipo deve retornar verdadeiro se o ponto (x,y)
 * deva ser considerado
 * "interno" da informacao i.
 */
typedef bool (*FpontoInternoAInfo)(Info i, double x, double y);

/*
 * Processa a informacao i associada a um no' da arvore, cuja ancora
 * e' o ponto (x,y). O parametro aux aponta para conjunto de dados
 * (provavelmente um registro) que sao compartilhados entre as
 * sucessivas invocacoes a esta funcao.
 */
typedef void (*FvisitaNo)(Info i, double x, double y, void *aux);

/*
 * Verifica se a informacao i associada a um no' da arvore, cuja ancora
 * e' o ponto (x,y) e' a informacao procurada. Retorna verdadeiro, em caso
 * afirmativo; falso, caso contrário. O parametro aux aponta para conjunto de dados
 * (provavelmente um registro) que sao compartilhados entre as
 * sucessivas invocacoes a esta funcao, incluindo (provavelmente) uma chave de busca.
 */
typedef bool (*FsearchNo)(Info i, double x, double y, void *aux);

/*
 * Retorna uma arvore Radial vazia de numSetores setores e com fator
 * de degradacao fd.
 *    0 <= fd < 1.0
 */
RadialTree newRadialTree(int numSetores, double fd);

/*
 * Insere a informacao i, associada 'a ancora (x,y) na arvore t.
 * Retorna um indicador para o no' inserido.
 */
Node insertRadialT(RadialTree t, double x, double y, Info i);

/*
 * Retorna o no' cuja ancora seja o ponto (x,y). Aceita-se uma pequena discrepancia
 * entre a coordenada da ancora (anc.x,anc.y) e o ponto (x,y) de epsilon unidades.
 * Ou seja, |anc.x - x | < epsilon e |anc.y - y | < epsilon.
 * Retorna NULL caso nao tenha encontrado o no'.
 */
Node getNodeRadialT(RadialTree t, double x, double y, double epsilon);

/*
 * Marca como removido o no' n. Caso, apos a remocao, o fator de degradacao
 * superar o limiar definido na criacao, a arvore e' recriada sem os nos delidos.
 */
void removeNoRadialT(RadialTree t, Node n);

/* Retorna a informacao associada ao no' n */
Info getInfoRadialT(RadialTree t, Node n);

/* Insere na lista L os nos (Node) da arvore t cujas ancoras estao dentro da regiao delimitada pelos
   pontos (x1,y1) e (x2,y2).
   Retorna falso, caso nao existam nos dentro da regiao; verdadeiro, caso contrario.
 */
bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L);

/* Insere na lista L os nos cujas respectivas informacoes associadas estao dentro da regiao
   delimitada pelos pontos (x1,y1) e (x2,y2). A funcao f e' usada para determinar se uma informacao
   armazenada na arvore esta' dentro da regiao.
   Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */
bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2,
                                 FdentroDeRegiao f, Lista L);

/* Insere na lista L  os nos para os quais o ponto (x,y) possa ser considerado
  interno 'as  informacoes associadas ao no'. A funcao f e' invocada para determinar
  se o ponto (x,y) e' interno a uma informacao especifica.
  Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */
bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L);

/* Percorre a arvore em profundidade. Invoca a funcao f em cada no visitado.
   O apontador aux e' parametro em cada invocacao de f; assim alguns
   dados podem ser compartilhados entre as diversas invocacoes de f.
 */
void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux);

/* Similar a visitaProfundidadeRadialT, porem, faz o percurso em largura.
 */
void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux);

/* Procura o no' da arvore que contenha um dado especifico.
   Visita cada no' da arvore e invoca a funcao f. A funcao f
   retornara' verdadeiro se o no' contem o dado procurado.
   Neste caso, retorna o no' encontrado. Caso a busca falhe,
   retorna NULL.
 */
Node procuraNoRadialT(RadialTree t, FsearchNo f, void *aux);

/* Gera representacao da arvore no arquivo fn, usando a Dot Language
   (ver https://graphviz.org/). Retorna falso, caso o arquivo nao possa
   ser criado (para escrita); true, caso contrario)
*/
bool printDotRadialTree(RadialTree t, char *fn);

/* Libera a memoria usada pela arvore t.
 */
void killRadialTree(RadialTree t);

#endif