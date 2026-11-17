#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "entidades.h"
#include "eventos.h"

#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MORRE 7
#define MISSAO 8
#define FIM 9

void chega(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ", t, h->id, b->id, cjto_card(b->presentes), b->lotacao);
	
	h->base = b->id;
	evento->heroi = h;
	evento->base = b;
	
	//espera
	if((cjto_card(b->presentes) < b->lotacao && fila_tamanho(b->espera) == 0) || h->paciencia > (10 * fila_tamanho(b->espera)))
	{
		fprio_insere(w->lef, evento, ESPERA, t);
		printf("ESPERA\n");
	}
	else
	{
		//desiste
		fprio_insere(w->lef, evento, DESISTE, t);
		printf("DESISTE\n");
	}
	
	return;
}

void espera(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", t, h->id, b->id, fila_tamanho(b->espera));
	
	fila_insere(b->espera, h->id);
	
	if(fila_tamanho(b->espera) > b->max_fila)
		b->max_fila = fila_tamanho(b->espera);
	
	evento->base = b;
	fprio_insere(w->lef, evento, AVISA, t);
	
	return;
}

void desiste(struct mundo *w, struct heroi *h, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: DESIST HEROI %2d BASE %d\n", t, h->id, h->base);
	
	evento->heroi = h;
	evento->base = *(w->bases + aleat(0, w->nbases-1));
	
	fprio_insere(w->lef, evento, VIAJA, t);
	
	return;
}

void avisa(struct mundo *w, struct base *b, int t)
{
	struct p_hbm *evento;
	int heroi_fila;
	
	printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b->id, cjto_card(b->presentes), b->lotacao);
	fila_imprime(b->espera);
	printf(" ]\n");
	
	while(cjto_card(b->presentes) < b->lotacao && fila_tamanho(b->espera) > 0)
	{
		evento = malloc(sizeof(struct p_hbm));
		if(!evento)
			return;
		
		fila_retira(b->espera, &heroi_fila);
		cjto_insere(b->presentes, heroi_fila);
		
		evento->heroi = *(w->herois + heroi_fila);
		evento->base = b;
		
		fprio_insere(w->lef, evento, ENTRA, t);
		
		printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", t, b->id, heroi_fila);
	}
	
	return;
}

void entra(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	int tpb;//tempo de permanencia na base
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	tpb = 15 + h->paciencia * aleat(1,20);
	evento->heroi = h;
	evento->base = b;
	
	printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h->id, b->id, cjto_card(b->presentes), b->lotacao, t + tpb);
	
	fprio_insere(w->lef, evento, SAI, t + tpb);
	
	return;
}

void sai(struct mundo *w, struct heroi *h, struct base *b, int t)
{
	struct p_hbm *evento;
	int base_d;
	
	cjto_retira(b->presentes, h->id);
	base_d = aleat(0, w->nbases-1);
	
	printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", t, h->id, b->id, cjto_card(b->presentes), b->lotacao);
	
	//viaja
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	evento->heroi = h;
	evento->base = *(w->bases + base_d);
	
	fprio_insere(w->lef, evento, VIAJA, t);
	
	//avisa
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	evento->base = *(w->bases + base_d);
	
	fprio_insere(w->lef, evento, AVISA, t);
	
	return;
}

void viaja(struct mundo *w, struct heroi *h, struct base *d, int t)
{
	struct p_hbm *evento;
	int distancia, duracao;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	distancia = distancia_cartesiana( (*(w->bases + h->base))->local , d->local );
	
	if(h->velocidade <= 0)
		h->velocidade = 1;
	duracao = distancia / h->velocidade;
	
	printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", t, h->id, h->base, d->id, distancia, h->velocidade, t + duracao);
	
	evento->heroi = h;
	evento->base = d;
	
	fprio_insere(w->lef, evento, CHEGA, t + duracao);
	
	return;
}

void morre(struct mundo *w, struct heroi *h, struct base *b, struct missao *m, int t)
{
	struct p_hbm *evento;
	
	evento = malloc(sizeof(struct p_hbm));
	if(!evento)
		return;
	
	printf("%6d: MORRE HEROI %2d MISSAO %d\n", t, h->id, m->id);
	
	cjto_retira(b->presentes, h->id);
	h->status = 0;
	
	evento->base = b;
	
	fprio_insere(w->lef, evento, AVISA, t);
	
	return;
}

void missao(struct mundo *w, struct missao *m, int t)
{
	struct p_hbm *evento;
	struct cjto_t *habilidades_da_base, *p_cjto_aux;
	int i, experiencia, bmp;//base mais proxima
	
	m->tentativas++;
	
	printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", t, m->id, m->tentativas);
	cjto_imprime(m->habilidades);
	printf(" ]\n");
	
	habilidades_da_base = cjto_cria(w->nhabilidades);
	if(!habilidades_da_base)
	{
		fprio_insere(w->lef, NULL, FIM, t);
		return;
	}
	
	p_cjto_aux = cjto_cria(w->nhabilidades);
	if(!p_cjto_aux)
	{
		cjto_destroi(habilidades_da_base);
		fprio_insere(w->lef, NULL, FIM, t);
		return;
	}
	
	bmp = encontra_bmp_apta(w, m, t, habilidades_da_base, p_cjto_aux);
	if(bmp >= 0)
	{
		m->status = 1;
		(*(w->bases + bmp))->missoes++;
		w->missoes_cumpridas++;
		
		//da experiencia para todos os herois da base
		for(i = 0; i < w->nherois; i++)
		{
			if(cjto_pertence( (*(w->bases + bmp))->presentes, i ))
				(*(w->herois + i))->experiencia++;
		}
		
		printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", t, m->id, bmp);
		cjto_imprime(habilidades_da_base);
		printf(" ]\n");
		
		cjto_destroi(habilidades_da_base);
		cjto_destroi(p_cjto_aux);
		habilidades_da_base = NULL;
		p_cjto_aux = NULL;
		return;
	}
	else
	{
		cjto_destroi(habilidades_da_base);
		cjto_destroi(p_cjto_aux);
		habilidades_da_base = NULL;
		p_cjto_aux = NULL;
		
		evento = malloc(sizeof(struct p_hbm));
		if(!evento)
			return;
		
		bmp = encontra_bmp(w, m);
		if(w->ncompostosv > 0 && t % 2500 == 0 && cjto_card((*(w->bases + bmp))->presentes) > 0)
		{
			w->ncompostosv--;
			m->status = 1;
			(*(w->bases + bmp))->missoes++;
			w->missoes_cumpridas++;
			
			//mata o heroi mais experiente
			experiencia = -1;
			for(i = 0; i < w->nherois; i++)
			{
				if(cjto_pertence( (*(w->bases + bmp))->presentes, i ) && (*(w->herois + i))->experiencia > experiencia)
				{
					experiencia = (*(w->herois + i))->experiencia;
					evento->heroi = *(w->herois + i);
				}
			}
			evento->base = *(w->bases + bmp);
			evento->missao = m;
			fprio_insere(w->lef, evento, MORRE, t);
			
			//da experiencia para todos da base com excecao do heroi morto
			for(i = 0; i < w->nherois; i++)
			{
				if(cjto_pertence( (*(w->bases + bmp))->presentes, i ) && i != evento->heroi->id)
					(*(w->herois + i))->experiencia++;
			}
			return;
		}
		else
		{
			evento->missao = m;
			printf("%6d: MISSAO %d IMPOSSIVEL\n", t, m->id);
			fprio_insere(w->lef, evento, MISSAO, t + 24 * 60);//para o proximo dia
		}
		return;
	}
}

void fim(struct mundo *w, int t)
{
	int i, tentativas_max, tentativas_min, nmortos;
	float m_cumpridas_porcnt, n_mortos_porcnt, tentativas_media;
	
	printf("%6d: FIM\n\n", t);
	
	//estatisticas dos herois
	nmortos = 0;
	for(i = 0; i < w->nherois; i++)
	{
		printf("HEROI %2d ", i);
		if((*(w->herois + i))->status == 1)
			printf("VIVO");
		else
		{
			printf("MORTO");
			nmortos++;
		}
		printf(" PAC %3d VEL %4d EXP %4d HABS [ ", (*(w->herois + i))->paciencia, (*(w->herois + i))->velocidade, (*(w->herois + i))->experiencia);
		cjto_imprime((*(w->herois + i))->habilidades);
		printf(" ]\n");
	}
	printf("\n");
	
	//estatisticas das bases
	for(i = 0; i < w->nbases; i++)
		printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, (*(w->bases + i))->lotacao, (*(w->bases + i))->max_fila, (*(w->bases + i))->missoes);
	printf("\n");
	
	printf("EVENTOS TRATADOS: %d\n", w->eventos_tratados);
	
	//estatisticas das missoes
	m_cumpridas_porcnt = (float) w->missoes_cumpridas / (float) w->nmissoes;
	m_cumpridas_porcnt *= 100;
	printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", w->missoes_cumpridas, w->nmissoes, m_cumpridas_porcnt);
	
	
	tentativas_max = (*(w->missoes + 0))->tentativas;
	tentativas_min = (*(w->missoes + 0))->tentativas;
	tentativas_media = 0;
	for(i = 0; i < w->nmissoes; i++)
	{
		if(tentativas_min > (*(w->missoes + i))->tentativas)
			tentativas_min = (*(w->missoes + i))->tentativas;
		if(tentativas_max < (*(w->missoes + i))->tentativas)
			tentativas_max = (*(w->missoes + i))->tentativas;
		tentativas_media += (*(w->missoes + i))->tentativas;
	}
	tentativas_media /= (float) w->nmissoes;
	
	printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", tentativas_min, tentativas_max, tentativas_media);
	
	n_mortos_porcnt = (float) nmortos / (float) w->nherois;
	n_mortos_porcnt *= 100;
	printf("TAXA MORTALIDADE: %.1f%%\n", n_mortos_porcnt);
	
	return;
}
