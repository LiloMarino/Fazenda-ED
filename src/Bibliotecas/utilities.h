#ifndef UTILITIES_H
#define UTILITIES_H

#include <time.h>
#include <stdbool.h>

#define PI 3.14159265358979323846

/**
 * @brief Recebe um valor em Graus e o retorna em Radianos
 * @param Graus Ângulo em Graus
 * @return Ângulo em Radianos
 */
double GrausParaRadianos(double Graus);

/**
 * @brief Recebe um valor em Radianos e o retorna em Graus
 * @param Radianos Ângulo em Radianos
 * @return Ângulo em Graus
 */
double RadianosParaGraus(double Radianos);

/**
 * @brief Verifica se um ponto está num determinado intervalo delimitado por 2 valores, ou seja se Inicio <= P <= Fim
 * @param Inicio Valor inicial
 * @param P Valor a ser verificado a pertencente do intervalo
 * @param Fim Valor final
 * @return Retorna verdadeiro se pertencer e falso caso não pertença
 */
bool VerificaIntervalo(double Inicio, double P, double Fim);

/**
 * @brief Verifica se um ponto pertence ou não a uma Área Retangular definida por Asup e Ainf, ou seja se 
 * Axsup <= Px <= Axinf e Ayinf <= Py <= Aysup
 * @param Axsup Coordenada x da Aresta Superior Esquerda
 * @param Px Coordenada x do ponto P
 * @param Axinf Coordenada x da Aresta Inferior Direita
 * @param Aysup Coordenada y da Aresta Superior Esquerda
 * @param Py Coordenada y do ponto P
 * @param Ayinf Coordenada y da Aresta Inferior Direita
 * @return Retorna verdadeiro se o ponto pertence a área e falso caso não pertença
 */
bool VerificaPonto(double Axsup, double Px, double Axinf, double Aysup, double Py, double Ayinf);

/**
 * @brief Calcula a distância entre 2 pontos
 * @param x1 Coordenada x do ponto 1
 * @param y1 Coordenada y do ponto 1
 * @param x2 Coordenada x do ponto 2
 * @param y2 Coordenada y do ponto 2
 * @return Retorna a distância entre o ponto 1 e o ponto 2
 */
double Distancia2Pontos(double x1, double y1, double x2, double y2);

/**
 * @brief Mostra uma barra de progresso baseada na % obtida pela divisão i/total
 * @param i Quantidade já feita
 * @param total Quantidade total a ser feita
 */
void BarraDeProgresso(int i, int total);

/**
 * @brief Mostra uma barra de progresso baseada na % obtida pela divisão i/total e é necessário antes do loop inicializar o tempo
 * @param i Quantidade já feita
 * @param total Quantidade total a ser feita
 * @param inicio Pode ser gerado a partir de time_t inicio = time(NULL);
 */
void BarraDeProgressoETA(int i, int total, time_t inicio);

#endif