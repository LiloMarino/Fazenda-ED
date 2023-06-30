#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "path.h"
#include "learquivo.h"

void splitPath(const char *fullPath, char **path, char **nomeArq, char **extArq)
{
    if (*path != NULL)
    {
        free(*path);
        *path = NULL;
    }

    if (*nomeArq != NULL)
    {
        free(*nomeArq);
        *nomeArq = NULL;
    }

    if (*extArq != NULL)
    {
        free(*extArq);
        *extArq = NULL;
    }

    char *ultimabarra = strrchr(fullPath, '/');
    char *ultimoponto = strrchr(fullPath, '.');

    if (ultimabarra != NULL)
    {
        int tamanhoPath = ultimabarra - fullPath + 1;
        *path = malloc((tamanhoPath + 1) * sizeof(char));
        strncpy(*path, fullPath, tamanhoPath);
        (*path)[tamanhoPath] = '\0';
    }
    else
    {
        *path = my_strdup("");
    }

    if (ultimoponto != NULL)
    {
        *nomeArq = malloc((ultimoponto - ultimabarra) * sizeof(char));
        strncpy(*nomeArq, ultimabarra + 1, ultimoponto - ultimabarra - 1);
        (*nomeArq)[ultimoponto - ultimabarra - 1] = '\0';

        *extArq = my_strdup(ultimoponto);
    }
    else
    {
        if (ultimabarra != NULL)
        {
            *nomeArq = malloc((strlen(ultimabarra) + 1) * sizeof(char));
            strcpy(*nomeArq, ultimabarra + 1);
        }
        else
        {
            *nomeArq = my_strdup("");
        }
        *extArq = my_strdup("");
    }
}

void joinFilePath(const char *path, const char *fileName, char **fullPath)
{
    if (path == NULL || fileName == NULL)
    {
        return;
    }

    if (*fullPath != NULL)
    {
        free(*fullPath);
        *fullPath = NULL;
    }

    size_t pathLength = strlen(path);
    size_t fileNameLength = strlen(fileName);
    size_t fullPathLength = pathLength + fileNameLength + 2; // +2 para o caractere '/' e o terminador nulo

    *fullPath = malloc(fullPathLength * sizeof(char));

    if (pathLength > 0 && path[pathLength - 1] == '/')
    {
        snprintf(*fullPath, fullPathLength, "%s%s", path, fileName);
    }
    else
    {
        snprintf(*fullPath, fullPathLength, "%s/%s", path, fileName);
    }
}

void joinAll(const char *path, const char *fileName, const char *ext, char **fullPath)
{
    if (path == NULL || ext == NULL || fileName == NULL)
    {
        return;
    }

    if (*fullPath != NULL)
    {
        free(*fullPath);
        *fullPath = NULL;
    }

    int pathLength = strlen(path);
    int fileNameLength = strlen(fileName);
    int extLength = strlen(ext);
    int fullPathLength = pathLength + fileNameLength + extLength + 2;

    *fullPath = malloc(fullPathLength * sizeof(char));

    if (strcmp("", path) != 0)
    {
        if (path[pathLength - 1] == '/')
        {
            sprintf(*fullPath, "%s%s%s", path, fileName, ext);
        }
        else
        {
            sprintf(*fullPath, "%s/%s%s", path, fileName, ext);
        }
    }
    else
    {
        sprintf(*fullPath, "%s%s", fileName, ext);
    }
}

void getFileName(const char *fullPath, char **fileName)
{
    if (fullPath == NULL)
    {
        return;
    }

    if (*fileName != NULL)
    {
        free(*fileName);
        *fileName = NULL;
    }

    char *ultimabarra = strrchr(fullPath, '/');

    if (ultimabarra == NULL)
    {
        *fileName = my_strdup(fullPath);
    }
    else
    {
        *fileName = my_strdup(ultimabarra + 1);
    }
}

void getPath(const char *fullPath, char **path)
{
    if (fullPath == NULL)
    {
        return;
    }

    if (*path != NULL)
    {
        free(*path);
        *path = NULL;
    }

    char *ultimabarra = strrchr(fullPath, '/');

    if (ultimabarra != NULL)
    {
        int pathLength = ultimabarra - fullPath;
        *path = malloc((pathLength + 1) * sizeof(char));
        strncpy(*path, fullPath, pathLength);
        (*path)[pathLength] = '\0';
    }
    else
    {
        *path = my_strdup("");
    }
}

void normalizePath(const char *path, char **normPath)
{
    if (path == NULL)
    {
        return;
    }

    if (*normPath != NULL)
    {
        free(*normPath);
        *normPath = NULL;
    }

    int pathLength = strlen(path);

    if (path[pathLength - 1] == '/')
    {
        pathLength--;
    }

    *normPath = realloc(*normPath, (pathLength + 1) * sizeof(char));
    strncpy(*normPath, path, pathLength);
    (*normPath)[pathLength] = '\0';
}

char *RemoveExtensao(const char *fileName)
{
    char *nomeArq = NULL;

    // Procura a última ocorrência do caractere '.'
    const char *ultimoponto = strrchr(fileName, '.');

    if (ultimoponto != NULL)
    {
        // Calcula o tamanho do nome do arquivo sem a extensão
        size_t nomeArqLen = ultimoponto - fileName;
        
        // Aloca memória para o novo nome do arquivo
        nomeArq = malloc((nomeArqLen + 1) * sizeof(char));
        
        // Copia o nome do arquivo sem a extensão
        strncpy(nomeArq, fileName, nomeArqLen);
        nomeArq[nomeArqLen] = '\0';
    }
    else
    {
        // Se não há ponto, o nome do arquivo é o mesmo que o original
        nomeArq = my_strdup(fileName);
    }

    return nomeArq;
}

char *ConcatenaNomes(const char *NomeGeo, const char *NomeQry)
{
    if (NomeQry != NULL)
    {
        char *nome = calloc(strlen(NomeGeo) + strlen(NomeQry) + 1, sizeof(char));
        strcpy(nome, NomeGeo);
        strtok(nome, ".");
        strcat(nome, "-");
        char *Qry = calloc(strlen(NomeQry) + 1, sizeof(char));
        strcpy(Qry, NomeQry);
        strtok(Qry, ".");
        strcat(nome, Qry);
        free(Qry);
        return nome;
    }
    else
    {
        return NULL;
    }
}

void ArgumentosDeComando(char **PathInput, char **PathOutput, char **nomeGeo, char **nomeQry, char **numSetor, char **fator, int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-e", argv[i]) == 0 && i + 1 < argc)
        {
            *PathInput = argv[++i];
        }
        else if (strcmp("-f", argv[i]) == 0 && i + 1 < argc)
        {
            *nomeGeo = argv[++i];
        }
        else if (strcmp("-o", argv[i]) == 0 && i + 1 < argc)
        {
            *PathOutput = argv[++i];
        }
        else if (strcmp("-q", argv[i]) == 0 && i + 1 < argc)
        {
            *nomeQry = argv[++i];
        }
        else if (strcmp("-ns", argv[i]) == 0 && i + 1 < argc)
        {
            *numSetor = argv[++i];
        }
        else if (strcmp("-fd", argv[i]) == 0 && i + 1 < argc)
        {
            *fator = argv[++i];
        }
    }
}
