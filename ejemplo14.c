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
#include <assert.h>

// Imprimir el tamaño reasignado
void Info_Almacenamiento(const int *siguiente, const int *anterior, int valores) {
    if (siguiente) {
        printf("%s ubicación = %p. Tamaño Reservado: %d valores (%ld bytes)\n",
               (siguiente != anterior ? "Nuevo" : "Anterior"), (void*)siguiente, valores, valores * sizeof(int));
    }
}

int main() {
    const int patron[] = {1, 2, 3, 4, 5, 6, 7, 8};
    const int patron_size = sizeof(patron) / sizeof(int);
    int *siguiente = NULL, *anterior = NULL;

    if ((siguiente = (int *)malloc(patron_size * sizeof(siguiente)))) {
        memcpy(siguiente, patron, sizeof patron);
        Info_Almacenamiento(siguiente, anterior, patron_size);
    } else {
        return EXIT_FAILURE;
    }

const int realloc_size[] = {18,20,30,40,50,60,70,80};
    for(int i=0; i < sizeof(realloc_size) / sizeof(int); i++){
        if ((siguiente = (int *)realloc(anterior, realloc_size[i]*sizeof(int)))) {
            Info_Almacenamiento(siguiente, anterior, realloc_size[i]);
            assert(memcmp(siguiente, patron, sizeof patron) == 0); // Verifica si los datos se copiaron correctamente
        } else {
            printf("Se libera memoria reservada\n");
            free(siguiente);
            return EXIT_FAILURE;
        }
    }

    free(anterior);
    return 0;
}

}
