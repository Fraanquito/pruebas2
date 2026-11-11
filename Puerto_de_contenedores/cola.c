#include <stdlib.h>
#include <string.h>
#include "cola.h"

void crearCola(tCola *p)
{
    p->prim = NULL;
    p->ult = NULL;
}

int colaVacia(const tCola *p)
{
    return p->prim == NULL;
}

int ponerEnCola(tCola *p, const void *d, unsigned cantBytes)
{
    tNodoCola *aux = malloc(sizeof(tNodoCola));
    if(!aux)
        return SIN_MEM;

    aux->info = malloc(cantBytes);
    if(!aux->info)
    {
        free(aux);
        return SIN_MEM;
    }

    memcpy(aux->info, d, cantBytes);
    aux->tamInfo = cantBytes;
    aux->sig = NULL;

    if(p->ult)
        p->ult->sig = aux;
    else
        p->prim = aux;

    p->ult = aux;
    return EXITO;
}

int sacarDeCola(tCola* p, void* d, unsigned cantBytes)
{
    tNodoCola *aux = p->prim;

    if(!aux)
        return COLA_VACIA;

    memcpy(d, aux->info, MINIMO(aux->tamInfo, cantBytes));
    p->prim = p->prim->sig;
    free(aux->info);
    free(aux);

    if(!p->prim)
    {
        p->ult = NULL;
    }
    return EXITO;
}

int verPrimeroCola(const tCola* p, void* d, unsigned cantBytes)
{
    if(!p->prim)
        return COLA_VACIA;

    memcpy(d, p->prim->info, MINIMO(p->prim->tamInfo, cantBytes));
    return EXITO;
}

void vaciarCola(tCola* p)
{
    tNodoCola *aux;

    while(p->prim)
    {
        aux = p->prim;
        p->prim = aux->sig;
        free(aux->info);
        free(aux);

    }
    p->ult = NULL;

}
