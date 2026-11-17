#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria ()
{
	struct fprio_t *fila;
	
	fila = malloc(sizeof(struct fprio_t));
	if(!fila)
		return NULL;
	
	fila->prim = NULL;
	fila->num = 0;
	
	return fila;
}

struct fprio_t *fprio_destroi (struct fprio_t *f)
{
	void *descarte1;
	int *descarte2;
	
	descarte2 = malloc(sizeof(int));
	if(!f || !descarte2)
		return NULL;
	
	while(f->num != 0)
	{
		descarte1 = fprio_retira(f, descarte2, descarte2);
		if(!descarte1)
			free(descarte1);
	}
	f->prim = NULL;
	
	free(f);
	f = NULL;
	
	free(descarte2);
	descarte2 = NULL;
	descarte1 = NULL;
	
	return NULL;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
	struct fpnodo_t *inserido, *anterior;
	
	if(!item)
		return -1;
	
	if(!f)
	{
		free(item);
		return -1;
	}
	
	inserido = malloc(sizeof(struct fpnodo_t));
	if(!inserido)
		return -1;
	
	inserido->item = item;
	inserido->tipo = tipo;
	inserido->prio = prio;
	
	if(f->num == 0 || inserido->prio < f->prim->prio)// insere no inicio
	{
		//se tem item na lista e o primeiro item eh igual ao novo item
		if((f->prim != NULL) && f->prim->item == item)
		{
			free(inserido);
			return -1;
		}
		
		f->num++;
		inserido->prox = f->prim;
		f->prim = inserido;
		return f->num;
	}
	
	//verifica se ha item repetido
	anterior = f->prim;
	while(anterior != NULL)
	{
		if(anterior->item == item)
		{
			free(inserido);
			return -1;
		}
		anterior = anterior->prox;
	}
	
	//anterior aponta para ultimo da fila com a prioridade menor
	anterior = f->prim;
	while(anterior->prox && anterior->prox->prio <= inserido->prio)
		anterior = anterior->prox;
	
	//adiciona inserido a fila
	inserido->prox = anterior->prox;
	anterior->prox = inserido;
	
	f->num++;
	
	return f->num;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
	struct fpnodo_t *retirado;
	void *item;
	
	if(!f || f->num == 0 || !tipo || !prio)
		return NULL;
	
	f->num--;
	
	*tipo = f->prim->tipo;
	*prio = f->prim->prio;
	item = f->prim->item;
	
	retirado = f->prim;
	f->prim = f->prim->prox;
	retirado->prox = NULL;
	retirado->item = NULL;
	
	free(retirado);
	retirado = NULL;
	
	return item;
}

int fprio_tamanho (struct fprio_t *f)
{
	if(!f)
		return -1;
	return f->num;
}

void fprio_imprime (struct fprio_t *f)
{
	struct fpnodo_t *apontador;
	int i;
	
	if(!f || f->num == 0)
		return;
	
	apontador = f->prim;
	for(i = 0; i < f->num; i++)
	{
		printf("(%d %d)", apontador->tipo, apontador->prio);
		if( !(apontador->prox == NULL) )
			printf(" ");
		apontador = apontador->prox;
	}
	
	return;
}
