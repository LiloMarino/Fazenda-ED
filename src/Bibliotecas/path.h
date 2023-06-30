#ifndef PATH__
#define PATH__

/*
   Este modulo prove funcoes para manipulacao de caminhos de arquivos.
   Basicamente, exitem 2 tipos de funcoes:

      - separam caminho em seus componentes: splitPath, getFileName, getPath
      - constroem caminho a partir de seus componentes: joinFilePath,
          joinAll, normalizePath.

   Um caminho completo (fullPath) compreende uma sequencia de subdiretorios(path),
   o nome do arquivo e sua extensao.
   O caminho (path) e´ toda a subcadeia que esta' esquerda do ultimo separador ´/´,
   incluindo-o. A extensao é a subcadeia que esta' `a direita do ultimo
   separador '.', incluindo-o. O nome do arquivo é a subcadeia que esta´ entre
   estes dois últimos.

   Nesta operacoes, um caminho vazio ou um componente vazio e´ representado
   por uma string vazia.

  Os resultados das operacoes sao atribuidos a paramentros da funcao.
  Este parametros sao vetores de caracteres pre-alocados. A cada um destes
  parametros, existe um respectivo parametro que indica seu tamanho (em caracteres).

 */

/*
    Dado um caminho (fullPath), copia cada um de seus componentes
    nos parametros path (caminho do arquivo), nomeArq (nome do arquivo, sem
    seu path e sem sua extensao) e extArq (extensao do arquivo).
    Estes parametros sao vetores de tamanhos, respectivamente, lenPath,
    lenNomeArq e lenExtArq. Exemplos:

    fullPath               path        nomeArqq   extArq
    =====================================================
    "aaa/bbb/ccc.txt" =>   "aaa/bbb"   "ccc"      ".txt"
    "ccc.txt"         =>   ""          "ccc"      ".txt"
    ".txt"            =>   ""          ""         ".txt"
    "ccc.ddd.txt"     =>   ""          "ccc.ddd"  ".txt"
    "ccc"             =>   ""          "ccc"      ""
 */
void splitPath(const char *fullPath, char **path, char **nomeArq, char **extArq);

/*
  Dado um caminho (path) e um nome de arquivo (possivelmente, com sua extensao),
  copia para fullPath o caminho completo do arquivo. Exemplos:
   path         fileName =>  fullPath
   =============================================
   "aaa/bbb"    "c.txt"  =>  "aaa/bbb/c.txt"
   "aaa/bbb/"   "c.txt"  =>  "aaa/bbb/c.txt"
   ""           "c.txt"  =>  "c.txt"
   "aaa/bbb"    "c"      =>  "aaa/bbb/c"
 */
void joinFilePath(const char *path, const char *fileName, char **fullPath);

/*
   Semelhante a joinFilePath, porem, a extensao do arquivo e´ informado explicitamente
   pelo parametro ext. Exemplos:

   path       fileName  ext     => fullPath
   =================================================
   "aaa/bbb"  "ccc"     ".txt"  => "aaa/bbb/ccc.txt"
   "aaa/bbb/  "ccc"     ".txt"  => "aaa/bbb/ccc.txt"
   ""         "ccc"     ".txt"  => "ccc.txt"
   "aaa/bbb"  "ccc"     ""      => "aaa/bbb/ccc"
 */
void joinAll(const char *path, const char *fileName, const char *ext, char **fullPath);

/*
    Dado um caminho completo (fullPath), copia em fileName o nome do arquivo
    (possivelmente) com sua extensao. Exemplos:

    fullPath          => fileName
    ==============================
    "aaa/bb/cc.txt"   => "cc.txt"
    "cc.txt"          => "cc.txt"
    "cc."             => "cc."
    "cc"              => "cc"
    ".txt"            => ".txt"
 */
void getFileName(const char *fullPath, char **fileName);

/*
   Dado um caminho completo, retorna seu respectivo path.

   fullPath           => path
   ================================
   "aaa/bbb/ccc.txt"  => "aab/bbb"
   "ccc.txt"          => ""
   "aaa/bbb/ccc"      => "aaa/bbb"
 */
void getPath(const char *fullPath, char **path);

/*
   Dado um caminho (path), (supostamente) sem nome de arquivo,
   copia em normPath o caminho, sem o separador do final (se existir).

   path         => normPath
   ==========================
   "aa/bb/cc"   => "aa/bb/cc"
   "aa/bb/cc/"  => "aa/bb/cc"
 */
void normalizePath(const char *path, char **normPath);

/**
 * @brief Remove a extensão de um arquivo por exemplo arq.geo -> arq
 * @param fileName Nome do arquivo com a extensão
 * @return Retorna o nome sem a extensão
 */
char *RemoveExtensao(const char *fileName);

/**
 * @brief Concatena NomeGeo com NomeQry ficando separando-os pelo caractere "-" ficando NomeGeo-NomeQry
 * @param NomeGeo Nome do arquivo .geo
 * @param NomeQry Nome do arquivo .qry
 * @return Retorna o nome concatenado dos 2
 */
char *ConcatenaNomes(const char *NomeGeo, const char *NomeQry);

/**
 * @brief Recebe os argumentos e interpreta a entrada
 * @param PathInput Local onde contém os arquivos a serem lidos
 * @param PathOutput Local onde será criado os arquivos de saída
 * @param nomeGeo Nome do arquivo .geo
 * @param nomeQry Nome do arquivo .qry
 * @param numSetor Número de setores da árvore radial
 * @param fator Fator de degradação da árvore radial
 * @param argc Argc
 * @param argv Argv
 */
void ArgumentosDeComando(char **PathInput, char **PathOutput, char **nomeGeo, char **nomeQry, char **numSetor, char **fator, int argc, char **argv);

#endif