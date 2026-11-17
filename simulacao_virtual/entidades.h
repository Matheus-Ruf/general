#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "conjunto.h"

//structs

struct p_hbm//ponteiro triplo para heroi, base e missao
{
    struct heroi *heroi;
    struct base *base;
    struct missao *missao;
};

struct coordenada
{
    int x;   //numero inteiro >= 0 que indica o eixo horizontal
    int y;   //numero inteiro >= 0 que indica o eixo vertical
};

struct heroi
{
    int id;                         //numero inteiro >= 0 que identifica unicamente o heroi
    struct cjto_t *habilidades;    //conjunto de habilidades que o heroi possui. Cada habilidade eh representada por um numero inteiro >= 0
    int paciencia;                 //numero inteiro >= 0 que indica quao paciente uma pessoa eh. Em nosso modelo, isso afeta as decisões de permanencia em bases e ﬁlas
    int velocidade;               //numero inteiro >= 0 indicando a velocidade de deslocamento de um heroi, que irah afetar o tempo de deslocamento entre as bases
    int experiencia;              //numero inteiro >= 0 que indica o numero de missoes em que o heroi ja participou
    int base;                    //ID da base onde o herói se encontra no momento
    int status;                  //morto == 0; vivo == 1
};

struct base
{
    int id;                        //numero inteiro >= 0 que identifica cada base
    int lotacao;                  //numero maximo de herois naquela base
    struct cjto_t *presentes;     //conjunto dos IDs dos herois que estao atualmente na base, constituem as equipes disponiveis para realizar missoes
    struct fila_t *espera;       //fila onde os herois esperam para poder entrar na base
    struct coordenada *local;    //localizacao da base (par de coordenadas inteiras X, Y >= 0)
    int max_fila;                //tamanho maximo em que a fila ja ficou
    int missoes;                 //numero de missoes em que a base participou
};

struct missao
{
    int id;                         //numero inteiro >= 0 que identifica a missao
    struct cjto_t *habilidades;     //conjunto de habilidades necessarias para cumprir a missao
    struct coordenada *local;       //localizacao da missao (par de coordenadas inteiras X, Y >= 0)
    int status;                     // nao cumprida == 0; cumprida == 1
    int tentativas;                 //numero de tentativas na missao
};

struct mundo
{
    int nherois;                            //numero total de herois no mundo
    struct heroi **herois;                  //vetor representando todos os herois
    int nbases;                             //numero total de bases no mundo
    struct base **bases;                    //vetor representando todas as bases
    int nmissoes;                           //numero total de missoes a cumprir
    struct missao **missoes;                //vetor representando todas as missoes
    int nhabilidades;                       //numero de habilidades distintas possiveis
    int ncompostosv;							     //numero de compostos v disponıveis
    struct coordenada *tamanhomundo;       //coordenadas maximas do plano cartesiano (as coordenadas minimas sao 0, 0); vamos considerar que o mapa 
                                           //de nossa supercidade eh representado por um plano cartesiano de tamanho tal que cada unidade representa 1 metro;
    int relogio;                           //numero inteiro >= 0 indicando o tempo atual no mundo. Cada unidade de tempo no mundo simulado representa 1 minuto de tempo real
    struct fprio_t *lef;                  //lista de eventos futuros
    int eventos_tratados;                 //numero de eventos realizados
    int missoes_cumpridas;				   	//numero de missoes cumpridas
};

//funcoes de auxilio

//retorna um valor inteiro entre [min..max]
int aleat(int min, int max);

//retorna a distancia cartesiana entre os pontos "a" e "b"
int distancia_cartesiana(struct coordenada *a, struct coordenada *b);

struct coordenada *coord_cria(int x, int y);

void coord_destroi(struct coordenada *coord);

//encontra base mais proxima
//retorna id da base mais proxima
int encontra_bmp(struct mundo *w, struct missao *m);

//encontra base mais proxima apta para a missao
//retorna -1 se nao tiver base apta, ou id da base apta
int encontra_bmp_apta(struct mundo *w, struct missao *m, int t, struct cjto_t *habilidades_da_base, struct cjto_t *p_cjto_aux);

//hbm: heroi base e missao
void destroi_hbm(struct mundo *w, int nherois, int nbases, int nmissoes);

//heroi

//tamanho_cjto: quantidade de habilidades para se ter no conjunto
//n_habs: as habilidades so serao aceitas se estiverem no intervalo de [0..n_habs - 1]
struct heroi *cria_heroi(int id, int tamanho_cjto, int n_habs, int paciencia, int velocidade, int experiencia, int base, int status);

void destroi_heroi(struct heroi *h);

//pos: posicao
//*h pode ser null
//pos precisa ser maior igual a zero
//quem chamar deve garatir que vet eh acessivel
void insere_heroi_vet(struct heroi **vet, struct heroi *h, int pos);

//base

struct base *cria_base(int id, int lotacao, int n_herois, int coord_x, int coord_y);

void destroi_base(struct base *b);

//pos: posicao
//*b pode ser null
//pos precisa ser maior igual a zero
//quem chamar deve garatir que vet eh acessivel
void insere_base_vet(struct base **vet, struct base *b, int pos);

//missao

//tamanho_cjto: quantidade de habilidades para se ter no conjunto
//n_habs: as habilidades so serao aceitas se estiverem no intervalo de [0..n_habs - 1]
struct missao *cria_missao(int id, int tamanho_cjto, int n_habs, int coord_x, int coord_y, int status, int tentativas);

void destroi_missao(struct missao *m);

//pos: posicao
//*m pode ser null
//pos precisa ser maior igual a zero
//quem chamar deve garatir que vet eh acessivel
void insere_missao_vet(struct missao **vet, struct missao *m, int pos);

//mundo

struct mundo *cria_mundo(int nherois, int nbases, int nmissoes, int nhabilidades, int ncompostosv, int ntamanhomundo, int relogio, int eventos_tratados, int missoes_cumpridas);

void destroi_mundo(struct mundo *w, int nherois, int nbases, int nmissoes);

#endif
