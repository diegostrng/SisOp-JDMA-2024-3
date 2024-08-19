/******************************************
*                                          *
* Autor: JD                                *
* Materia: Sistemas Operativos             *
* Fecha: 01-08-24                          *
* Tema: Prueba 2 clase 010824              *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  
  char *p;
  char *q = NULL;
  
  printf("Address of p = %s\n", p);
  
  strcpy(p, "Hello Im the best Operating System!");
  printf("%s\n", p);
  printf("About to copy \"Goodbye\" to q\n");
  strcpy(q, "Goodbye");
  printf("String copied");
  printf("%s\n", q);
  
  return 0;
  
}