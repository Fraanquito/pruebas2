#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "arbol.h"
/// PROTOTIPOS DE FUNCIONES PRIVADAS
void _recorrerPre(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel);
void _recorrerIn(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel);
void _recorrerPos(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel);
void _recorrerPreInv(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel);
void _recorrerInInv(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel);
void _recorrerPosInv(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel);

unsigned _cantNodosEnNivel(const tArbol* arbol, unsigned nivelActual, unsigned nivelObjetivo);
unsigned _cantNodosAPartirDeNivel(const tArbol* arbol, unsigned nivelActual, unsigned nivelObjetivo);
/// INICIALIZACIÓN E INSERCIÓN

void crearArbol(tArbol* arbol)
{
    *arbol = NULL;
}

int insArb(tArbol *arbol, void *info, unsigned tamInfo, tCmp cmpClave, tAccion actualizar)
{
    tNodoArbol *nue;
    int resCmp;

    while(*arbol)
    {
        if((resCmp = cmpClave(info, (*arbol)->info)) < 0)
            arbol = &(*arbol)->izq;
        else if(resCmp > 0)
            arbol = &(*arbol)->der;
        else
        {
            if(actualizar)
                actualizar((*arbol)->info, info);
            return DUPLICADO;
        }
    }

    if(!(nue = malloc(sizeof(tNodoArbol))))
        return SIN_MEM;

    if(!(nue->info = malloc(tamInfo)))
    {
        free(nue);
        return SIN_MEM;
    }

    nue->tamInfo = tamInfo;
    memcpy(nue->info, info, tamInfo);
    nue->izq = NULL;
    nue->der = NULL;

    *arbol = nue;
    return EXITO;
}
/// RECORRIDOS


void _recorrerPre(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel)
{
    if(!*arbol) return;
    accion((*arbol)->info, (*arbol)->tamInfo, nivel, param);
    _recorrerPre((const tArbol*)&(*arbol)->izq, accion, param, nivel+1);
    _recorrerPre((const tArbol*)&(*arbol)->der, accion, param, nivel+1);
}
void recorrerPre(const tArbol *arbol, tAccionRec accion, void *param) { _recorrerPre(arbol, accion, param, 0); }

void _recorrerIn(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel)
{
    if(!*arbol) return;
    _recorrerIn((const tArbol*)&(*arbol)->izq, accion, param, nivel+1);
    accion((*arbol)->info, (*arbol)->tamInfo, nivel, param);
    _recorrerIn((const tArbol*)&(*arbol)->der, accion, param, nivel+1);
}
void recorrerIn(const tArbol *arbol, tAccionRec accion, void *param) { _recorrerIn(arbol, accion, param, 0); }

void _recorrerPos(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel)
{
    if(!*arbol) return;
    _recorrerPos((const tArbol*)&(*arbol)->izq, accion, param, nivel+1);
    _recorrerPos((const tArbol*)&(*arbol)->der, accion, param, nivel+1);
    accion((*arbol)->info, (*arbol)->tamInfo, nivel, param);
}
void recorrerPos(const tArbol *arbol, tAccionRec accion, void *param) { _recorrerPos(arbol, accion, param, 0); }

void _recorrerPreInv(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel)
{
    if(!*arbol) return;
    accion((*arbol)->info, (*arbol)->tamInfo, nivel, param);
    _recorrerPreInv((const tArbol*)&(*arbol)->der, accion, param, nivel+1);
    _recorrerPreInv((const tArbol*)&(*arbol)->izq, accion, param, nivel+1);
}
void recorrerPreInv(const tArbol *arbol, tAccionRec accion, void *param) { _recorrerPreInv(arbol, accion, param, 0); }

void _recorrerInInv(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel)
{
    if(!*arbol) return;
    _recorrerInInv((const tArbol*)&(*arbol)->der, accion, param, nivel+1);
    accion((*arbol)->info, (*arbol)->tamInfo, nivel, param);
    _recorrerInInv((const tArbol*)&(*arbol)->izq, accion, param, nivel+1);
}
void recorrerInInv(const tArbol *arbol, tAccionRec accion, void *param) { _recorrerInInv(arbol, accion, param, 0); }

void _recorrerPosInv(const tArbol *arbol, tAccionRec accion, void *param, unsigned nivel)
{
    if(!*arbol) return;
    _recorrerPosInv((const tArbol*)&(*arbol)->der, accion, param, nivel+1);
    _recorrerPosInv((const tArbol*)&(*arbol)->izq, accion, param, nivel+1);
    accion((*arbol)->info, (*arbol)->tamInfo, nivel, param);
}
void recorrerPosInv(const tArbol *arbol, tAccionRec accion, void *param) { _recorrerPosInv(arbol, accion, param, 0); }
/// CONTEOS Y ESTADÍSTICAS DEL ÁRBOL


unsigned alturaArbol(const tArbol *arbol)
{
    if(!*arbol) return 0;
    return MAXIMO(alturaArbol((const tArbol*)&(*arbol)->izq), alturaArbol((const tArbol*)&(*arbol)->der)) + 1;
}

int esCompletoHastaNivelArbol(const tArbol *arbol, unsigned n)
{
    if(!*arbol) return 0;
    if(!n) return 1;
    return esCompletoHastaNivelArbol((const tArbol*)&(*arbol)->izq, n-1) &&
           esCompletoHastaNivelArbol((const tArbol*)&(*arbol)->der, n-1);
}

int esCompletoArbol(const tArbol *arbol)
{
    if (!arbol || !*arbol) return 1;
    return esCompletoHastaNivelArbol(arbol, alturaArbol(arbol) - 1);
}

int esAVLArbol(const tArbol *arbol)
{
    if (!*arbol) return 1;

    unsigned hi = alturaArbol((const tArbol*)&(*arbol)->izq);
    unsigned hd = alturaArbol((const tArbol*)&(*arbol)->der);

    if (abs((int)hi - (int)hd) > 1) return 0;

    return esAVLArbol((const tArbol*)&(*arbol)->izq) && esAVLArbol((const tArbol*)&(*arbol)->der);
}
/// CONTEOS Y ESTADÍSTICAS DE NODOS


unsigned cantAbuelosArbol(const tArbol* arbol)
{
    if (!*arbol) return 0;
    int esAbuelo = ((*arbol)->izq && ((*arbol)->izq->izq || (*arbol)->izq->der)) ||
                   ((*arbol)->der && ((*arbol)->der->izq || (*arbol)->der->der)) ? 1 : 0;

    return esAbuelo + cantAbuelosArbol((const tArbol*)&(*arbol)->izq) + cantAbuelosArbol((const tArbol*)&(*arbol)->der);
}

unsigned cantNodosDosHijos(const tArbol* arbol)
{
    if (!*arbol) return 0;
    int dosHijos = ((*arbol)->izq && (*arbol)->der) ? 1 : 0;
    return dosHijos + cantNodosDosHijos((const tArbol*)&(*arbol)->izq) + cantNodosDosHijos((const tArbol*)&(*arbol)->der);
}

unsigned cantHijosSoloDer(const tArbol* arbol)
{
    if (!*arbol) return 0;
    int soloDer = (!(*arbol)->izq && (*arbol)->der) ? 1 : 0;
    return soloDer + cantHijosSoloDer((const tArbol*)&(*arbol)->izq) + cantHijosSoloDer((const tArbol*)&(*arbol)->der);
}

unsigned cantNodosArbol(const tArbol *arbol)
{
    if(!*arbol) return 0;
    return cantNodosArbol((const tArbol*)&(*arbol)->izq) + cantNodosArbol((const tArbol*)&(*arbol)->der) + 1;
}

unsigned cantHojasArbol(const tArbol* arbol)
{
    if (!*arbol) return 0;
    if (!(*arbol)->izq && !(*arbol)->der) return 1;
    return cantHojasArbol((const tArbol*)&(*arbol)->izq) + cantHojasArbol((const tArbol*)&(*arbol)->der);
}

unsigned cantNoHojasArbol(const tArbol* arbol)
{
    if (!*arbol) return 0;
    if ((*arbol)->izq || (*arbol)->der)
        return 1 + cantNoHojasArbol((const tArbol*)&(*arbol)->izq) + cantNoHojasArbol((const tArbol*)&(*arbol)->der);
    return 0;
}

unsigned cantNodosHastaNivel(const tArbol* arbol, unsigned nivel)
{
    if (!*arbol) return 0;
    if(!nivel) return 1;
    return 1 + cantNodosHastaNivel((const tArbol*)&(*arbol)->izq, nivel - 1) +
               cantNodosHastaNivel((const tArbol*)&(*arbol)->der, nivel - 1);
}

unsigned cantNodosEnNivel(const tArbol* arbol, unsigned nivel)
{
    return _cantNodosEnNivel(arbol, 0, nivel);
}

unsigned _cantNodosEnNivel(const tArbol* arbol, unsigned nivelActual, unsigned nivelObjetivo)
{
    if (!*arbol) return 0;
    if (nivelActual == nivelObjetivo) return 1;
    return _cantNodosEnNivel((const tArbol*)&(*arbol)->izq, nivelActual + 1, nivelObjetivo) +
           _cantNodosEnNivel((const tArbol*)&(*arbol)->der, nivelActual + 1, nivelObjetivo);
}

unsigned cantNodosAPartirDeNivel(const tArbol* arbol, unsigned nivel)
{
    return _cantNodosAPartirDeNivel(arbol, 0, nivel);
}

unsigned _cantNodosAPartirDeNivel(const tArbol* arbol, unsigned nivelActual, unsigned nivelObjetivo)
{
    if (!*arbol) return 0;
    unsigned cont = nivelActual >= nivelObjetivo ? 1 : 0;
    return cont + _cantNodosAPartirDeNivel((const tArbol*)&(*arbol)->izq, nivelActual + 1, nivelObjetivo) +
                  _cantNodosAPartirDeNivel((const tArbol*)&(*arbol)->der, nivelActual + 1, nivelObjetivo);
}

unsigned cantHijosIzq(const tArbol* arbol)
{
    if(!*arbol) return 0;
    int cont = (*arbol)->izq ? 1 : 0;
    return cantHijosIzq((const tArbol*)&(*arbol)->izq) + cantHijosIzq((const tArbol*)&(*arbol)->der) + cont;
}

unsigned cantHijosSoloIzq(const tArbol* arbol)
{
    if (!*arbol) return 0;
    int soloIzq = ((*arbol)->izq && !(*arbol)->der) ? 1 : 0;
    return soloIzq + cantHijosSoloIzq((const tArbol*)&(*arbol)->izq) + cantHijosSoloIzq((const tArbol*)&(*arbol)->der);
}
/// BÚSQUEDA


tArbol* busqNodoArbol(tArbol* arbol, void* clave, tCmp cmpClave)
{
    if (!*arbol) return NULL;
    int resCmp = cmpClave(clave, (*arbol)->info);

    if (resCmp < 0) return busqNodoArbol(&(*arbol)->izq, clave, cmpClave);
    if (resCmp > 0) return busqNodoArbol(&(*arbol)->der, clave, cmpClave);

    return arbol;
}

int busqElemArbol(const tArbol* arbol, void* destino, unsigned tamDestino, tCmp cmpClave)
{
    tArbol* hallado = busqNodoArbol((tArbol*)arbol, destino, cmpClave);
    if (!hallado) return NO_ENCONTRADO;

    memcpy(destino, (*hallado)->info, MINIMO(tamDestino, (*hallado)->tamInfo));
    return EXITO;
}

unsigned cantNodosSubArbol(const tArbol* arbol, void* clave, tCmp cmpClave)
{
    tArbol* hallado = busqNodoArbol((tArbol*)arbol, clave, cmpClave);
    if(!hallado) return 0;
    return cantNodosArbol((const tArbol*)hallado);
}
/// MÍNIMOS Y MÁXIMOS


tArbol* busqNodoMaxArbol(tArbol *arbol)
{
    if (!arbol || !*arbol) return NULL;
    while ((*arbol)->der) arbol = &(*arbol)->der;
    return arbol;
}

tArbol* busqNodoMinArbol(tArbol *arbol)
{
    if (!arbol || !*arbol) return NULL;
    while ((*arbol)->izq) arbol = &(*arbol)->izq;
    return arbol;
}

int busqElemMaxArbol(const tArbol *arbol, void *info, unsigned tamInfo)
{
    tArbol* max = busqNodoMaxArbol((tArbol*)arbol);
    if(!max) return ARBOL_VACIO;

    memcpy(info, (*max)->info, MINIMO(tamInfo, (*max)->tamInfo));
    return EXITO;
}

int busqElemMinArbol(const tArbol *arbol, void *info, unsigned tamInfo)
{
    tArbol* min = busqNodoMinArbol((tArbol*)arbol);
    if(!min) return ARBOL_VACIO;

    memcpy(info, (*min)->info, MINIMO(tamInfo, (*min)->tamInfo));
    return EXITO;
}
/// ELIMINACIÓN Y PODAS

int eliminarRaiz(tArbol* arbol)
{
    if (!*arbol) return NO_ENCONTRADO;
    free((*arbol)->info);

    if (!(*arbol)->izq && !(*arbol)->der)
    {
        free(*arbol);
        *arbol = NULL;
        return EXITO;
    }

    tArbol *remp = alturaArbol((const tArbol*)&(*arbol)->izq) > alturaArbol((const tArbol*)&(*arbol)->der)
            ? busqNodoMaxArbol(&(*arbol)->izq)
            : busqNodoMinArbol(&(*arbol)->der);

    tNodoArbol *elim = *remp;

    (*arbol)->info = elim->info;
    (*arbol)->tamInfo = elim->tamInfo;
    *remp = elim->izq ? elim->izq : elim->der;

    free(elim);
    return EXITO;
}

int eliminarNodo(tArbol* arbol, void* clave, tCmp cmpClave)
{
    tArbol* hallado = busqNodoArbol(arbol, clave, cmpClave);
    if (!hallado) return NO_ENCONTRADO;
    return eliminarRaiz(hallado);
}

void eliminarArbol(tArbol* arbol)
{
    if (!*arbol) return;
    eliminarArbol(&(*arbol)->izq);
    eliminarArbol(&(*arbol)->der);

    free((*arbol)->info);
    free(*arbol);
    *arbol = NULL;
}

void podarHojasArbol(tArbol* arbol)
{
    if (!*arbol) return;

    if (!(*arbol)->izq && !(*arbol)->der)
    {
        free((*arbol)->info);
        free(*arbol);
        *arbol = NULL;
        return;
    }

    podarHojasArbol(&(*arbol)->izq);
    podarHojasArbol(&(*arbol)->der);
}

void podarArbolHastaNivel(tArbol* arbol, unsigned nivel)
{
    if (!*arbol) return;

    if (nivel == 0)
    {
        eliminarArbol(arbol);
        return;
    }

    podarArbolHastaNivel(&(*arbol)->izq, nivel - 1);
    podarArbolHastaNivel(&(*arbol)->der, nivel - 1);
}
/// UTILIDADES

void mostrarArbolGrafico(const tArbol *arbol, int nivel, tMostrar mostrar)
{
    if(!*arbol) return;

    mostrarArbolGrafico((const tArbol*)&(*arbol)->der, nivel + 1, mostrar);

    for(int i = 0; i < nivel; i++)
        printf("\t");

    mostrar((*arbol)->info);
    printf("\n");

    mostrarArbolGrafico((const tArbol*)&(*arbol)->izq, nivel + 1, mostrar);
}

void espejarArbol(tArbol* arbol)
{
    if (!*arbol) return;

    tNodoArbol* aux = (*arbol)->izq;
    (*arbol)->izq = (*arbol)->der;
    (*arbol)->der = aux;

    espejarArbol(&(*arbol)->izq);
    espejarArbol(&(*arbol)->der);
}
