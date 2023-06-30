#ifndef EFFICIENCY_H
#define EFFICIENCY_H

#include <sys/resource.h>

/**
 * @brief Função que inicia a contagem de tempo.
 * 
 * @note Essa função usa a função gettimeofday() da biblioteca <sys/time.h>.
 * 
 */
void iniciarTempo();

/**
 * @brief Finaliza a medição do tempo de execução e exibe o tempo total em segundos.
 * 
 * @return O tempo total de execução em segundos.
 * 
 * @note Para utilizar esta função corretamente, é necessário ter chamado a função iniciarTempo() antes.
 * 
 * Esta função utiliza a função gettimeofday() para obter a hora atual do sistema. Ela subtrai a hora de início 
 * armazenada globalmente pela hora de término, calculando o tempo total de execução em segundos.
 * O tempo total é exibido na saída padrão e retornado como um valor double.
 * 
 */
double finalizarTempo();

/**
 * @brief Calcula a quantidade de memória utilizada pelo programa.
 * 
 * @return A quantidade de memória utilizada pelo programa em bytes.
 * 
 * @note Essa função utiliza a função getrusage() para obter informações sobre o uso de recursos do programa.
 * 
 */
long double calcularMemoriaUtilizada();

/**
 * @brief Calcula o tempo de CPU utilizado pelo programa.
 * 
 * @return O tempo de CPU utilizado pelo programa em segundos.
 * 
 * @note Essa função utiliza a função getrusage() para obter informações sobre o uso de recursos do programa.
 * 
 */
double calcularTempoCPU();

#endif
