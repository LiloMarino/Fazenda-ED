#include "utilities.h"
#include <stdio.h>
#include <math.h>

double GrausParaRadianos(double Graus)
{
    double Radianos = Graus * (PI / 180.0);
    return Radianos;
}

double RadianosParaGraus(double Radianos)
{
    double Graus = Radianos * (180.0 / PI);
    return Graus;
}

bool VerificaIntervalo(double Inicio, double P, double Fim)
{
    return (Inicio <= P) && (P <= Fim);
}

bool VerificaPonto(double Axsup, double Px, double Axinf, double Aysup, double Py, double Ayinf)
{
    return (Axsup <= Px && Px <= Axinf && Ayinf <= Py && Py <= Aysup);
}

double Distancia2Pontos(double x1, double y1, double x2, double y2)
{
    double distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return distancia;
}

void BarraDeProgresso(int i, int total)
{
    // Calcula a porcentagem de conclusão
    float porcentagem = (float)i / total * 100;

    // Calcula a largura da barra
    int largura = 100;

    // Calcula o número de caracteres '=' na barra
    int num_igual = (int)(porcentagem / 100 * largura);

    // Imprime a barra
    printf("[");
    for (int j = 0; j < largura; j++)
    {
        if (j < num_igual)
        {
            printf("=");
        }
        else
        {
            printf(" ");
        }
    }
    printf("] %3.0f%%\r", porcentagem);

    // Limpa o buffer de saída para atualizar a barra
    fflush(stdout);
}

void BarraDeProgressoETA(int i, int total, time_t inicio)
{
    // Calcula a porcentagem de conclusão
    float porcentagem = (float)i / total * 100;

    // Calcula a largura da barra
    int largura = 100;

    // Calcula o número de caracteres '=' na barra
    int num_igual = (int)(porcentagem / 100 * largura);

    // Calcula o tempo decorrido desde o início do processo
    time_t agora = time(NULL);
    time_t tempo_decorrido = agora - inicio;

    // Calcula o tempo estimado de conclusão (ETA)
    time_t ETA;
    if (i == 0)
    {
        ETA = ((total - i) * tempo_decorrido / 1);
    }
    else
    {
        ETA = ((total - i) * tempo_decorrido / i);
    }

    // Formata o tempo restante em uma string legível
    char str_tempo_restante[30];
    if (ETA < 60)
    {
        sprintf(str_tempo_restante, "%ld segundos", ETA);
    }
    else if (ETA < 3600)
    {
        double minutos = ETA / 60.0;
        sprintf(str_tempo_restante, "%.1f minutos", minutos);
    }
    else
    {
        double horas = ETA / 3600.0;
        sprintf(str_tempo_restante, "%.1f horas", horas);
    }

    // Imprime a barra de progresso e o tempo restante
    printf("[");
    for (int j = 0; j < largura; j++)
    {
        if (j < num_igual)
        {
            printf("=");
        }
        else
        {
            printf(" ");
        }
    }
    printf("] %3.2f%% ETA: %s\r", porcentagem, str_tempo_restante);

    // Limpa o buffer de saída para atualizar a barra
    fflush(stdout);
}
