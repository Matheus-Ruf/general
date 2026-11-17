#ifndef EVENTOS_H
#define EVENTOS_H

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "conjunto.h"
#include "entidades.h"

/*
Representa um heroi H chegando em uma base B no instante T. Ao chegar, o
heroi analisa o tamanho da fila e decide se espera para entrar ou desiste
*/
void chega(struct mundo *w, struct heroi *h, struct base *b, int t);

/*
O heroi H entra na fila de espera da base B. Assim que H entrar na fila, o
porteiro da base B eh avisado para verificar a fila
*/
void espera(struct mundo *w, struct heroi *h, struct base *b, int t);

/*
O heroi H desiste de entrar na base B, escolhe uma base aleatoria D e viaja
para la
*/
void desiste(struct mundo *w, struct heroi *h, int t);

/*
O porteiro da base B trata a fila de espera
*/
void avisa(struct mundo *w, struct base *b, int t);

/*
O heroi H entra na base B. Ao entrar, o heroi decide quanto tempo vai ficar e
agenda sua saida da base
*/
void entra(struct mundo *w, struct heroi *h, struct base *b, int t);

/*
O heroi H sai da base B. Ao sair, escolhe uma base de destino para viajar; o
porteiro de B eh avisado, pois uma vaga foi liberada
*/
void sai(struct mundo *w, struct heroi *h, struct base *b, int t);

/*
O heroi H se desloca para uma base D (que pode ser a mesma onde ja esta)
*/
void viaja(struct mundo *w, struct heroi *h, struct base *d, int t);

/*
O heroi H morre no instante T.
* O heroi eh retirado da base B e libera uma vaga na base.
* O porteiro de B eh avisado da nova vaga.
* Eventos futuros de um heroi morto sao ignorados.
*/
void morre(struct mundo *w, struct heroi *h, struct base *b, struct missao *m, int t);

/*
Uma missao M eh disparada no instante T. Sao caracteristicas de uma missao:

* Cada missao ocorre em um local aleatorio e requer um conjunto aleatorio
de habilidades; ambos sao definidos durante a inicializacao.

* Cada equipe eh formada pelo conjunto de herois presentes em uma base.

* Uma equipe esta apta para a missao se a uniao das habilidades de seus
herois contem as habilidades requeridas pela missao.

* Deve ser escolhida para a missao a equipe da base mais proxima ao local
da missao e que esteja apta para ela.

* Ao completar uma missao, os herois da equipe escolhida ganham pontos
de experiencia, 1 por missao.

* Se nenhuma equipe estiver apta para a missao, eh possivel que o heroi mais
experiente da base mais proxima utilize um Composto V, se disponivel.
(O Composto V fara com que ele consiga todas as habilidades necessarias.
Contudo, apos a utilizacao do Composto V, o heroi morre.)

* Se uma missao nao puder ser completada, ela eh marcada como “impossivel”
e adiada de 24 horas.


Para simplificar o modelo, as missoes sao consideradas instantaneas e sem
tempo de deslocamento. Por isso, neste modelo o heroi nem sai da base.

Com objetivo de regular o consumo dos Compostos V, so eh possivel a utilizacao
do mesmo se a missao ocorrer em um instante T multiplos de 2500.
Dessa forma, se uma missao ocorre em um instante T nao multiplo de 2500 e
nao existem equipes aptas, a missao eh marcada como ”impossivel”, mesmo que
existam Compostos V disponiveis.
*/
void missao(struct mundo *w, struct missao *m, int t);

/*
Encerra a simulacao no instante T
*/
void fim(struct mundo *w, int t);

#endif
