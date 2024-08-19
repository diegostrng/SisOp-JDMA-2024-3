/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 8 clase 010824              *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 10

int main(int argc, char *argv[]){
    int *vectorPunteros[size];  // Vector de punteros de tipo int
    int p = 10, q = 80 , r = 120;  // Variables tipo entero

    vectorPunteros[0] = &p;  // Apuntar a la dirección de p
    vectorPunteros[1] = &q;  // Apuntar a la dirección de q
    vectorPunteros[2] = &r;  // Apuntar a la dirección de r

    printf("\nForma de acceso al vector de punteros:\n");

    for (int i = 0; i < 3; i++) {
        printf("Para la dirección: %d \t el valor = %d\n", i, *vectorPunteros[i]);
    }

    return 0;
}

