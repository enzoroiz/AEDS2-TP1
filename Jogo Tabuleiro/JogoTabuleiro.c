#include "JogoTabuleiro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //String funções
#include <unistd.h> //GETOPT()
#define VIVO 1
#define MORTO 0
#define TAM_BUFFER 5000

void PegaArgumentos(int argc, char **argv, char ** entrada, char ** saida){
    const char *vetorOpcoes = "i:o:O:I:";
    char opcoes;
    while ((opcoes = getopt(argc, argv, vetorOpcoes)) != EOF){
        switch (opcoes){
        case 'i':
            *entrada = optarg;
            break;
        case 'I':
            *entrada = optarg;
            break;
        case 'o':
            *saida = optarg;
            break;
        case 'O':
            *saida = optarg;
            break;
        default:
            break;
        }
    }
}

void FLVazia(TipoLista *Lista){
    Lista->Primeiro = (Apontador) malloc(sizeof(Celula));
    Lista->Ultimo = Lista->Primeiro;
    Lista->Primeiro->Prox = NULL;
}

int Vazia(TipoLista Lista){
    return (Lista.Primeiro == Lista.Ultimo);
}

void Insere(TipoItem x, TipoLista *Lista){
    Apontador apontadorCelula;
    Lista->Ultimo->Prox = (Apontador) malloc(sizeof(Celula));
    apontadorCelula = Lista->Ultimo->Prox;
    Lista->Ultimo = Lista->Ultimo->Prox;
    x.enderecoCelula=apontadorCelula;
    Lista->Ultimo->Item = x;
    Lista->Ultimo->Prox = NULL;
}

void Imprime(char * saida, TipoLista Lista){
    FILE *fpSaida;
    fpSaida=fopen(saida, "w");
    Apontador Aux;
    Aux = Lista.Primeiro->Prox;
    while (Aux != NULL){
        fprintf(fpSaida, "%s %s\nPontos de vida %s %lld\n", Aux->Item.jogador, Aux->Item.apontadorCasaAtual->Item.casa, Aux->Item.jogador, Aux->Item.energia);
        Aux = Aux->Prox;
    }

    Aux = Lista.Primeiro->Prox;
    while(Aux!=NULL){
        if(Aux->Item.ganhador==1)
            fprintf(fpSaida, "Ganhei %s\n", Aux->Item.jogador);
        Aux = Aux->Prox;
    }
    Aux=Lista.Primeiro->Prox;
    while(Aux!=NULL){
        if(Aux->Item.status==MORTO)
            fprintf(fpSaida, "Morri %s\n", Aux->Item.jogador);
        Aux = Aux->Prox;
    }
    fclose(fpSaida);
}

void PegaRegraCasa(TipoLista *Lista){
    Apontador atual, percorre;
    atual=Lista->Primeiro->Prox;
    percorre=atual;
    while(atual!=NULL){
        while(percorre!=NULL){
            if(strcmp(atual->Item.regraCasa,"0")==0){
                atual->Item.apontadorRegraCasa=atual->Item.enderecoCelula;
                percorre=Lista->Ultimo;
            }
            else if(strcmp(atual->Item.regraCasa,percorre->Item.casa)==0){
                atual->Item.apontadorRegraCasa=percorre;
                percorre=Lista->Ultimo;
            }
            percorre=percorre->Prox;
        }
        atual=atual->Prox;
        percorre=Lista->Primeiro->Prox;
    }
}

Apontador CriaJogador(TipoLista *jogadores, Apontador apontadorJogadas, Apontador primeiraCasa){
    Apontador aux;
    TipoItem Item;
    if(Vazia(*jogadores)){
        strcpy(Item.jogador,apontadorJogadas->Item.jogador);
        Item.energia=5;
        Item.status=VIVO;
        Item.apontadorCasaAtual=primeiraCasa;
        Insere(Item, jogadores);
        return jogadores->Primeiro->Prox->Item.enderecoCelula;
    } else {
        aux=jogadores->Primeiro->Prox;
        while(aux!=NULL){
            if(strcmp(apontadorJogadas->Item.jogador,aux->Item.jogador)==0)
                return aux->Item.enderecoCelula;
            aux=aux->Prox;
        }
        strcpy(Item.jogador,apontadorJogadas->Item.jogador);
        Item.energia=5;
        Item.apontadorCasaAtual=primeiraCasa;
        Item.status=VIVO;
        Insere(Item, jogadores);
        return jogadores->Ultimo->Item.enderecoCelula;
    }
}

TipoLista ExecutaJogadas(TipoLista *jogadas, TipoLista *tabuleiro){
    Apontador percorreJogadas;
    Apontador percorreTabuleiro;
    Apontador jogadorAtual;
    int fimJogadas=0;
    long long int numDado;
    long long int i;
    TipoLista jogadores;

    FLVazia(&jogadores);//Faz a Lista de jogadores
    percorreJogadas=jogadas->Primeiro->Prox;//PercorreJogadas recebe a primeira Célula da Lista Jogadas

    while(percorreJogadas!=NULL){//ENQUANTO TIVER JOGADAS FAÇA
        percorreTabuleiro=tabuleiro->Primeiro->Prox;//Tabuleiro recebe a primeira Célula da Lista Tabuleiro
        numDado=percorreJogadas->Item.numDado;//Numdado
        jogadorAtual=CriaJogador(&jogadores,percorreJogadas,percorreTabuleiro);//CRIA JOGADOR E RETORNA ENDEREÇO DE ONDE A CELULA ESTA ARMAZENADA
        if((jogadorAtual->Item.status==VIVO) && (fimJogadas==0)) {//SE O JOGADOR NÃO ESTIVER MORTO
            for(i=1;i<=numDado;i++){//PARA I<=NUMDADO FAÇA
                percorreTabuleiro=jogadorAtual->Item.apontadorCasaAtual;//CASA ONDE O JOGADOR ESTÁ
                percorreTabuleiro=percorreTabuleiro->Item.apontadorProximaCasa;//ANDA UMA CASA
                if(percorreTabuleiro->Item.apontadorProximaCasa==NULL){//SE A CASA FOR A ULTIMA GANHA O JOGO OU MORRE NÃO TEM COMO ANDAR MAIS
                    jogadorAtual->Item.apontadorCasaAtual=percorreTabuleiro;//JOGADOR RECEBE A ULTIMA CASA
                    jogadorAtual->Item.energia+=percorreTabuleiro->Item.energia;//JOGADOR RECEBE SUA ENERGIA + ENERGIA DA CASA
                    if(jogadorAtual->Item.energia<=0)//SE A ENERGIA <= 0 JOGADOR MORRE
                        jogadorAtual->Item.status=MORTO;//MORRE JOGADOR
                    else {
                        fimJogadas=1; // JOGADA NA QUAL ACABOU O JOGO
                        //percorreJogadas=NULL;//DEFINE O FIM DAS JOGADAS
                        jogadorAtual->Item.ganhador=1;
                    }
                    i=numDado+1;//SETA i PARA NUMDADO+1 PARA NÃO SATISFAZER A CONDIÇÃO ABAIXO
                }


                else if(numDado==i){//QUANDO O JOGADOR ACABAR DE ANDAR NO TABULEIRO
                    jogadorAtual->Item.apontadorCasaAtual=percorreTabuleiro;//JOGADOR RECEBE ENDEREÇO DA CASA, É REDIRECIONADO
                    jogadorAtual->Item.energia+=percorreTabuleiro->Item.energia;//JOGADOR RECEBE SUA ENERGIA + ENERGIA DA CASA
                    if(jogadorAtual->Item.energia<=0)//SE A ENERGIA <= 0 JOGADOR MORRE
                        jogadorAtual->Item.status=MORTO;
                    else//SE NÃO O JOGADOR É REDIRECIONADO PELO REGRACASA
                        jogadorAtual->Item.apontadorCasaAtual=percorreTabuleiro->Item.apontadorRegraCasa;
                    if((jogadorAtual->Item.apontadorCasaAtual->Item.apontadorProximaCasa==NULL)&&(jogadorAtual->Item.status==VIVO)){//SE A CASA QUE FOR REDIRECIONADO FOR A ULTIMA E ENERGIA FOR SUFICIENTE, GANHA
                        jogadorAtual->Item.energia+=jogadorAtual->Item.apontadorCasaAtual->Item.energia;//VERIFICA SE TEM ENERGIA PARA GANHAR
                        if(jogadorAtual->Item.energia<=0)//SE NAO TIVER MORRE
                            jogadorAtual->Item.status=MORTO;
                        else {
                            jogadorAtual->Item.ganhador=1;//SE TIVER GANHA
                            fimJogadas=1; // JOGADA NA QUAL ACABOU O JOGO
                            //percorreJogadas=NULL;//TERMINA AS JOGADAS
                        }
                    }
                }
                else //QUANDO NÃO TIVER CHEGADO NA CASA QUE TIROU NO DADO
                    jogadorAtual->Item.apontadorCasaAtual=percorreTabuleiro;
            }//FECHA FOR DADO
        }//FECHA IF MORTO
        //if(percorreJogadas!=NULL){}
            percorreJogadas=percorreJogadas->Prox;

    }//FECHA WHILE JOGADAS
    return jogadores;
}

void PegaProximaCasa(TipoLista *Lista){
    Apontador atual, percorre;
    atual=Lista->Primeiro->Prox;
    percorre=atual;
    while(atual!=NULL){
        while(percorre!=NULL){
            if(strcmp(atual->Item.proximaCasa,percorre->Item.casa)==0){
                atual->Item.apontadorProximaCasa=percorre;
                percorre=Lista->Ultimo;
            } else if(strcmp(atual->Item.proximaCasa,"0")==0){
                atual->Item.apontadorProximaCasa=NULL;
                percorre=Lista->Ultimo;
            }
            percorre=percorre->Prox;
        }
        atual=atual->Prox;
        percorre=Lista->Primeiro->Prox;
    }
}

void FazTabuleiro(char * entrada, TipoLista *tabuleiro, TipoLista *jogadas){
    FILE *fpEntrada;
    char confere[TAM_BUFFER], casa[TAM_BUFFER], proximaCasa[TAM_BUFFER], regraCasa[TAM_BUFFER], jogador[TAM_BUFFER];
    int energia, numDado;
    TipoItem Item;

    fpEntrada=fopen(entrada, "r");
    fscanf(fpEntrada, "%s", confere);
    if(!strcmp(confere,"TABULEIRO")){
        fscanf(fpEntrada, "%s", confere);//casa 1ª linha
        while(strcmp(confere,"JOGADAS")!=0){
            strcpy(casa, confere); //copia confere em casa
            fscanf(fpEntrada, "%s", proximaCasa);
            fscanf(fpEntrada, "%s", regraCasa);
            fscanf(fpEntrada, "%d", &energia);
            strcpy(Item.casa,casa);
            strcpy(Item.proximaCasa,proximaCasa);
            strcpy(Item.regraCasa,regraCasa);
            Item.energia=energia;
            Insere(Item, tabuleiro);

            fscanf(fpEntrada, "%s", confere);
        }
        fscanf(fpEntrada, "%s", confere);//jogador 1ª linha
        while(strcmp(confere,"FIM")!=0){
            strcpy(jogador, confere); //copia confere em casa
            fscanf(fpEntrada, "%d", &numDado);
            strcpy(Item.jogador,jogador);
            Item.numDado=numDado;
            Item.ganhador=0;
            Insere(Item, jogadas);
            fscanf(fpEntrada, "%s", confere);
        }
        fclose(fpEntrada);
    } else if(!strcmp(confere,"JOGADAS")){
        fscanf(fpEntrada, "%s", confere);//casa 1ª linha
        while(strcmp(confere,"TABULEIRO")!=0){
            strcpy(jogador, confere); //copia confere em casa
            fscanf(fpEntrada, "%d", &numDado);
            strcpy(Item.jogador,jogador);
            Item.numDado=numDado;
            Item.ganhador=0;
            Insere(Item, jogadas);
            fscanf(fpEntrada, "%s", confere);
        }
        fscanf(fpEntrada, "%s", confere);//casa 1ª linha
        while(strcmp(confere,"FIM")!=0){
            strcpy(casa, confere); //copia confere em casa
            fscanf(fpEntrada, "%s", proximaCasa);
            fscanf(fpEntrada, "%s", regraCasa);
            fscanf(fpEntrada, "%d", &energia);
            strcpy(Item.casa,casa);
            strcpy(Item.proximaCasa,proximaCasa);
            strcpy(Item.regraCasa,regraCasa);
            Item.energia=energia;
            Insere(Item, tabuleiro);
            fscanf(fpEntrada, "%s", confere);
        }
        fscanf(fpEntrada, "%s", confere);//jogador 1ª linha
        fclose(fpEntrada);
    }
}
