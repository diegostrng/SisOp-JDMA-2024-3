/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 15-08-24                          *
* Tema: Gestion de Memoria                 *
*                                          *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>

int mA[9];
int mB[9];
int mC[9];

void imprimir(int size, int matriz[]){
    for(int i = 0; i<size; i++){
    if(i%3==0){
    printf("\n");
    }
    printf("%d ", matriz[i]);
  
  }
  
  
}

void insertar(int size, int dato, int matriz[]){
  for(int i = 0; i<size; i++){
    matriz[i] = dato;
   
  }
  
}

int main(){

  insertar(9, 1, mA);
  insertar(9, 2, mB);
  insertar(9, 0, mC);
  
  imprimir(9, mA);
  imprimir(9, mB);
  imprimir(9, mC);
  
  
  printf("\nEso es todo amigos.\n");
}