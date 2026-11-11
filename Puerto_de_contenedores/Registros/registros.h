#ifndef REGISTROS_H_INCLUDED
#define REGISTROS_H_INCLUDED

#include "../arbol.h"

#define ARCHIVO_OPERADORES "operadores.dat"
#define ARCHIVO_INDICE     "indice_operadores.idx"
#define ARCHIVO_JORNADAS   "jornadas.dat"

#define OPERADOR_EQUIVOCADO -10

typedef struct
{
    char nombre[50];
    long offset;
} tRegIndice;

typedef struct
{
    char nombre[50];
    int jornadasJugadas;
    int puntajeTotal;
} tOperador;

typedef struct
{
    char nombreOperador[50];
    int puntajeFinal;
    int buquesDescargados;
    int contenedoresEntregados;
    int reubicacionesRealizadas;
    int camionesPendientes;
    int tiempoUtilizado;
    char motivoFinalizacion[30];
} tJornada;

int cmpIndiceOperador(const void *a, const void *b);
int cmpOperadorRanking(const void *a, const void *b);
void accionGuardarIndice(void *info, unsigned tamInfo, unsigned nivel, void *param);
int guardarIndiceOrdenado(const char *nomArchIndice, const tArbol *arbol);
int _cargarIndiceBalanceado(tArbol *arbol, FILE *fp, int li, int ls);
int levantarIndice(const char *nomArchIndice, tArbol *arbol);
int loginOperador(const char *nombreBuscado, tOperador *operadorActual, long *offsetActual, tArbol *indice);
int actualizarOperador(tOperador *operadorModificado, long offsetOperador);
int guardarJornadaBD(tJornada *nuevaJornada);
void mostrarRankingOperadores();
void mostrarHistorialJornadas();

#endif // REGISTROS_H_INCLUDED
