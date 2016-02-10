#include <stdio.h>
#include <stdlib.h>
#define TAM_BUFFER 5000
#define VIVO 1
#define MORTO 0

typedef struct Celula_str *Apontador;
typedef struct {
    char casa[TAM_BUFFER];
    char proximaCasa[TAM_BUFFER];
    char regraCasa[TAM_BUFFER];
    long long int energia;
    char jogador[TAM_BUFFER];
    long long int numDado;
    Apontador apontadorCasaAtual;
    Apontador enderecoCelula;
    Apontador apontadorRegraCasa;
    Apontador apontadorProximaCasa;
    int status;
    int ganhador;
} TipoItem;


typedef struct Celula_str {
    TipoItem Item;
    Apontador Prox;
} Celula;

typedef struct {
    Apontador Primeiro, Ultimo;
} TipoLista;

void PegaArgumentos(int , char **, char **, char **);
void FLVazia(TipoLista *);
int Vazia(TipoLista);
void Insere(TipoItem, TipoLista *);
void Imprime(char *, TipoLista);
void PegaRegraCasa(TipoLista *);
Apontador CriaJogador(TipoLista *, Apontador, Apontador);
TipoLista ExecutaJogadas(TipoLista *, TipoLista *);
void PegaProximaCasa(TipoLista *);
void FazTabuleiro(char *, TipoLista *, TipoLista *);
