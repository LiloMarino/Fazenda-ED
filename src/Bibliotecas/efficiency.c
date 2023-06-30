#include "efficiency.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>

static struct rusage inicio, fim;
struct timeval tempo_inicio, tempo_fim;

void iniciarTempo()
{
    gettimeofday(&tempo_inicio, NULL);
}

double finalizarTempo()
{
    gettimeofday(&tempo_fim, NULL);

    double segundos = (double)(tempo_fim.tv_sec - tempo_inicio.tv_sec) +
                      (double)(tempo_fim.tv_usec - tempo_inicio.tv_usec) / 1000000;

    //printf("\nTempo total de execução: %lf segundos.\n", segundos);
    return segundos;
}

long double calcularMemoriaUtilizada()
{
    getrusage(RUSAGE_SELF, &fim);

    //printf("\nMemória utilizada: %ld bytes.\n", fim.ru_maxrss);
    return fim.ru_maxrss;
}

double calcularTempoCPU()
{
    getrusage(RUSAGE_SELF, &fim);

    double tempo_cpu = (double)(fim.ru_utime.tv_sec - inicio.ru_utime.tv_sec) +
                       (double)(fim.ru_utime.tv_usec - inicio.ru_utime.tv_usec) / 1000000;

    //printf("\nTempo de CPU utilizado: %lf segundos.\n", tempo_cpu);
    return tempo_cpu;
}