/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 9 clase 010824              *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 10

int main(int argc, char *argv[]){
  
    //Suma de dos valores p
    int valor1, valor2; //Variables de tipo entero
    int *ptrV1, *ptrV2; //Punteros de tipo entero

    ptrV1 = &valor1; //Apunta a la variable valor1
    ptrV2 = &valor2; //Apunta a la variable valor2

    printf("\nIngrese dos valores a ser sumados: ");
    scanf("%d %d", &valor1, &valor2);

    printf("\nLa suma es = %d\n", *ptrV1 + *ptrV2);

    return 0;
}
