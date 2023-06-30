#include "arqsvg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../def.h"
#include "geradores.h"

ArqSvg abreEscritaSvg(char *fn)
{
    char nomeSVG[strlen(fn) + 4];
    strcpy(nomeSVG,fn);
    strcat(nomeSVG,".svg");
    ArqSvg fsvg = fopen(nomeSVG, "w");

    if (fsvg == NULL)
    {
        return NULL;
    }
    fprintf(fsvg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%%\" height=\"100%%\" viewBox=\"%d %d %d %d\">\n", SIZE_X1_Y1, SIZE_X1_Y1, SIZE_X2_Y2, SIZE_X2_Y2);
    return fsvg;
}

void preparaDecoracao(char **deco, int decoLen, char *corBorda, char *corPreenchimento, char *larguraBorda, double transparencia, double transparenciaPreenchimento, double transparenciaBorda, double pontilhadoBorda)
{
    char decoracao[250];
    decoracao[0] = '\0';
    if (corBorda != NULL)
    {
        strcat(decoracao, "stroke=\"");
        strcat(decoracao, corBorda);
        strcat(decoracao, "\" ");
    }
    if (corPreenchimento != NULL)
    {
        strcat(decoracao, "fill=\"");
        strcat(decoracao, corPreenchimento);
        strcat(decoracao, "\" ");
    }
    if (larguraBorda != NULL)
    {
        strcat(decoracao, "stroke-width=\"");
        strcat(decoracao, larguraBorda);
        strcat(decoracao, "\" ");
    }
    if (transparencia >= 0)
    {
        char strTransparencia[20];
        sprintf(strTransparencia, "%f", transparencia);
        strcat(decoracao, "opacity=");
        strcat(decoracao, strTransparencia);
        strcat(decoracao, "\" ");
    }
    if (transparenciaPreenchimento >= 0)
    {
        char strTransparenciaPreenchimento[20];
        sprintf(strTransparenciaPreenchimento, "%f", transparenciaPreenchimento);
        strcat(decoracao, "fill-opacity=\"");
        strcat(decoracao, strTransparenciaPreenchimento);
        strcat(decoracao, "\" ");
    }
    if (transparenciaBorda >= 0)
    {
        char strTransparenciaBorda[20];
        sprintf(strTransparenciaBorda, "%f", transparenciaBorda);
        strcat(decoracao, "stroke-opacity=\"");
        strcat(decoracao, strTransparenciaBorda);
        strcat(decoracao, "\" ");
    }
    if (pontilhadoBorda >= 0)
    {
        char strPontilhadoBorda[20];
        sprintf(strPontilhadoBorda, "%lf", pontilhadoBorda);
        strcat(decoracao, "stroke-dasharray=\"");
        strcat(decoracao, strPontilhadoBorda);
        strcat(decoracao, "\" ");
    }
    decoLen = strlen(decoracao);
    *deco = malloc((decoLen + 1) * sizeof(char));
    strcpy(*deco, decoracao);
}

void escreveCirculoSvg(ArqSvg fsvg, double xc, double yc, double r, char *deco)
{
    fprintf(fsvg, " <circle cx=\"%f\" cy=\"%f\" r=\"%f\" %s/>\n", xc, yc, r, deco);
}

void escreveRetanguloSvg(ArqSvg fsvg, double x, double y, double larg, double alt, char *deco)
{
    fprintf(fsvg, " <rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" %s/>\n", x, y, larg, alt, deco);
}

void escreveLinhaSvg(ArqSvg fsvg, double x1, double y1, double x2, double y2, char *deco)
{
    fprintf(fsvg, " <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" %s/>\n", x1, y1, x2, y2, deco);
}

void preparaDecoracaoTexto(char **deco, int decoLen, char *fontFamily, char *fontStyle, char *fontWeight, char *fontSize, char *corBorda, char *fontColor, char *textAnchor, char *rotacao)
{
    char decoracao[200];
    decoracao[0] = '\0';
    if (fontFamily != NULL)
    {
        strcat(decoracao, "font-family=\"");
        strcat(decoracao, fontFamily);
        strcat(decoracao, "\" ");
    }
    if (fontStyle != NULL)
    {
        strcat(decoracao, "font-style=\"");
        strcat(decoracao, fontStyle);
        strcat(decoracao, "\" ");
    }
    if (fontWeight != NULL)
    {
        strcat(decoracao, "font-weight=\"");
        strcat(decoracao, fontWeight);
        strcat(decoracao, "\" ");
    }
    if (fontSize != NULL)
    {
        strcat(decoracao, "font-size=\"");
        strcat(decoracao, fontSize);
        strcat(decoracao, "\" ");
    }
    if (corBorda != NULL)
    {
        strcat(decoracao, "stroke=\"");
        strcat(decoracao, corBorda);
        strcat(decoracao, "\" ");
    }
    if (fontColor != NULL)
    {
        strcat(decoracao, "fill=\"");
        strcat(decoracao, fontColor);
        strcat(decoracao, "\" ");
    }
    if (textAnchor != NULL)
    {
        strcat(decoracao, "text-anchor=\"");
        strcat(decoracao, textAnchor);
        strcat(decoracao, "\" ");
    }
    if (rotacao != NULL)
    {
        strcat(decoracao, "transform =\"rotate(");
        strcat(decoracao, rotacao);
        strcat(decoracao, ")\"");
    }
    decoLen = strlen(decoracao);
    *deco = malloc((decoLen + 1) * sizeof(char));
    strcpy(*deco, decoracao);
}

void escreveTextoSvg(ArqSvg fsvg, double x, double y, char *txt, char *decoTxt)
{
    fprintf(fsvg, " <text x=\"%f\" y=\"%f\" %s>%s</text>\n", x, y, decoTxt, txt);
}

void fechaSvg(ArqSvg fsvg)
{
    fprintf(fsvg, "</svg>\n");
    fclose(fsvg);
}
