#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "comunes.h"

typedef struct sNodoArbol {
    void* info;
    unsigned tamInfo;
    struct sNodoArbol* izq;
    struct sNodoArbol* der;
} tNodoArbol;

typedef tNodoArbol* tArbol;

/// ========================================================================================================
/// TIPOS DE FUNCIONES AUXILIARES (CALLBACKS)
/// ========================================================================================================
typedef int (*tCmp)(const void* a, const void* b);
typedef void (*tAccion)(void *info, void *param);
typedef void (*tAccionRec)(void *info, unsigned tamInfo, unsigned nivel, void *param);
typedef void (*tMostrar)(const void*);

/// ========================================================================================================
/// INICIALIZACIÓN E INSERCIÓN
/// ========================================================================================================
void crearArbol(tArbol* arbol);
int insArb(tArbol *arbol, void *info, unsigned tamInfo, tCmp cmpClave, tAccion actualizar);

/// ========================================================================================================
/// RECORRIDOS
/// ========================================================================================================
void recorrerPre(const tArbol *arbol, tAccionRec accion, void *param);
void recorrerIn(const tArbol *arbol, tAccionRec accion, void *param);
void recorrerPos(const tArbol *arbol, tAccionRec accion, void *param);

void recorrerPreInv(const tArbol *arbol, tAccionRec accion, void *param);
void recorrerInInv(const tArbol *arbol, tAccionRec accion, void *param);
void recorrerPosInv(const tArbol *arbol, tAccionRec accion, void *param);

/// ========================================================================================================
/// CONTEOS Y ESTADÍSTICAS DEL ÁRBOL
/// ========================================================================================================
unsigned alturaArbol(const tArbol *arbol);
int esCompletoHastaNivelArbol(const tArbol *arbol, unsigned n);
int esCompletoArbol(const tArbol *arbol);
int esAVLArbol(const tArbol *arbol);

/// ========================================================================================================
/// CONTEOS Y ESTADÍSTICAS DE NODOS
/// ========================================================================================================
unsigned cantAbuelosArbol(const tArbol* arbol);
unsigned cantNodosDosHijos(const tArbol* arbol);
unsigned cantHijosSoloDer(const tArbol* arbol);

unsigned cantNodosArbol(const tArbol *arbol);
unsigned cantHojasArbol(const tArbol* arbol);
unsigned cantNoHojasArbol(const tArbol* arbol);

unsigned cantNodosHastaNivel(const tArbol* arbol, unsigned nivel);
unsigned cantNodosEnNivel(const tArbol* arbol, unsigned nivel);
unsigned cantNodosAPartirDeNivel(const tArbol* arbol, unsigned nivel);

unsigned cantHijosIzq(const tArbol* arbol);
unsigned cantHijosSoloIzq(const tArbol* arbol);

/// ========================================================================================================
/// BÚSQUEDA
/// ========================================================================================================
tArbol* busqNodoArbol(tArbol* arbol, void* clave, tCmp cmpClave);
int busqElemArbol(const tArbol* arbol, void* destino, unsigned tamDestino, tCmp cmpClave);
unsigned cantNodosSubArbol(const tArbol* arbol, void* clave, tCmp cmpClave);

/// ========================================================================================================
/// MÍNIMOS Y MÁXIMOS
/// ========================================================================================================
tArbol* busqNodoMaxArbol(tArbol *arbol);
tArbol* busqNodoMinArbol(tArbol *arbol);
int busqElemMaxArbol(const tArbol *arbol, void *info, unsigned tamInfo);
int busqElemMinArbol(const tArbol *arbol, void *info, unsigned tamInfo);

/// ========================================================================================================
/// ELIMINACIÓN Y PODAS
/// ========================================================================================================
int eliminarRaiz(tArbol* arbol);
int eliminarNodo(tArbol* arbol, void* clave, tCmp cmpClave);
void eliminarArbol(tArbol* arbol);

void podarHojasArbol(tArbol* arbol);
void podarArbolHastaNivel(tArbol* arbol, unsigned nivel);

/// ========================================================================================================
/// UTILIDADES
/// ========================================================================================================
void mostrarArbolGrafico(const tArbol *arbol, int nivel, tMostrar mostrar);
void espejarArbol(tArbol* arbol);

#endif // ARBOL_H_INCLUDED
