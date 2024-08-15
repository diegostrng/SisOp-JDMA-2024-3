/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 12 clase 080824             *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  
    int *varptr1 = malloc(sizeof(int));
    int *varptr2 = malloc(sizeof(int[4]));
    int *varptr3 = malloc(4*sizeof *varptr3);

    if(*varptr1){
      for(int i=0; i<4; i++)
        varptr1(i) = i*i;
      for(int i=0; i<4; i++)
        printf("varptr1[%d]\t == %d\n", i, varptr1[i]);    
    }

    printf("Se libera la memoria.\n");
    free(varptr1);
    free(varptr2);
    free(varptr3);

    return 0;
                          }}

  
  
