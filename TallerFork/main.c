#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


/******************************************
*                                          *
* Autor: Juan Diego Munoz Angulo           *
* Materia: Sistemas Operativos             *
* Fecha: 17-10-24                          *
* Tema: Taller Fork                        *
*                                          *
*******************************************/


// Modo de Compilacion.
/* 

gcc main.c -o main
./main 5 archivo1.txt 5 archivo2.txt
 
*/
// Un Fork() crea un proceso hijo que es una copia exacta del proceso padre.
// Un Pipe es un mecanismo de comunicacion entre procesos que crea un canal de comunicacion entre dos procesos.
// Un Wait() hace que el proceso padre espere a que el proceso hijo termine.
// Un Exit() termina el proceso actual en ejecucion.

//Funcion que lee los arreglos de los archivos
//Recibe el nombre del archivo y la cantidad de elementos que tiene
// Regresa un apuntador a un arreglo de enteros


int* lecturaArreglos(char* nombreArchivo, int cont) {
    
    FILE* archivo = fopen(nombreArchivo, "r"); // r para lectura p
    //Abrimos archivo

    int* arreglo = (int*)malloc(cont * sizeof(int));

     //Leer los enteros del archivo y guardarlos
    for (int i = 0; i < cont; i++)
    {
        if (fscanf(archivo, "%d", &arreglo[i]) != 1)
        {

            free(arreglo); // libero
            fclose(archivo); // cierro
            return NULL;
        }
    }

    fclose(archivo);
    return arreglo;
}

// Funcion que suma los elementos de un arreglo
//Recibe un apuntador a un arreglo y la cantidad de elementos que tiene
// Retorna la suma de los elementos del arreglo
int sumaEnteros(int* arreglo, int cant){ //Funcion que suma los elementos de un arreglo
    int cont = 0; // Iniciamos un contador que nos va a dar la suma al final
    for(int i = 0; i < cant; i++){
        cont += arreglo[i]; // Recorremos el arreglo y sumamos cada elemento que hay en el
    }
    return cont; // Retornamos lo que nos dio el contador

}

int main(int argc, char *argv[])
{
    if(argc != 5){ // Si los argumentos estan mal, mandamos un mensaje de error
        printf("Error de argumentos.\n");
        return 0;
    }


    int N1 = atoi(argv[1]);
    char* archivo00 = argv[2];
    int N2 = atoi(argv[3]);
    char* archivo01 = argv[4]; // Guardamos los argumentos en variables

    int* arreglo1 = lecturaArreglos(archivo00, N1);
    int* arreglo2 = lecturaArreglos(archivo01, N2); // Leemos los arreglos de los archivos, y los creamos

    int pipe1[2], pipe2[2], pipe3[2]; // Creamos los 3 pipes

    pipe(pipe1);
    pipe(pipe2);
    pipe(pipe3); // Los inicializamos



    pid_t pid1 = fork(); //Creamos el primer proceso hijo y le sacamos el pid

    if(pid1 == 0)
    {

        int con1 = sumaEnteros(arreglo1, N1); // Sumamos los elementos del arreglo 1
        close(pipe1[0]); // Cerramos el pipe 1
        write(pipe1[1], &con1, sizeof(con1)); // Escribimos en el pipe 1

        printf("GranHijo: [ %d ] Sum_File1 = %d\n", getpid(), con1); // Impresion sin mas
        close(pipe1[1]); // Cerramos el pipe 1

        exit(EXIT_SUCCESS); // todo bien
    }

    pid_t pid2 = fork(); // Creamos el segundo proceso hijo y le sacamos el pid

    if(pid2 == 0)
    {
        // 2do hijo
        int cont2 = sumaEnteros(arreglo2, N2); // El mismo proceso que con el de arriba
        close(pipe2[0]);
        write(pipe2[1], &cont2, sizeof(cont2));

        printf("SegundoHijo: [ %d ] Sum_File2 = %d\n", getpid(), cont2);
        close(pipe2[1]);

        exit(EXIT_SUCCESS);
    
    }

    pid_t pid3 = fork();

    if(pid3 == 0)
    {

        int cont3, cont4;
        close(pipe1[1]);
        read(pipe1[0], &cont3, sizeof(cont3));
        close(pipe2[1]);
        read(pipe2[0], &cont4, sizeof(cont4));
        int res = cont3 + cont4;
        close(pipe3[0]);
        write(pipe3[1], &res, sizeof(res));

        //Esto es lo mismo que con los dos primeros hijos pero solo q aqui se suman los resultados de los dos hijos anteriores

        printf("PrimerHijo: [ %d ] Suma Total = %d\n", getpid(), res);
        close(pipe3[1]);

        exit(EXIT_SUCCESS);
    }



    // El padre recibe los 3 y los imprime
    int final;
    close(pipe3[1]);
    read(pipe3[0], &final, sizeof(final));

    printf("Padre: [ %d ] Suma total = %d\n", getpid(), final);
    close(pipe3[0]);

    free(arreglo1);
    free(arreglo2);
    
}
