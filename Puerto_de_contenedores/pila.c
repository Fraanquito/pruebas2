#include <stdlib.h>
#include <string.h>
#include "pila.h"

void crearPila(tPila *p)
{
    *p = NULL;
}
int pilaLlena(const tPila *p, unsigned cantBytes)
{
    tNodoPila *aux = (tNodoPila*)malloc(sizeof(tNodoPila));
    void *info = malloc(cantBytes);

    free(aux);
    free(info);

    return aux == NULL || info == NULL;
}
int ponerEnPila(tPila *p, const void *d, unsigned cantBytes)
{
    tNodoPila *nuevo;

    if((nuevo = malloc(sizeof(tNodoPila))) == NULL || (nuevo->info = malloc(cantBytes)) == NULL)
    {
        free(nuevo);
        return SIN_MEM;
    }

    memcpy(nuevo->info, d, cantBytes);
    nuevo->tamInfo = cantBytes;
    nuevo->sig = *p;
    *p = nuevo;

    return EXITO;
}
int verTope(const tPila *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return PILA_VACIA;

    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return EXITO;
}
int pilaVacia(const tPila *p)
{
    return *p == NULL;
}
int sacarDePila(tPila *p, void *d, unsigned cantBytes)
{
     tNodoPila* aux = *p;

    if(aux == NULL)
        return PILA_VACIA;

    *p = aux->sig;

    memcpy(d, aux->info, MINIMO(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);

    return EXITO;
}
void vaciarPila(tPila *p)
{
    while(*p)
    {
        tNodoPila* aux = *p;

        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}
