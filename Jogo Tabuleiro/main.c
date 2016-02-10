#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strcpy()
#include "JogoTabuleiro.h"
#define TAM_BUFFER 5000

int main(int argc, char ** argv)
{
    char *entrada=NULL;
    char *saida=NULL;
    TipoLista tabuleiro, jogadas;
    FLVazia(&tabuleiro);
    FLVazia(&jogadas);

    PegaArgumentos(argc, argv, &entrada, &saida);
    FazTabuleiro(entrada, &tabuleiro, &jogadas);
    PegaRegraCasa(&tabuleiro);
    PegaProximaCasa(&tabuleiro);
    Imprime(saida, ExecutaJogadas(&jogadas, &tabuleiro));
    return 0;
}
