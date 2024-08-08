/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 10 clase 010824             *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    int rows, cols, i, j;
    int **matrix;

    rows = (int) atof(argv[1]);
    cols = (int) atof(argv[2]);

    //Asignación de Memoria para la Matriz
    matrix = (int **)malloc(rows*sizeof(int *));
    for(i = 0; i < rows; i++){
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    //Llenado de matriz con valores iniciales
    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            matrix[i][j] = i * j;
        }
    }

    // Impresión de Matriz
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Liberación/entrega de memoria al SO
    for (i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
