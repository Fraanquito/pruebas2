#include <string.h>

#include "menu.h"

int menu()
{
    tArbol indiceOperadores;
    tOperador operadorActual;
    long offsetActual;

    tConfiguracion config;
    tPuerto puerto;
    tJornada jornada;

    char nombreIngresado[50];
    int opcion = -1;
    int loginOk = 0;
    int resLogin;
    int camionesPendientes = 0;


    crearArbol(&indiceOperadores);
    levantarIndice(ARCHIVO_INDICE, &indiceOperadores);


    while (loginOk == 0)
    {
        printf("=========================================\n");
        printf("  PUERTO DE CONTENEDORES: CONTRARRELOJ  \n");
        printf("=========================================\n");
        printf("Ingrese su nombre de operador: ");

        if (fgets(nombreIngresado, sizeof(nombreIngresado), stdin) != NULL)
        {
            limpiarSaltoLinea(nombreIngresado);
            if (*nombreIngresado != '\0')
            {
                resLogin = loginOperador(nombreIngresado, &operadorActual, &offsetActual, &indiceOperadores);
                if (resLogin == EXITO)
                {
                    loginOk = 1;
                }
            }
        }
    }
    while (opcion != 0)
    {
        printf("\n=========================================\n");
        printf("             MENU PRINCIPAL              \n");
        printf("  Operador activo: %s\n", operadorActual.nombre);
        printf("=========================================\n");
        printf("1. Iniciar nueva jornada\n");
        printf("2. Ver ranking de operadores\n");
        printf("3. Ver historial de jornadas\n");
        printf("4. Cambiar de operador\n");
        printf("0. Salir\n");
        printf("=========================================\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1)
        {
            opcion = -1;
        }
        while (getchar() != '\n');

        switch (opcion)
        {
        case 1:
            if (cargarConfiguracion("config.txt", &config) != EXITO)
            {
                printf("Error: No se pudo cargar config.txt\n");
                break;
            }
            if (generarEscenario("puerto.txt", &config) != EXITO)
            {
                printf("Error: No se pudo generar puerto.txt\n");
                break;
            }
            if (inicializarPuerto(&puerto, "puerto.txt", &config) == EXITO)
            {
                int motivo = ejecutarSimulacion(&puerto);
                mostrarResumenFinal(&puerto);

                camionesPendientes = contarCamionesPendientes(&puerto);
                strcpy(jornada.nombreOperador, operadorActual.nombre);
                jornada.buquesDescargados = puerto.buquesDescargados;
                jornada.contenedoresEntregados = puerto.contenedoresEntregados;
                jornada.reubicacionesRealizadas = puerto.reubicacionesRealizadas;
                jornada.camionesPendientes = camionesPendientes;
                jornada.tiempoUtilizado = puerto.relojActual;
                jornada.puntajeFinal = puerto.buquesDescargados * PUNTAJE_BUQUE +
                                       puerto.contenedoresEntregados * PUNTAJE_ENTREGA -
                                       camionesPendientes * PENALIZACION_CAMION;

                if (motivo == SIM_FIN_NORMAL) strcpy(jornada.motivoFinalizacion, "Normal");
                else if (motivo == SIM_FIN_BLOQUEO) strcpy(jornada.motivoFinalizacion, "Bloqueo");
                else if (motivo == SIM_FIN_TIEMPO) strcpy(jornada.motivoFinalizacion, "Tiempo");
                else strcpy(jornada.motivoFinalizacion, "Forzado");

                guardarJornadaBD(&jornada);
                operadorActual.jornadasJugadas++;
                operadorActual.puntajeTotal += jornada.puntajeFinal;
                actualizarOperador(&operadorActual, offsetActual);

                liberarPuerto(&puerto);
            }
            break;


            break;

        case 2:
            mostrarRankingOperadores();
            break;

        case 3:
            mostrarHistorialJornadas();
            break;

        case 4:
            loginOk = 0;
            while (loginOk == 0)
            {
                printf("\nIngrese el nuevo nombre de operador: ");
                if (fgets(nombreIngresado, sizeof(nombreIngresado), stdin) != NULL)
                {
                    limpiarSaltoLinea(nombreIngresado);
                    if (*nombreIngresado != '\0')
                    {
                        resLogin = loginOperador(nombreIngresado, &operadorActual, &offsetActual, &indiceOperadores);
                        if (resLogin == EXITO)
                        {
                            loginOk = 1;
                        }
                    }
                }
            }
            break;

        case 0:
            printf("\nGuardando indice de operadores y saliendo del sistema...\n");
            guardarIndiceOrdenado(ARCHIVO_INDICE, &indiceOperadores);
            eliminarArbol(&indiceOperadores);
            break;

        default:
            printf("\nOpcion invalida. Por favor, seleccione un numero del menu.\n");
            break;
        }
    }

    return EXITO;
}
