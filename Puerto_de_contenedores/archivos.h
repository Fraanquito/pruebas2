#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "comunes.h"

typedef struct {
    int duracionJornadaMinutos;
    int cantidadMuelles;
    int cantidadZonasAlmacenamiento;
    int capacidadPila;
    int maximoBuques;
    int maximoContenedoresPorBuque;
    int maximoCamiones;
    int tiempoDescargaContenedor;
    int tiempoReubicacionContenedor;
    int tiempoCargaCamion;
} tConfiguracion;

void limpiarSaltoLinea(char* cadena);
int cargarConfiguracion(const char* ruta, tConfiguracion* config);
int generarEscenario(const char* ruta, const tConfiguracion* config);

#endif // ARCHIVOS_H_INCLUDED
