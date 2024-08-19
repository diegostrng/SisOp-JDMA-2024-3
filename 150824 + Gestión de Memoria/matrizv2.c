/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 18-08-24                          *
* Tema: Gestion de Memoria                 *
*  Entrega Tarea Gestion de Memoria        *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void imprimir(int size, int *matriz){
    int n = (int)sqrt(size);
    for(int i = 0; i < size; i++){
        if(i % n == 0){
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

int* multiplicar(int size, int *mA, int *mB){
  
    int n = (int)sqrt(size);
    int *mC = (int*)malloc(size * sizeof(int));
  
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            mC[i*n + j] = 0;
            for(int k = 0; k < n; k++){
                mC[i*n + j] += mA[i*n + k] * mB[k*n + j];
            }
        }
    }
    return mC;
  
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
  
    insertar(size, 1, mA);
    insertar(size, 2, mB);
  
    printf("Matriz A:");
    imprimir(size, mA);
    printf("Matriz B:");
    imprimir(size, mB);
  
    int *mC = multiplicar(size, mA, mB);
  
    printf("Matriz C Resultado:");
    imprimir(size, mC);
  
    free(mC);
  
    printf("\nEso es todo amigos.\n");
    return 0;
}
