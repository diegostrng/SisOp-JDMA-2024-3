/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 15-08-24                          *
* Tema: Gestion de Memoria                 *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void imprimir(int size, int *matriz){
  
    for(int i = 0; i < size; i++){
        if(i % ((int)sqrt(size)) == 0){
            printf("\n");
        }
        printf("%d ", matriz[i]);
    }
    printf("\n");
}

void insertar(int size, int dato, int *matriz){
    for(int i = 0; i < size; i++){
        matriz[i] = dato;
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Error de argumentos.\n");
        printf("Debe tener 1 argumento, el núm de espacios en la Matriz.\n");
        return 0;
    }
  
    int size = atoi(argv[1]);
    int mA[size];
    int mB[size];
    int mC[size];
  
    insertar(size, 1, mA);
    insertar(size, 2, mB);
    insertar(size, 0, mC);
  
    imprimir(size, mA);
    imprimir(size, mB);
    imprimir(size, mC);
  
    printf("\nEso es todo amigos.\n");
    return 0;
}