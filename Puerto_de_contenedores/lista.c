#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void crearLista(tLista* p)
{
    *p = NULL;
}

int vaciarLista(tLista* p)
{
    int cant = 0;
    while(*p)
    {
        tNodoLista* aux = *p;

        cant++;
        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
    return cant;
}

int vaciarListaYMostrar(tLista* p, void (*Mostrar)(const void* elem, FILE* arch), FILE* fp)
{
    int cant = 0;
    while(*p)
    {
        tNodoLista* aux = *p;

        cant++;
        *p = aux->sig;

        if(Mostrar && fp)
            Mostrar(aux->info, fp);

        free(aux->info);
        free(aux);
    }
    return cant;
}

int listaLlena(const tLista* p, unsigned cantBytes)
{
    tNodoLista* aux = malloc(sizeof(tNodoLista));
    void* info = malloc(cantBytes);

    free(aux);
    free(info);

    return aux == NULL || info == NULL;
}

int listaVacia(const tLista* p)
{
    return *p == NULL;
}

int ponerEnOrden(tLista* p, const void* d, unsigned cantBytes,
                 int(*Comparar)(const void*, const void*),
                 int(*Acumular)(void**, unsigned*, const void*, unsigned))
{
    tNodoLista* nuevo;

    while(*p && Comparar((*p)->info, d) < 0)
        p = &(*p)->sig;

    if(*p && Comparar((*p)->info, d) == 0)
    {
        if(Acumular)
            if(!Acumular(&(*p)->info, &(*p)->tamInfo, d, cantBytes))
                return SIN_MEM;
        return DUPLICADO;
    }

    if((nuevo = malloc(sizeof(tNodoLista))) == NULL || (nuevo->info = malloc(cantBytes)) == NULL)
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

int ponerAlComienzo(tLista* p, const void* d, unsigned cantBytes)
{
    tNodoLista* nuevo;

    if((nuevo = malloc(sizeof(tNodoLista))) == NULL ||
            (nuevo->info = malloc(cantBytes)) ==  NULL)
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

int ponerAlFinal(tLista* p, const void* d, unsigned cantBytes)
{
    tNodoLista* nuevo;

    while(*p)
        p = &(*p)->sig;

    if((nuevo = malloc(sizeof(tNodoLista))) == NULL ||
            (nuevo->info = malloc(cantBytes)) == NULL)
    {
        free(nuevo);
        return SIN_MEM;
    }

    memcpy(nuevo->info, d, cantBytes);
    nuevo->tamInfo = cantBytes;
    nuevo->sig = NULL;
    *p = nuevo;

    return EXITO;
}

int sacarPrimeroLista(tLista* p, void* d, unsigned cantBytes)
{
    tNodoLista* aux = *p;

    if(aux == NULL)
        return LISTA_VACIA;

    *p = aux->sig;
    memcpy(d, aux->info, MINIMO(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);

    return EXITO;
}

int sacarUltimoLista(tLista* p, void* d, unsigned cantBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;

    while((*p)->sig)
        p = &(*p)->sig;

    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    free((*p)->info);
    free(*p);
    *p = NULL;

    return EXITO;
}

int verPrimeroLista(const tLista* p, void* d, unsigned cantBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return EXITO;
}

int verUltimoLista(const tLista* p, void* d, unsigned cantBytes)
{
    if(*p == NULL)
        return LISTA_VACIA;

    while((*p)->sig)
        p = &(*p)->sig;

    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));

    return EXITO;
}

void ordenarLista(tLista* p, int(*Comparar)(const void*, const void*))
{
    tLista* pri =p;

    if(*p == NULL)
        return;

    while((*p)->sig)
    {
        if(Comparar((*p)->info, (*p)->sig->info) > 0)
        {
            tLista* q = pri;
            tNodoLista* aux = (*p)->sig;

            (*p)->sig = aux->sig;

            while(Comparar((*q)->info, aux->info) <=  0)
                q = &(*q)->sig;

            aux->sig = *q;
            *q = aux;
        }
        else
            p = &(*p)->sig;
    }
}

int mostrarLista(tLista* p, tMostrarLista mostrar, FILE* fp)
{
    int cant = 0;

    while(*p)
    {

        if(!mostrar)
        {
            mostrar((*p)->info, fp);
        }

        p = &(*p)->sig;
        cant++;
    }
    return cant;
}

int mostrarListaAlReves(tLista* p, tMostrarLista mostrar, FILE* fp)
{
    if(*p)
    {
        int n = mostrarListaAlReves(&(*p)->sig, mostrar, fp);

        mostrar((*p)->info, fp);

        return n + 1;
    }
    return EXITO;
}

int mostrarListaAlRevesYVaciar(tLista* p, tMostrarLista mostrar, FILE* fp)
{
    if(*p)
    {
        int n = mostrarListaAlReves(&(*p)->sig, mostrar, fp);

        mostrar((*p)->info, fp);
        free((*p)->info);
        free(*p);
        *p = NULL;

        return n + 1;
    }
    return EXITO;
}


int sacarClaveLista(tLista* p, void* d, unsigned cantBytes, tCmp comparar)
{
    tNodoLista* aux;
    if(!*p)
        return LISTA_VACIA;

    while(*p && comparar((*p)->info, d)<0)
        p=&(*p)->sig;

    if(*p && comparar((*p)->info, d)==0)
    {
        memcpy(d,(*p)->info,cantBytes);
        aux=*p;
        *p=aux->sig;
        free(aux->info);
        free(aux);

        return EXITO;
    }
    return NO_ENCONTRADO;
}


int busqClaveLista(tLista *p, tCmp comparar, void* clave)
{

    int pos = 0;

    while(*p)
    {
        if(comparar((*p)->info, clave) == 0)
        {
            return pos;
        }
        p = &(*p)->sig;
        pos++;
    }

    return NO_ENCONTRADO;
}



int sacarPosLista(tLista *p, void *info, unsigned tamInfo, unsigned pos)
{
    tNodoLista *aux;

    while(*p && pos)
    {
        p = &(*p)->sig;
        pos--;
    }

    if(!*p || pos)
        return NO_ENCONTRADO;


    aux = *p;
    *p = aux->sig;

    memcpy(info, aux->info, MINIMO(tamInfo, aux->tamInfo));

    free(aux->info);
    free(aux);
    return EXITO;
}


void mapLista(tLista* p, tAccionLista accion, void* parametro)
{
    if(!accion)
        return;

    while(*p)
    {
        accion((*p)->info,parametro);
        p = &(*p)->sig;
    }
}
