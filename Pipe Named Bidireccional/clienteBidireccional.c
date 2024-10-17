/******************************************
*                                          *
* Autor: Juan Diego Muñoz Angulo           *
* Materia: Sistemas Operativos             *
* Fecha: 17-10-24                          *
* Tema: Piped Named Bidireccional          *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
    int fd;
    int end_process;
    int stringlen;
    int read_bytes;
    char readbuf[80];
    char end_str[5];

    printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");

    fd = open(FIFO_FILE, O_CREAT | O_RDWR, 0640);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    strcpy(end_str, "end");

    while (1) {
        printf("Enter string: ");
        fgets(readbuf, sizeof(readbuf), stdin);
        stringlen = strlen(readbuf);
        readbuf[stringlen - 1] = '\0';
        end_process = strcmp(readbuf, end_str);

        if (end_process != 0) {
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("write");
                close(fd);
                exit(EXIT_FAILURE);
            }
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
            read_bytes = read(fd, readbuf, sizeof(readbuf));
            if (read_bytes == -1) {
                perror("read");
                close(fd);
                exit(EXIT_FAILURE);
            }
            readbuf[read_bytes] = '\0';
            printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
        } else {
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("write");
                close(fd);
                exit(EXIT_FAILURE);
            }
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
            close(fd);
            break;
        }
    }

    return 0;
}