#ifndef LE_ARQUIVOS__
#define LE_ARQUIVOS__

#include <stdbool.h>

/*Conjunto de funções que são capazes de ler o arquivo e separar as palavras separadas por espaço
escritas nele*/

typedef void *ArqCmds;

/*Abre um arquivo com tipo não especificado para leitura,
portanto é necessário acrescentar a extensão do arquivo*/
ArqCmds abreArquivoCmd(char *fn);

/*Le uma linha do arquivo e atribui por referência ao ponteiro buf*/
bool leLinha (ArqCmds ac,char **buf);

/*Separa os parâmetros da linha delimtados por espaço em várias palavras 
e retorna o parâmetro de índice i*/
char* getParametroI(char *buf, int i);

/*Retorna todos os parâmetros depois de i
Ex: Casa Amarela Verde
i = 1
Retorna = Amarela Verde*/
char* getParametroDepoisI(char *buf, int i);

/*Fecha o arquivo aberto pela função abreArquivoCmd */
void fechaArquivoCmd(ArqCmds ac);

/*Versão do strdup para C99*/
char *my_strdup(const char *src);

#endif