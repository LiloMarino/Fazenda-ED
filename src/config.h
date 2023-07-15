#ifndef CONFIG_H
#define CONFIG_H

/*Definem a Área da árvore e o viewBox do SVG, e são usados na procura de elementos no final do qry */
#define SIZE_X1 -100 /* (x1,y1) */
#define SIZE_Y1 -100 /* (x1,y1) */
#define SIZE_X2 7000 /* (x2,y2) */
#define SIZE_Y2 3000 /* (x2,y2) */

#define EPSILON_PADRAO 0.001 /* Epsilon padrão usado nas funções de busca */
#define RAIO_BASE 3.0 /* Raio padrão do círculo da função semeia */
#define DISPERCAO_MIN 1 /* Dispersão mínima da função de semear */
#define DISPERCAO_MAX 10 /* Dispersão máxima da função de semear */
#define SHOW_ID_FIGURA 0 /* 1 = True, 0 = False, Diz se mostra os IDs da figura no SVG*/
#define SKIP_IMAGE_GENERATION 0 /* 1 = True, 0 = False, Pula a etapa de criação do png usando o .dot */
#define FRAME_GENERATION 0 /* 1 = True, 0 = False, Diz se gera os frames de cada ação do qry em uma pasta */
#define FINAL_DOT_ONLY 1 /* 1 = True, 0 = False, Se verdadeiro gera apenas o .dot contendo a imagem final da árvore, se falso gera os .dot cada vez que a árvore é recriada */

#endif