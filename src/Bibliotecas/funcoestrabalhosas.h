/**
 * @brief Dada um retangulo Fig calcula a área de intersecção entre do retângulo e região delimitada por Afeta
 * @param Ret Ponteiro para a struct do tipo Retangulo
 * @param Afeta Ponteiro para a struct do tipo ProcAfetado
 * @return Retorna a área de intersecção
 */
double CalculaAreaIntersecaoRetanguloRetangulo(void *Ret, void *Afeta);

/**
 * @brief Dada um circulo Circ calcula a área de intersecção entre do círculo e região delimitada por Afeta
 * @param Circ Ponteiro para a struct do tipo Circulo
 * @param Afeta Ponteiro para a struct do tipo ProcAfetado
 * @return Retorna a área de intersecção
 */
double CalculaAreaIntersecaoCirculoRetangulo(void *Circ, void *Afeta);

/**
 * @brief Função do tipo FvisitaNo que é utilizada para procurar na árvore os objetos atingidos e inserí-los numa lista
 * @param i Conteúdo do nó atual
 * @param x Coordenada x do nó atual
 * @param y Coordenada y do nó atual
 * @param aux Estrutura que guarda as informações da área atingida e o ponteiro para a lista
 */
void ObjetoAtingido(Info i, double x, double y, void *aux);

/**
 * @brief Verifica se determinado objeto foi atingido pela área
 * @param i Conteúdo do nó atual
 * @param aux Estrutura que guarda as informações da área atingida e o ponteiro para a lista
 * @return Retorna verdadeiro caso tenha sido atingido e falso caso não tenha
 */
bool VerificaAtingido(Info i, void *aux);