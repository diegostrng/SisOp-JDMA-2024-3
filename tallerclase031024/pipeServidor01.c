/******************************************
*                                          *
* Autor: Juan Diego Muñoz Angulo           *
* Materia: Sistemas Operativos             *
* Fecha: 03-10-24                          *
* Tema: Taller Piped Names                 *
* SERVIDOR                                 *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_FILE "MYFIFO"

int main() {
    int ArchivoDesc;
    char BusMensaje[80];
    int cantidadBytes;

    printf("\n \t\t >>>>>>> SERVIDOR cs1.6 <<<<<<< \n");

    /* Elimina el FIFO si ya existe */
    if (unlink(FIFO_FILE) == -1 && errno != ENOENT) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    /* Crea el FIFO o tubería si no existe */
    if (mkfifo(FIFO_FILE, 0640) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    /* Abre el FIFO para lectura */
    ArchivoDesc = open(FIFO_FILE, O_RDONLY);
    if (ArchivoDesc == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* Lee del FIFO hasta recibir "end" */
    while (1) {
        cantidadBytes = read(ArchivoDesc, BusMensaje, sizeof(BusMensaje) - 1);
        if (cantidadBytes == -1) {
            perror("read");
            close(ArchivoDesc);
            exit(EXIT_FAILURE);
        }

        BusMensaje[cantidadBytes] = '\0';  // Termina la cadena con null
        printf("Recibido: %s\n", BusMensaje);

        if (strcmp(BusMensaje, "end") == 0) {
            break;
        }
    }

    /* Cierra el FIFO */
    if (close(ArchivoDesc) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    /* Elimina el FIFO */
    if (unlink(FIFO_FILE) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}