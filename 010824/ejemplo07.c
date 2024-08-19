/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 7 clase 010824              *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    int variable = 123;
    int *ptr = &variable;

    printf("Valor de la Variable %d \n", variable);
    printf("Dirección de memoria de la Variable %p\n", &variable);
    printf("Dirección a la que apunta el puntero %p \n", (void *)ptr);
    printf("Valor de la Variable %d \n", *ptr);

    return 0;
}
