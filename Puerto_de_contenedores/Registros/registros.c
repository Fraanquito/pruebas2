#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "registros.h"


int cmpIndiceOperador(const void *a, const void *b)
{
    const tRegIndice *ia = (const tRegIndice *)a;
    const tRegIndice *ib = (const tRegIndice *)b;

    return strcmp(ia->nombre, ib->nombre);
}

int cmpOperadorRanking(const void *a, const void *b)
{
    const tOperador *oa = (const tOperador *)a;
    const tOperador *ob = (const tOperador *)b;

    return ob->puntajeTotal - oa->puntajeTotal;
}

void accionGuardarIndice(void *info, unsigned tamInfo, unsigned nivel, void *param)
{
    FILE *fp = (FILE*)param;
    fwrite(info, sizeof(tRegIndice), 1, fp);
}

int guardarIndiceOrdenado(const char *nomArchIndice, const tArbol *arbol)
{
    FILE *fp = fopen(nomArchIndice, "wb");
    if (!fp)
    {
        return ERR_ARCH;
    }

    recorrerIn(arbol, accionGuardarIndice, fp);

    fclose(fp);
    return EXITO;
}

int _cargarIndiceBalanceado(tArbol *arbol, FILE *fp, int li, int ls)
{
    int m;
    tRegIndice indAux;

    if (li > ls)
    {
        return EXITO;
    }

    m = (li + ls) / 2;

    fseek(fp, m * sizeof(tRegIndice), SEEK_SET);
    fread(&indAux, sizeof(tRegIndice), 1, fp);

    insArb(arbol, &indAux, sizeof(tRegIndice), cmpIndiceOperador, NULL);

    _cargarIndiceBalanceado(arbol, fp, li, m - 1);
    _cargarIndiceBalanceado(arbol, fp, m + 1, ls);

    return EXITO;
}

int levantarIndice(const char *nomArchIndice, tArbol *arbol)
{
    FILE *fp;
    int cantRegistros;

    crearArbol(arbol);

    fp = fopen(nomArchIndice, "rb");
    if (!fp)
    {
        fp = fopen(nomArchIndice, "w+b");
        if (!fp)
        {
            return ERR_ARCH;
        }
    }

    fseek(fp, 0, SEEK_END);
    cantRegistros = ftell(fp) / sizeof(tRegIndice);

    if (cantRegistros > 0)
    {
        _cargarIndiceBalanceado(arbol, fp, 0, cantRegistros - 1);
    }

    fclose(fp);
    return EXITO;
}

int loginOperador(const char *nombreBuscado, tOperador *operadorActual, long *offsetActual, tArbol *indice)
{
    tRegIndice regEncontrado;
    FILE *fpOperadores;
    char confirmacion;

    strcpy(regEncontrado.nombre, nombreBuscado);

    if (busqElemArbol(indice, &regEncontrado, sizeof(tRegIndice), cmpIndiceOperador) == EXITO)
    {
        printf("\n===================================\n");
        printf("Se encontro un registro para el operador: %s\n", regEncontrado.nombre);
        printf("Sos vos este operador? (S/N): ");
        do
        {
            scanf(" %c", &confirmacion);
            while (getchar() != '\n');
            confirmacion = toupper(confirmacion);

            if (confirmacion != 'S' && confirmacion != 'N')
            {
                printf("Opcion invalida. Por favor ingresa S o N: ");
            }
        }
        while (confirmacion != 'S' && confirmacion != 'N');

        if (confirmacion == 'S')
        {
            fpOperadores = fopen(ARCHIVO_OPERADORES, "rb");
            if (!fpOperadores)
            {
                return ERR_ARCH;
            }

            fseek(fpOperadores, regEncontrado.offset, SEEK_SET);
            fread(operadorActual, sizeof(tOperador), 1, fpOperadores);
            fclose(fpOperadores);

            *offsetActual = regEncontrado.offset;

            printf("\n===================================\n");
            printf("Bienvenido nuevamente, Operador %s!\n", operadorActual->nombre);
            printf("Jornadas jugadas: %d | Puntaje acumulado total: %d\n",
                   operadorActual->jornadasJugadas, operadorActual->puntajeTotal);
            printf("===================================\n");

            return EXITO;
        }
        else
        {
            return OPERADOR_EQUIVOCADO;
        }
    }
    else
    {
        strcpy(operadorActual->nombre, nombreBuscado);
        operadorActual->puntajeTotal = 0;
        operadorActual->jornadasJugadas = 0;

        fpOperadores = fopen(ARCHIVO_OPERADORES, "r+b");
        if (!fpOperadores)
        {
            fpOperadores = fopen(ARCHIVO_OPERADORES, "w+b");
        }

        if (!fpOperadores)
        {
            return ERR_ARCH;
        }

        fseek(fpOperadores, 0, SEEK_END);
        *offsetActual = ftell(fpOperadores);

        if (fwrite(operadorActual, sizeof(tOperador), 1, fpOperadores) != 1)
        {
            fclose(fpOperadores);
            return ERR_ARCH;
        }
        fclose(fpOperadores);

        regEncontrado.offset = *offsetActual;
        strcpy(regEncontrado.nombre, nombreBuscado);

        insArb(indice, &regEncontrado, sizeof(tRegIndice), cmpIndiceOperador, NULL);

        guardarIndiceOrdenado(ARCHIVO_INDICE, indice);

        printf("\n===================================\n");
        printf("Operador nuevo registrado.\nBienvenido al Puerto, Operador %s!\n", operadorActual->nombre);
        printf("===================================\n");
        return EXITO;
    }
}

int actualizarOperador(tOperador *operadorModificado, long offsetOperador)
{
    FILE *fp = fopen(ARCHIVO_OPERADORES, "r+b");
    if (!fp)
    {
        return ERR_ARCH;
    }

    fseek(fp, offsetOperador, SEEK_SET);
    fwrite(operadorModificado, sizeof(tOperador), 1, fp);

    fclose(fp);
    return EXITO;
}

int guardarJornadaBD(tJornada *nuevaJornada)
{
    FILE *fp = fopen(ARCHIVO_JORNADAS, "ab");
    if (!fp)
    {
        return ERR_ARCH;
    }

    fwrite(nuevaJornada, sizeof(tJornada), 1, fp);

    fclose(fp);
    return EXITO;
}

void mostrarRankingOperadores()
{
    FILE *fp = fopen(ARCHIVO_OPERADORES, "rb");
    tOperador *vecOp = NULL;
    tOperador *ptrVec;
    int cant = 0;
    int i;
    int puesto = 1;

    if (!fp)
    {
        printf("\nNo se pudo abrir el archivo de operadores o no hay registros previos.\n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    cant = ftell(fp) / sizeof(tOperador);
    fseek(fp, 0, SEEK_SET);

    if (cant > 0)
    {
        vecOp = (tOperador*)malloc(cant * sizeof(tOperador));
        if (vecOp)
        {
            ptrVec = vecOp;
            for (i = 0; i < cant; i++)
            {
                fread(ptrVec, sizeof(tOperador), 1, fp);
                ptrVec++;
            }

            qsort(vecOp, cant, sizeof(tOperador), cmpOperadorRanking);

            printf("\n=================== RANKING DE OPERADORES ===================\n");
            printf("%-6s | %-20s | %-16s | %-14s\n", "PUESTO", "OPERADOR", "JORNADAS JUGADAS", "PUNTOS TOTALES");
            printf("-------------------------------------------------------------\n");

            ptrVec = vecOp;
            for (i = 0; i < cant; i++)
            {
                printf("#%-5d | %-20s | %-16d | %-14d\n",
                       puesto,
                       ptrVec->nombre,
                       ptrVec->jornadasJugadas,
                       ptrVec->puntajeTotal);
                puesto++;
                ptrVec++;
            }
            printf("=============================================================\n");

            free(vecOp);
        }
    }
    else
    {
        printf("\nNo hay operadores registrados en el sistema.\n");
    }

    fclose(fp);
}

void mostrarHistorialJornadas()
{
    FILE *fp = fopen(ARCHIVO_JORNADAS, "rb");
    tJornada jornada;

    if (!fp)
    {
        printf("\nNo se pudo abrir el archivo de jornadas o no hay jornadas registradas.\n");
        return;
    }

    printf("\n====================== HISTORIAL DE JORNADAS ======================\n");
    printf("%-18s | %-6s | %-6s | %-6s | %-6s | %-12s\n", "OPERADOR", "PUNTOS", "BUQUES", "CONTEN", "TIEMPO", "MOTIVO FIN");
    printf("-------------------------------------------------------------------\n");

    while (fread(&jornada, sizeof(tJornada), 1, fp) == 1)
    {
        printf("%-18s | %-6d | %-6d | %-6d | %-6d | %-12s\n",
               jornada.nombreOperador,
               jornada.puntajeFinal,
               jornada.buquesDescargados,
               jornada.contenedoresEntregados,
               jornada.tiempoUtilizado,
               jornada.motivoFinalizacion);
    }

    printf("================================================-------------------\n");
    fclose(fp);
}
