#ifndef COMUNES_H_INCLUDED
#define COMUNES_H_INCLUDED

/// GENERALES
#define ELIM             2
#define EXITO            1

/// VACIOS
#define ARBOL_VACIO      0
#define PILA_VACIA      -1
#define COLA_VACIA      -2
#define LISTA_VACIA     -3

/// ERRORES
#define NO_ENCONTRADO   -4
#define SIN_MEM         -5
#define ERR_ARCH        -6
#define SIN_PARAMETRO   -12

/// ABB
#define NO_ES_HOJA      -7
#define DUPLICADO       -8
#define ERR_INSER_ABB   -9

/// MACROS
#define MINIMO(X,Y) ((X) > (Y) ? (Y) : (X))
#define MAXIMO(X,Y) ((X) > (Y) ? (X) : (Y))



#define PUNTAJE_ENTREGA 10
#define PUNTAJE_BUQUE 5
#define PENALIZACION_CAMION 2


#endif // COMUNES_H_INCLUDED
