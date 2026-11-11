#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include "comunes.h"
typedef struct sNodoCola
{
    void* info;
    unsigned tamInfo;
    struct sNodoCola *sig;
}tNodoCola;

typedef struct
{
    tNodoCola *prim,
         *ult;
}tCola;
void crearCola(tCola *p);
void vaciarCola(tCola *p);
int colaVacia(const tCola *p);
int ponerEnCola(tCola *p, const void *d, unsigned tamInfo);
int verPrimeroCola(const tCola *p, void* d, unsigned tamInfo);
int sacarDeCola(tCola* p, void* d, unsigned tamInfo);

#endif // COLA_H_INCLUDED
