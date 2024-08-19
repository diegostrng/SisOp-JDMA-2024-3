/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 5 clase 010824              *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *function(char *p) {
    char q[strlen(p) + 1];
    strcpy(q, p);
    printf("From q: the string is %s\n", q);
    return q;
}

int main() {
    char *a = NULL;
    char *b = NULL;
    a = function("Hi, It's fun to learn");
    b = function("systems engineer");
    printf("From main: %s %s\n", a, b);
}

