/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 11 clase 080824             *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    int var = 480;
    int *varptr = &var;
    int **doubleptr = &varptr;

    printf("Valor de la Variable \t\t= %d\n", var);
    printf("Valor del puntero \t\t= %d\n", *varptr);
    printf("Valor del puntero doble \t= %d\n", **doubleptr);

    printf("Dirección de la Variable \t= %p\n", &var);
    printf("Dirección del puntero \t\t= %p\n", &varptr);
    printf("Dirección del puntero doble \t= %p\n", &doubleptr);

    printf("Valor en el puntero \t\t= %p\n", varptr);
    printf("Valor en el puntero doble \t= %p\n", doubleptr);

    return 0;
}
