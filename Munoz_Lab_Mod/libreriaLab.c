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
Librería de Funciones: Implementación de funciones
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "libreriaLab.h"

/*Inicializacion de los valores del Vector, casting y reserva de memoria*/
// Establece los valores predeterminados de un vectorDinamico. Establece su capacidad al predefinido en 4, el totalElementos a 0
// y libera la memoria para los elementos segun la capacidad.
// Debe recibir el vectorDinamico nuevo, y no retorna nada.
void vectorInicio(vectorDinamico *V){
        V->capacidad = CAPACIDAD_INICIAL;
        V->totalElementos = 0;
        V->elementos = malloc(sizeof(void *) * V->capacidad); 
}

/*Retorna el total de elementos*/
// Nos permite ver el total de elementos que tiene un vectorDinamico
// Recibe el vectorDinamico y Retorna un entero, el total de Elementos del vector.
int totalVector(vectorDinamico *V){
        return V->totalElementos;
}

/*Redimensiona el Vector*/
//Esta función se encarga de cambiar el tamaño del vectorDinamico, para cuando queremos añadir un nuevo elemento o borrar los espacios que dejaron unos que eleiminamos.
// Recibe el vectorDinamico a cambiar, y la capacidad que le queremos poner. No retorna nada.
static void resizeVector(vectorDinamico *V, int capacidad){
        printf("Redimensión: %d a %d \n", V->capacidad, capacidad);

        void **elementos = realloc(V->elementos, sizeof(void *) * capacidad);
        if(elementos){
                V->elementos = elementos;
                V->capacidad = capacidad;
        }
}

/*Agregar elementos al Vector*/
// Añade un nuevo elemento en la cola del vectorDinamico. Si no tiene suficiente espacio, lo aumenta con resizeVector.
// Recibe el vectorDinamico, y el elemento a añadir. Lo hace y no retorna nada.
void addVector(vectorDinamico *V, void *elemento){
        if(V->capacidad == V->totalElementos)
                resizeVector(V, V->capacidad*2);
        V->elementos[V->totalElementos++] = elemento;
}

/*Liberar la memoria dinámica del Vector*/
//Libera la memoria del vectorDinamico que se tiene creado, elemento por elemento ya que es una estructura.
//Solo recibe el vector a liberar, no tiene ningun retorno.
void freeVector(vectorDinamico *V){
        free(V->elementos);
}

/*Obtención de elemento según índice*/
// Nos da el valor indicado en un indice especifico en el vectorDinamico.
// Recibe un vectorDinamico, y el indice del valor que necesitamos.
// Retorna el elemento en el indice especifico, de existir: (Que el indice sea alcanzable). Si no, retorna Nulo.
void *getVector(vectorDinamico *V, int indice){
        if(indice >= 0 && indice < V->totalElementos)
                return V->elementos[indice];
        return NULL;
}

/*UBicar un elemento en un índice i*/
// Establece un valor para un indice especifico de un vectorDinamico.
// Recibe el vector, el indice donde se va a establecer el valor, y la variable a guardar allí. No retorna nada.
void setVector(vectorDinamico *V, int indice, void *elemento){
        if(indice >= 0 && indice < V->totalElementos)
                V->elementos[indice]=elemento;
}

/*Borrar elemento según índice, redimensión también*/
// Borra el elemento de un vectorDinamico, en un indice especifico. Si queda memoria libre sin usar, reduce la capacidad del vectorDinamico con el resizeVector.
// Recibe un vectorDinamico, el indice del elemento a borrar. Hace todo dentro y no retorna nada.
void borrarVector(vectorDinamico *V, int indice){
        if(indice < 0 || indice >= V->totalElementos)
                return; 

        V->elementos[indice] = NULL;

        for(int i=indice; i<V->totalElementos-1; i++){
                V->elementos[i] = V->elementos[i+1];
                V->elementos[i+1] = NULL; 
        }
        V->totalElementos--;
        if(V->totalElementos>0 && V->totalElementos == V->capacidad/4)
                resizeVector(V, V->capacidad/2);
}

