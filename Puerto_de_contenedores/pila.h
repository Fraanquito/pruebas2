#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED
#include "comunes.h"
typedef struct sNodoPila
{
    void            *info;
    unsigned        tamInfo;
    struct sNodoPila    *sig;
}tNodoPila;

typedef tNodoPila *tPila;

void crearPila(tPila *p);
int pilaLlena(const tPila *p, unsigned cantBytes);
int ponerEnPila(tPila *p, const void *d, unsigned cantBytes);
int verTope(const tPila *p, void *d, unsigned cantBytes);
int pilaVacia(const tPila *p);
int sacarDePila(tPila *p, void *d, unsigned cantBytes);
void vaciarPila(tPila *p);

#endif // PILA_H_INCLUDED
