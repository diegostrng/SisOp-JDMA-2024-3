/******************************************
*                                          *
* Autor: Juan Diego Muñoz Angulo           *
* Materia: Sistemas Operativos             *
* Fecha: 03-10-24                          *
* Tema: Taller Piped Names                 *
* CLIENTE                                  *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "MYFIFO"

int main() {
    int fd;
    int end_process;
    int stringlen;
    char readbuf[80];
    char end_str[5];

    printf("FIFO_CLIENTE: Enviar mensajes infinitamente, para terminar ingrese \"end\"\n");

    /* Abre el FIFO para escritura */
    fd = open(FIFO_FILE, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    strcpy(end_str, "end");

    while (1) {
        printf("Ingrese cadena: ");
        fgets(readbuf, sizeof(readbuf), stdin);
        stringlen = strlen(readbuf);
        readbuf[stringlen - 1] = '\0';  // Termina la cadena con null
        end_process = strcmp(readbuf, end_str);

        if (write(fd, readbuf, strlen(readbuf)) == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (end_process == 0) {
            break;
        }
    }

    /* Cierra el FIFO */
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}