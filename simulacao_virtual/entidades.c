#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "entidades.h"

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

//funcoes auxiliares

int aleat(int min, int max)
{
    return min + rand() % (max - min + 1);
}

int distancia_cartesiana(struct coordenada *a, struct coordenada *b)
{
    int difx, dify;
    
    difx = b->x - a->x;
    dify = b->y - a->y;
    
    return sqrt( difx*difx + dify*dify );
}

struct coordenada *coord_cria(int x, int y)
{
	struct coordenada *coord;
	
	coord = malloc(sizeof(struct coordenada));
	if(!coord)
		return NULL;
	
	coord->x = x;
	coord->y = y;
	
	return coord;
}

void coord_destroi(struct coordenada *coord)
{
	if(!coord)
		return;
	free(coord);
	coord = NULL;
	return;
}

//encontra base mais proxima
int encontra_bmp(struct mundo *w, struct missao *m)
{
	struct base *b;
	int bmp, i;
	
	b = *(w->bases + 0);
	for(i = 0; i < w->nbases; i++)
	{
		if( distancia_cartesiana( m->local, b->local ) > distancia_cartesiana( m->local, (*(w->bases + i))->local ) )
			b = *(w->bases + i);
	}
	bmp = b->id;
	
	return bmp;
}

//retorna 0 se base nao eh apta, 1 caso seja apta
int base_apta(struct mundo *w, struct base *b, struct missao *m, int t, struct cjto_t *p_cjto_aux)
{
	int i, j;
	
	printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", t, m->id, b->id, distancia_cartesiana(m->local, b->local));
	cjto_imprime(b->presentes);
	printf(" ]\n");
	
	//cada heroi
	for(i = 0; i < w->nherois; i++)
	{
		//heroi da base
		if(cjto_pertence(b->presentes, i) && (*(w->herois + i))->status == 1)
		{
			printf("%6d: MISSAO %d HAB HEROI %2d: [ ", t, m->id, i);
			//cada habilidade
			for(j = 0; j < w->nhabilidades; j++)
			{
				//habilidade do heroi
				if(cjto_pertence( (*(w->herois + i))->habilidades, j ))
				{
					//adiciona habilidade do heroi as habilidades da base
					cjto_insere(p_cjto_aux, j);
					printf("%d ", j);
				}
			}
			printf("]\n");
		}
	}
	
	printf("%6d: MISSAO %d UNIAO HAB BASE %d: [ ", t, m->id, b->id);
	cjto_imprime(p_cjto_aux);
	printf(" ]\n");
	
	if(cjto_contem(p_cjto_aux, m->habilidades))
		return 1;
	return 0;
}

int encontra_bmp_apta(struct mundo *w, struct missao *m, int t, struct cjto_t *habilidades_da_base, struct cjto_t *p_cjto_aux)
{
	struct base *b;
	int i, j;
	
	b = NULL;
	//cada base
	for(i = 0; i < w->nbases; i++)
	{
		if(base_apta(w, *(w->bases + i), m, t, p_cjto_aux))
		{
			if(b == NULL)//primeiro caso
				b = *(w->bases + i);
			else
			{
				if(distancia_cartesiana( m->local, b->local ) > distancia_cartesiana( m->local, (*(w->bases + i))->local ))
					b = *(w->bases + i);
			}
		}
		
		//caso base apta
		if(b == *(w->bases + i))
		{
			//esvazia habilidades_da_base
			for(j = 0; j < w->nhabilidades; j++)
				cjto_retira(habilidades_da_base, j);
			
			//adicionar o p_cjto_aux a habilidades_da_base
			for(j = 0; j < w->nhabilidades; j++)
			{
				if(cjto_pertence(p_cjto_aux, j))
					cjto_insere(habilidades_da_base, j);
			}
		}
		
		//esvazia p_cjto_aux
		for(j = 0; j < w->nhabilidades; j++)
				cjto_retira(p_cjto_aux, j);
	}
	
	if(b == NULL)
		return -1;
	return b->id;
}

//destroi heroi, base e missao
void destroi_hbm(struct mundo *w, int nherois, int nbases, int nmissoes)
{
	int i;
	
	//liberar herois e vetor
	for(i = 0; i < nherois; i++)
	{
		destroi_heroi(*(w->herois + i));
		*(w->herois + i) = NULL;
	}
	free(w->herois);
	w->herois = NULL;
	
	//liberar bases e vetor
	for(i = 0; i < nbases; i++)
	{
		destroi_base(*(w->bases + i));
		*(w->bases + i) = NULL;
	}
	free(w->bases);
	w->bases = NULL;
	
	//liberar missoes e e vetor
	for(i = 0; i < nmissoes; i++)
	{
		destroi_missao(*(w->missoes + i));
		*(w->missoes + i) = NULL;
	}
	free(w->missoes);
	w->missoes = NULL;
	
	return;
}

//heroi

struct heroi *cria_heroi(int id, int tamanho_cjto, int n_habs, int paciencia, int velocidade, int experiencia, int base, int status)
{
	struct heroi *h;
	
	h = malloc(sizeof(struct heroi));
	if(!h)
		return NULL;
	
	h->habilidades = cjto_aleat(tamanho_cjto, n_habs);
	if(!h->habilidades)
	{
		free(h);
		h = NULL;
		return NULL;
	}
	
	h->id = id;
	h->base = base;
	h->status = status;
	h->paciencia = paciencia;
	h->velocidade = velocidade;
	h->experiencia = experiencia;
	
	return h;
}

void destroi_heroi(struct heroi *h)
{
	h->habilidades = cjto_destroi(h->habilidades);
	h->habilidades = NULL;
	free(h);
	h = NULL;
	return;
}

void insere_heroi_vet(struct heroi **vet, struct heroi *h, int pos)
{
	if(pos < 0)
		return;
	
	*(vet + pos) = h;
	return;
}

//base

struct base *cria_base(int id, int lotacao, int n_herois, int coord_x, int coord_y)
{
	struct base *b;
	
	b = malloc(sizeof(struct base));
	if(!b)
		return NULL;
	
	b->presentes = cjto_cria(n_herois);
	if(!b->presentes)
	{
		free(b);
		b = NULL;
		return NULL;
	}
	
	b->espera = fila_cria();
	if(!b->espera)
	{
		free(b->presentes);
		b->presentes = NULL;
		free(b);
		b = NULL;
		return NULL;
	}
	
	b->local = coord_cria(coord_x, coord_y);
	if(!b->local)
	{
		free(b->espera);
		b->espera = NULL;
		free(b->presentes);
		b->presentes = NULL;
		free(b);
		b = NULL;
		return NULL;
	}
	
	b->id = id;
	b->lotacao = lotacao;
	b->max_fila = 0;
	b->missoes = 0;
	
	return b;
}

void destroi_base(struct base *b)
{
	if(!b)
		return;
	
	fila_destroi(b->espera);
	b->espera = NULL;
	cjto_destroi(b->presentes);
	b->presentes = NULL;
	coord_destroi(b->local);
	b->local = NULL;
	
	free(b);
	b = NULL;
	return;
}

void insere_base_vet(struct base **vet, struct base *b, int pos)
{
	if(pos < 0)
		return;
	
	*(vet + pos) = b;
	return;
}

//missao

struct missao *cria_missao(int id, int tamanho_cjto, int n_habs, int coord_x, int coord_y, int status, int tentativas)
{
	struct missao *m;
	
	m = malloc(sizeof(struct missao));
	if(!m)
		return NULL;
	
	m->habilidades = cjto_aleat(tamanho_cjto, n_habs);
	if(!m->habilidades)
	{
		free(m);
		m = NULL;
		return NULL;
	}
	
	m->local = coord_cria(coord_x, coord_y);
	if(!m->local)
	{
		free(m->habilidades);
		m->habilidades = NULL;
		free(m);
		m = NULL;
		return NULL;
	}	
	
	m->id = id;
	m->status = status;
	m->tentativas = tentativas;
	return m;
}

void destroi_missao(struct missao *m)
{
	if(!m)
		return;
	
	cjto_destroi(m->habilidades);
	m->habilidades = NULL;
	coord_destroi(m->local);
	m->local = NULL;
	free(m);
	m = NULL;
	return;
}

void insere_missao_vet(struct missao **vet, struct missao *m, int pos)
{
	if(pos < 0)
		return;
	
	*(vet + pos) = m;
	return;
}

//mundo

struct mundo *cria_mundo(int nherois, int nbases, int nmissoes, int nhabilidades, int ncompostosv, int ntamanhomundo, int relogio, int eventos_tratados, int missoes_cumpridas)
{
	struct mundo *w;
	int i;
	
	//alocacao
	w = malloc(sizeof(struct mundo));
	if(!w)
		return NULL;
	
	w->herois = malloc(sizeof(struct heroi *) * nherois);
	if(!w->herois)
	{
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->bases = malloc(sizeof(struct base *) * nbases);
	if(!w->bases)
	{
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->missoes = malloc(sizeof(struct missao *) * nmissoes);
	if(!w->missoes)
	{
		free(w->bases);
		w->bases = NULL;
		
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->tamanhomundo = coord_cria(ntamanhomundo, ntamanhomundo);
	if(!w->tamanhomundo)
	{
		free(w->missoes);
		w->missoes = NULL;
		
		free(w->bases);
		w->bases = NULL;
		
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	w->lef = fprio_cria();
	if(!w->lef)
	{
		free(w->tamanhomundo);
		w->tamanhomundo = NULL;
		
		free(w->missoes);
		w->missoes = NULL;
		
		free(w->bases);
		w->bases = NULL;
		
		free(w->herois);
		w->herois = NULL;
		
		free(w);
		w = NULL;
		
		return NULL;
	}
	
	//insercao dos inteiros
	w->nherois = nherois;
	w->nbases = nbases;
	w->nmissoes = nmissoes;
	w->nhabilidades = nhabilidades;
	w->ncompostosv = ncompostosv;
	w->relogio = relogio;
	w->eventos_tratados = eventos_tratados;
	w->missoes_cumpridas = missoes_cumpridas;
	
	//insercao de null nos ponteiros dos vetores das estruturas
	for(i = 0; i < nherois; i++)
		insere_heroi_vet(w->herois, NULL, i);
	for(i = 0; i < nbases; i++)
		insere_base_vet(w->bases, NULL, i);
	for(i = 0; i < nmissoes; i++)
		insere_missao_vet(w->missoes, NULL, i);
	
	return w;
}

void destroi_mundo(struct mundo *w, int nherois, int nbases, int nmissoes)
{	
	destroi_hbm(w, nherois, nbases, nmissoes);
	
	coord_destroi(w->tamanhomundo);
	
	w->lef = fprio_destroi(w->lef);
	
	free(w);
	w = NULL;
	
	return;
}
