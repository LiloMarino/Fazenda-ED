#include "learquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ArqCmds abreArquivoCmd(char *fn)
{
    ArqCmds ac;
    ac = fopen(fn, "r");
    return ac;
}

bool leLinha(ArqCmds ac, char **buf)
{
    if (*buf != NULL)
    {
        free(*buf);
        *buf = NULL;
    }
    char linha[300] = "\0";
    if (fgets(linha, 300, ac))
    {
        *buf = malloc((strlen(linha) + 1)*sizeof(char));
        strcpy(*buf, linha);
        return true;
    }
    else
    {
        return false;
    }
}

char* getParametroI(char *buf, int i)
{
    char *paramI = NULL;
    char *bufcopia;
    bufcopia = my_strdup(buf);
    paramI = strtok(bufcopia, " ");
    for (int j = 0; j < i; j++)
    {
        paramI = strtok(NULL, " ");
    }
    paramI = my_strdup(paramI);
    free(bufcopia);
    return paramI;
}

char* getParametroDepoisI(char *buf, int i)
{
    char *paramI = NULL;
    char *bufcopia = my_strdup(buf);
    for (int j = 0; j < i-1; j++)
    {
        if (j == 0)
        {
           paramI = strtok(bufcopia, " ");
        }
        paramI = strtok(NULL, " ");
    }
    paramI = strtok(NULL, "");
    return paramI;
}

char *my_strdup(const char *src) {
    char *dst = malloc(strlen(src) + 1); 
    if (dst != NULL) {
        strcpy(dst, src); 
    }
    return dst; 
}

void fechaArquivoCmd(ArqCmds ac)
{
    fclose(ac);
}