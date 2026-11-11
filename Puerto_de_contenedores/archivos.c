#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "archivos.h"
#include "lista.h"


typedef struct
{
    char id[15];
    int tArribo;
    char manifiesto[512];
} tBuqueGen;

typedef struct
{
    char id[15];
    int tArribo;
    char contenedor[25];
} tCamionGen;

void limpiarSaltoLinea(char* cadena)
{
    char* ptr = cadena;

    while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r')
    {
        ptr++;
    }
    *ptr = '\0';
}

int cmpBuquesGen(const void* a, const void* b)
{
    tBuqueGen* b1 = (tBuqueGen*)a;
    tBuqueGen* b2 = (tBuqueGen*)b;

    int dif = b1->tArribo - b2->tArribo;
    if (dif == 0)
    {
        return strcmp(b1->id, b2->id);
    }
    return dif;
}

int cmpCamionesGen(const void* a, const void* b)
{
    tCamionGen* c1 = (tCamionGen*)a;
    tCamionGen* c2 = (tCamionGen*)b;

    int dif = c1->tArribo - c2->tArribo;
    if (dif == 0)
    {
        return strcmp(c1->id, c2->id);
    }
    return dif;
}

int cargarConfiguracion(const char* ruta, tConfiguracion* config)
{
    char linea[256];
    char clave[100];
    int valor;
    FILE* arch = fopen(ruta, "rt");
    if (!arch)
    {
        return ERR_ARCH;
    }

    ///default
    config->duracionJornadaMinutos = 30;
    config->cantidadMuelles = 1;
    config->cantidadZonasAlmacenamiento = 3;
    config->capacidadPila = 3;
    config->maximoBuques = 2;
    config->maximoContenedoresPorBuque = 3;
    config->maximoCamiones = 5;
    config->tiempoDescargaContenedor = 2;
    config->tiempoReubicacionContenedor = 1;
    config->tiempoCargaCamion = 2;

    while (fgets(linea, sizeof(linea), arch))
    {
        if (sscanf(linea, "%s: %d", clave, &valor) == 2)
        {
            if (strcmp(clave, "duracion_jornada_minutos") == 0)
                config->duracionJornadaMinutos = valor;
            else if (strcmp(clave, "cantidad_muelles") == 0)
                config->cantidadMuelles = valor;
            else if (strcmp(clave, "cantidad_zonas_almacenamiento") == 0)
                config->cantidadZonasAlmacenamiento = valor;
            else if (strcmp(clave, "capacidad_pila") == 0)
                config->capacidadPila = valor;
            else if (strcmp(clave, "maximo_buques") == 0)
                config->maximoBuques = valor;
            else if (strcmp(clave, "maximo_contenedores_por_buque") == 0)
                config->maximoContenedoresPorBuque = valor;
            else if (strcmp(clave, "maximo_camiones") == 0)
                config->maximoCamiones = valor;
            else if (strcmp(clave, "tiempo_descarga_contenedor") == 0)
                config->tiempoDescargaContenedor = valor;
            else if (strcmp(clave, "tiempo_reubicacion_contenedor") == 0)
                config->tiempoReubicacionContenedor = valor;
            else if (strcmp(clave, "tiempo_carga_camion") == 0)
                config->tiempoCargaCamion = valor;
        }
    }

    fclose(arch);
    return EXITO;
}

int generarEscenario(const char* ruta, const tConfiguracion* config)
{
    FILE* arch;
    tLista listaBuques, listaCamiones;
    int maxContenedores, totalContenedores, cantContenedores;
    int cantidadCamiones, indiceAleatorio;
    int i, j;
    tBuqueGen buque;
    tCamionGen camion;
    char (*loteria)[25];
    char codigoContenedor[25];

    arch = fopen(ruta, "wt");
    if (!arch)
        return ERR_ARCH;

    srand(time(NULL));

    crearLista(&listaBuques);
    crearLista(&listaCamiones);

    maxContenedores = config->maximoBuques * config->maximoContenedoresPorBuque;

    loteria = malloc(maxContenedores * sizeof(char[25]));
    if (!loteria)
    {
        fclose(arch);
        return SIN_MEM;
    }

    totalContenedores = 0;

    for (i = 0; i < config->maximoBuques; i++)
    {
        buque.tArribo = (i == 0) ? 0 : (rand() % config->duracionJornadaMinutos);
        sprintf(buque.id, "B%03d", i + 1);
        buque.manifiesto[0] = '\0';

        cantContenedores = (rand() % config->maximoContenedoresPorBuque) + 1;

        for (j = 1; j <= cantContenedores; j++)
        {
            sprintf(codigoContenedor, "C%d%02d", i + 1, j);

            if (j > 1) strcat(buque.manifiesto, ",");
            strcat(buque.manifiesto, codigoContenedor);

            strcpy(*(loteria+totalContenedores), codigoContenedor);
            totalContenedores++;
        }

        ponerEnOrden(&listaBuques, &buque, sizeof(tBuqueGen), cmpBuquesGen, NULL);
    }

    cantidadCamiones = MINIMO(config->maximoCamiones, totalContenedores);

    for (i = 0; i < cantidadCamiones; i++)
    {
        camion.tArribo = rand() % config->duracionJornadaMinutos;
        sprintf(camion.id, "K%03d", i + 1);

        indiceAleatorio = rand() % totalContenedores;
        strcpy(camion.contenedor, *(loteria+indiceAleatorio));

        strcpy(*(loteria+indiceAleatorio), *(loteria+totalContenedores - 1));
        totalContenedores--;

        ponerEnOrden(&listaCamiones, &camion, sizeof(tCamionGen), cmpCamionesGen, NULL);
    }

    fprintf(arch, "JORNADA:%d\n", config->duracionJornadaMinutos);
    fprintf(arch, "MUELLES:%d\n", config->cantidadMuelles);
    fprintf(arch, "ZONAS:%d\n", config->cantidadZonasAlmacenamiento);
    fprintf(arch, "CAPACIDAD_PILA:%d\n\n", config->capacidadPila);

    fprintf(arch, "[BUQUES]\n");

    while (sacarPrimeroLista(&listaBuques, &buque, sizeof(tBuqueGen)) == EXITO)
    {
        fprintf(arch, "%s;T=%d;C=%s\n", buque.id, buque.tArribo, buque.manifiesto);
    }

    fprintf(arch, "\n[CAMIONES]\n");
    while (sacarPrimeroLista(&listaCamiones, &camion, sizeof(tCamionGen)) == EXITO)
    {
        fprintf(arch, "%s;T=%d;C=%s\n", camion.id, camion.tArribo, camion.contenedor);
    }

    free(loteria);
    fclose(arch);

    return EXITO;
}
