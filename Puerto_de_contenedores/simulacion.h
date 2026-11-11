#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED


#include "comunes.h"
#include "archivos.h"
#include "cola.h"
#include "pila.h"
#include "lista.h"

#define NO_OP_VALIDAS   -10

#define SIM_EN_CURSO    10
#define SIM_FIN_NORMAL  11
#define SIM_FIN_BLOQUEO 12
#define SIM_FIN_TIEMPO  13
#define SIM_FIN_FORZADO  14

typedef struct
{
    char codigo[25];
} tContenedor;

typedef struct
{
    char id[25];
    int tArribo;
    tCola contenedores; // Cola FIFO con el manifiesto de carga del buque
} tBuque;

typedef struct
{
    char id[25];
    int tArribo;
    char contenedorPedido[25];
} tCamion;


typedef struct
{
    int idMuelle;
    int ocupado;        // 0 = Libre, 1 = Ocupado
    tBuque buqueActual;
} tMuelle;

typedef struct
{
    int idZona;
    int cantidadActual;
    int capacidadMaxima;
    tPila pilaContenedores;
} tZona;


typedef struct
{
    int relojActual;

    int duracionJornada;
    int cantidadMuelles;
    int cantidadZonas;
    int capacidadPila;

    int tiempoDescarga;
    int tiempoReubicacion;
    int tiempoCarga;

    tCola colaBuques;
    tCola colaCamiones;

    tLista muelles;
    tLista zonas;
    tLista historialOp;

    int buquesDescargados;
    int contenedoresEntregados;
    int reubicacionesRealizadas;
} tPuerto;

typedef struct
{
    int tInicio;
    int tFin;
    char comando[4];
    char contenedor[25];
    char origen[15];
    char destino[15];
} tMovimiento;



int inicializarPuerto(tPuerto* puerto, const char* rutaEscenario, const tConfiguracion* config);
int ejecutarSimulacion(tPuerto* puerto);
int actualizarEstadoPuerto(tPuerto* puerto);
void mostrarResumenFinal(tPuerto* puerto);
int liberarPuerto(tPuerto* puerto);

int contarCamionesPendientes(tPuerto* puerto);
void ejecutarDES(tPuerto* puerto, char* idMuelle, char* idZona);
void ejecutarREU(tPuerto* puerto, char* idZonaOrigen, char* idZonaDestino);
void ejecutarENT(tPuerto* puerto);
void ejecutarVER(tPuerto* puerto);
void ejecutarESP(tPuerto* puerto);



#endif // TIPOS_H_INCLUDED
