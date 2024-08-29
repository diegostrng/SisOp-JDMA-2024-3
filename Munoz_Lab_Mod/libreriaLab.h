/************************************************************************
*                                                                       *
* Autor: JD                                                             *
* Materia: Sistemas Operativos                                          *
* Fecha: 29-08-24                                                       *
* Tema: Lab de compliacion modular:                                     *
*   -Primera parte: Funcionamiento del código: Completado a las 10:46   *
*   -Segunda parte: Modularización del código                           *
*   -Última parte: Creación de fichero de automatización de Compilación *
*                                                                       *
************************************************************************/
/************************************************************************
Librería de Funciones: Implementación de funciones. .h.
************************************************************************/

#ifndef _LIBRERIALAB_H_
#define _LIBRERIALAB_H_

#define CAPACIDAD_INICIAL 4

/*Creación Estructura de Datos para el Vector*/
typedef struct vectorDinamico{
        int capacidad;
        int totalElementos;
        void **elementos;
} vectorDinamico;


/*Inicializacion de los valores del Vector, casting y reserva de memoria*/
void vectorInicio(vectorDinamico *V);

/*Retorna el total de elementos*/
int totalVector(vectorDinamico *V);

/*Redimensiona el Vector*/
static void resizeVector(vectorDinamico *V, int capacidad);

/*Agregar elementos al Vector*/
void addVector(vectorDinamico *V, void *elemento);

/*Liberar la memoria dinámica del Vector*/
void freeVector(vectorDinamico *V);

/*Obtención de elemento según índice*/
void *getVector(vectorDinamico *V, int indice);

/*UBicar un elemento en un índice i*/
void setVector(vectorDinamico *V, int indice, void *elemento);

/*Borrar elemento según índice, redimensión también*/
void borrarVector(vectorDinamico *V, int indice);

#endif