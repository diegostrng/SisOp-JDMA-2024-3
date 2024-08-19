/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 13 clase 080824             *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int *vP1 = calloc(4, sizeof(int));
    int *vP2 = calloc(4, sizeof(int));   
    int *vP3 = calloc(4, sizeof(*vP3)); 

    if (vP2) {
        for (int i = 0; i < 4; i++) {
            vP2[i] = i + 1;
            printf("vP2[%d]\t= %d\n", i, vP2[i]);
        }
    }

    printf("Libero memoria reservada\n");
    free(vP1);
    free(vP2);
    free(vP3);

    return 0;
}
