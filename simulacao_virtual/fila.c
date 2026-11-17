#include <stdio.h>
#include <stdlib.h>
#include "fila.h"


struct fila_t *fila_cria ()
{
  struct fila_t *fila;
  
  fila = malloc(sizeof(struct fila_t));
  if(!fila)
    return NULL;

  fila->prim = NULL;
  fila->ult = NULL;
  fila->num = 0;

  return fila;
}


struct fila_t *fila_destroi (struct fila_t *f)
{
  int descarte;
  
  if(!f)
    return NULL;
  
  while(f->num > 0)
    descarte = fila_retira(f, &descarte);
    
  f->prim = NULL;
  f->ult = NULL;
  
  free(f);
  f = NULL;
  
  return NULL;
}


int fila_insere (struct fila_t *f, int item)
{
  struct fila_nodo_t *inserido, *andarilho;
	
	if(!f)
		return 0;
	
	inserido = malloc(sizeof(struct fila_nodo_t));
	if(!inserido)
		return 0;
	
	inserido->item = item;
  inserido->prox = NULL;
	
	//verifica se ha item repetido
	andarilho = f->prim;
	while(andarilho != NULL)
	{
		if(andarilho->item == item)
		{
			free(inserido);
      
      andarilho = NULL;
			return 0;
		}
		andarilho = andarilho->prox;
	}

  if(f->prim == NULL)//fila vazia
    f->prim = inserido;
  else
    f->ult->prox = inserido;
  f->ult = inserido;
  
	f->num++;
	return 1;
}


int fila_retira (struct fila_t *f, int *item)
{
	struct fila_nodo_t *retirado;
	
	if(!f || f->num == 0)
		return 0;
	
	f->num--;
	*item = f->prim->item;
	
	retirado = f->prim;
	f->prim = f->prim->prox;
	if(f->prim == NULL)
		f->ult = NULL;
	retirado->prox = NULL;
	
	free(retirado);
	retirado = NULL;
	
	return 1;
}


int fila_tamanho (struct fila_t *f)
{
  if(!f || f->num < 0)
    return -1;
  return f->num;
}


void fila_imprime (struct fila_t *f)
{
  struct fila_nodo_t *apontador;
	
	if(!f)
		return;
	
	apontador = f->prim;
	while(apontador != NULL)
	{
		printf("%d", apontador->item);
    apontador = apontador->prox;
		if(apontador)
			printf(" ");
	}
	
	return;
}
