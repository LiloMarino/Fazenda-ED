#ifndef DEF_H
#define DEF_H

/*Definem a Área da árvore e o viewBox do SVG, e são usados tanto na procura de elementos quanto na elaboração do SVG*/
#define SIZE_X1_Y1 -100 /* (x1,y1) */
#define SIZE_X2_Y2 5000 /* (x2,y2) */

#define EPSILON_PADRAO 0.001 /* Epsilon padrão usado nas funções de busca */
#define RAIO_BASE 3.0 /* Raio padrão do círculo da função semeia */
#define DISPERCAO_MIN 1 /* Dispersão mínima da função de semear */
#define DISPERCAO_MAX 10 /* Dispersão máxima da função de semear */
#define SHOW_ID_FIGURA 0 /* 1 = True, 0 = False */
#define SKIP_IMAGE_GENERATION 0 /* 1 = True, 0 = False */
#define FRAME_GENERATION 1 /* 1 = True, 0 = False */

#endif