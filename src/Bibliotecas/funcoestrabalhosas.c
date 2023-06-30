double CalculaAreaIntersecaoRetanguloRetangulo(void *Ret, void *Afeta)
{
    Retangulo *r = Ret;
    ProcAfetado *Af = Afeta;
    double intersecaoX = fmax(r->x, Af->x);
    double intersecaoY = fmax(r->y, Af->y);
    double intersecaoW = fmin(r->x + r->larg, Af->x + Af->larg) - intersecaoX;
    double intersecaoH = fmin(r->y + r->alt, Af->y + Af->alt) - intersecaoY;
    return intersecaoW * intersecaoH;
}

double CalculaAreaIntersecaoCirculoRetangulo(void *Circ, void *Afeta)
{
    Circulo *c = Circ;
    ProcAfetado *Af = Afeta;
    // Verificar se não há interseção entre o círculo e o retângulo
    if (c->x - c->raio > (Af->x + Af->larg) ||
        c->x + c->raio < Af->x ||
        c->y - c->raio > (Af->y + Af->alt) ||
        c->y + c->raio < Af->y)
    {
        return 0;
    }

    // Reduzir o retângulo para limitar à interseção
    double intersectionLeft = fmax(Af->x, c->x - c->raio);
    double intersectionTop = fmax(Af->y, c->y - c->raio);
    double intersectionRight = fmin((Af->x + Af->larg), c->x + c->raio);
    double intersectionBottom = fmin((Af->y + Af->alt), c->y + c->raio);

    // Calcular a área de interseção
    double intersectionWidth = intersectionRight - intersectionLeft;
    double intersectionHeight = intersectionBottom - intersectionTop;
    double intersectionArea = intersectionWidth * intersectionHeight;

    // Calcular a área do setor circular que está dentro da interseção
    double circleX = c->x - intersectionLeft;
    double circleY = c->y - intersectionTop;
    double circleAngle = atan2(circleY, circleX);
    double circleSectorArea = 0.5 * circleAngle * c->raio * c->raio;

    // Calcular a área de interseção final
    double finalIntersectionArea = intersectionArea - circleSectorArea;

    return finalIntersectionArea;
}

void ObjetoAtingido(Info i, double x, double y, void *aux)
{
    ProcAfetado *A = aux;
    Lista Atingido = A->Atingido;
    if (VerificaAtingido(i, aux))
    {
        insertLst(Atingido, i);
    }
}

bool VerificaAtingido(Info i, void *aux)
{
    ProcAfetado *Atinge = aux;
    Figura *F = i;
    if (F->Tipo == 'T')
    {
        Texto *t = F->Figura;
        return VerificaPonto(Atinge->x, t->x, Atinge->x + Atinge->larg, Atinge->y + Atinge->alt, t->y, Atinge->y);
    }
    else if (F->Tipo == 'C')
    {
        Circulo *c = F->Figura;
        return (c->x + c->raio >= Atinge->x && c->x - c->raio <= Atinge->x + Atinge->larg &&
                c->y + c->raio >= Atinge->y && c->y - c->raio <= Atinge->y + Atinge->alt);
    }
    else if (F->Tipo == 'R')
    {
        Retangulo *r = F->Figura;
        return (r->x + r->larg >= Atinge->x && r->x <= Atinge->x + Atinge->larg &&
                r->y + r->alt >= Atinge->y && r->y <= Atinge->y + Atinge->alt);
    }
    else if (F->Tipo == 'L')
    {
        Linha *l = F->Figura;
        return (VerificaIntervalo(Atinge->x, l->x1, Atinge->x + Atinge->larg) &&
                VerificaIntervalo(Atinge->x, l->x2, Atinge->x + Atinge->larg) &&
                VerificaIntervalo(Atinge->y, l->y1, Atinge->y + Atinge->alt) &&
                VerificaIntervalo(Atinge->y, l->y2, Atinge->y + Atinge->alt));
    }

    else
    {
        printf("Erro ao verificar forma da figura atingida!\n");
        return false;
    }
}

double CalculaAreaAfetadaAntiga(void *Fig, void *Afeta)
{
    Figura *F = Fig;
    double tolerancia = 0.000001; // Tolerância para lidar com imprecisões numéricas
    if (F->Tipo == 'T')
    {
        return 0.1; // Proporção fixa em 10%
    }
    else if (F->Tipo == 'C')
    {
        Circulo *c = F->Figura;
        double AreaIntersecao = CalculaAreaIntersecaoCirculoRetangulo(F->Figura, Afeta);
        double AreaCirculo = PI * c->raio * c->raio;
        double diferenca = fabs(1 - AreaIntersecao / AreaCirculo);
        if (diferenca < tolerancia)
        {
            return 1.0;
        }
        else
        {
            return AreaIntersecao / AreaCirculo;
        }
    }
    else if (F->Tipo == 'R')
    {
        Retangulo *r = F->Figura;
        double AreaIntersecao = CalculaAreaIntersecaoRetanguloRetangulo(F->Figura, Afeta);
        double AreaRetangulo = r->larg * r->alt;
        double diferenca = fabs(1 - AreaIntersecao / AreaRetangulo);
        if (diferenca < tolerancia)
        {
            return 1.0;
        }
        else
        {
            return AreaIntersecao / AreaRetangulo;
        }
    }
    else if (F->Tipo == 'L')
    {
        return 0.1; // Proporção fixa em 10%
    }
    else
    {
        printf("Erro ao verificar forma da figura afetada!\n");
        return 0.0;
    }
}