#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>

/*

    En este programa se implementa un suscriptor que se conecta al sistema de noticias y se suscribe a un tema de noticias.
    Puede recibir noticias de los temas de Arte, Política, Espectáculos, Ciencia y Sucesos, que vienen de un publicador, pasando por sistema.

*/


#define MAX_BUFFER 256


typedef struct
{
    char *pipeSSC;
} Config;


char suscripciones[1024] = "";

// Función para borrar FIFOs
// Recibe: nombre del FIFO
// Retorna: nada
void borrarFifos(const char *fifo_name) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error al obtener el directorio actual");
        return;
    }

    DIR *dir;
    struct dirent *entry;
    
    if ((dir = opendir(cwd)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name, fifo_name) != NULL) {
                char path[PATH_MAX];
                if (snprintf(path, sizeof(path), "%s/%s", cwd, entry->d_name) >= sizeof(path)) {
                    fprintf(stderr, "La ruta del archivo es demasiado larga.\n");
                    continue;
                }
                unlink(path);
            }
        }
        closedir(dir);
    } else {
        perror("Error al abrir el directorio");
    }
}

// Función para procesar los argumentos de la línea de comandos
// Recibe: argumentos de la línea de comandos, configuración
// Retorna: nada
void argumentos(int argc, char *argv[], Config *configuracion) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s -s pipeSSC\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            configuracion->pipeSSC = argv[i + 1];
            return;
        }
    }

    fprintf(stderr, "Uso: %s -s pipeSSC\n", argv[0]);
    exit(EXIT_FAILURE);
}

// Función para mostrar el menú de suscripción y obtener la opción del usuario
// Recibe: tema
// Retorna: nada
void menu(char *tema) {
    printf("\n");
    printf("Suscríbete a tu tipo de noticias preferido \n");
    printf("(A = Arte  P = Politica E = Espectaculo  C = Ciencia  S = Sucesos)\n");
    printf("O, escriba 'salir' para esperar las noticias a las que ya se suscribió. \n");
    printf("\n");
    fgets(tema, sizeof(tema), stdin);
    printf("\n");
    tema[strcspn(tema, "\n")] = '\0';
}

// Función para procesar la suscripción del usuario
// Recibe: file descriptor del pipe, nombre del FIFO y tema
// Retorna: nada
void enviarSuscripcion(int fdSSC, const char *fifo, const char *tema) {
    char suscripcion[1024];
    snprintf(suscripcion, sizeof(suscripcion), "%s %s", tema, fifo);
    write(fdSSC, suscripcion, strlen(suscripcion) + 1);
    printf("Suscripción enviada exitosamente al sistema.\n");

    if (strcmp(tema, "A") == 0) {
        strcat(suscripciones, "Arte - ");
    } else if (strcmp(tema, "P") == 0) {
        strcat(suscripciones, "Politica - ");
    } else if (strcmp(tema, "E") == 0) {
        strcat(suscripciones, "Espectaculo - ");
    } else if (strcmp(tema, "C") == 0) {
        strcat(suscripciones, "Ciencia - ");
    } else if (strcmp(tema, "S") == 0) {
        strcat(suscripciones, "Sucesos - ");
    }
}

// Función para leer y mostrar las noticias
// Recibe: file descriptor del FIFO y file descriptor del pipe
// Retorna: nada
void leer(int fd, int fdSSC) {
    char noticias[MAX_BUFFER];
    while (1) {
        int bytesRead = read(fd, noticias, sizeof(noticias));
        if (bytesRead > 0) {
            noticias[bytesRead] = '\0';
            if (strcmp(noticias, "salida") == 0) {
                printf("Esta emisión de noticias ha terminado. Gracias por sintonizarnos.\n");
                printf("\n");
                write(fdSSC, "salida", 7);
                break;
            }
            printf("Nueva noticia de ");
            if (noticias[0] == 'A') {
                printf("Arte:\n");
            } else if (noticias[0] == 'P') {
                printf("Politica:\n");
            } else if (noticias[0] == 'E') {
                printf("Espectaculo:\n");
            } else if (noticias[0] == 'C') {
                printf("Ciencia:\n");
            } else if (noticias[0] == 'S') {
                printf("Sucesos:\n");
            }
            printf("%s\n", noticias + 2);
            printf("\n");
        }
    }
}

// Función principal de suscripción
// Recibe: file descriptor del pipe y nombre del FIFO
// Retorna: nada
void suscribirse(int fdSSC, const char *fifo) {
    int fd;
    bool tieneSubs = false;
    bool abierto = false;

    while (1) {
        char tema[6];
        menu(tema);

        // Salimos del programa si el usuario escribe "salir"
        if (strcmp(tema, "salir") == 0 || strcmp(tema, "Salir") == 0 || strcmp(tema, "SALIR") == 0) {
            if (!tieneSubs) {
                printf("No tiene suscripciones. Saliendo del programa.\n");
                exit(EXIT_SUCCESS);
            }
            printf("\033[H\033[J"); // Estas son unas secuencias de escape q vi por internet, para limpiar la pantalla y que se vea mejor :)

            // Eliminamos el último " - " de la cadena de suscripciones
            if (strlen(suscripciones) > 3) {
                suscripciones[strlen(suscripciones) - 3] = '\0';
            }
            printf("\n");
            printf("Suscripciones realizadas a:\n%s\n", suscripciones);
            printf("Esperando las noticias. \n");
            printf("\n");
            break;
        }

        enviarSuscripcion(fdSSC, fifo, tema);
        tieneSubs = true;

        if (!abierto) {
            fd = open(fifo, O_RDONLY);
            abierto = true;
        }
    }

    leer(fd, fdSSC);
    close(fd);
}

int main(int argc, char *argv[]) {
    Config configuracion;
    argumentos(argc, argv, &configuracion);

    // Obtenemos el directorio actual
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error al obtener el directorio actual");
        exit(EXIT_FAILURE);
    }

    // Revisamos la ruta del FIFO
    char fifo[PATH_MAX];
    if (snprintf(fifo, sizeof(fifo), "%s/FIFO_%d", cwd, getpid()) >= sizeof(fifo)) {
        fprintf(stderr, "La ruta del FIFO es demasiado larga.\n");
        exit(EXIT_FAILURE);
    }

    // Creamos el FIFO
    if (mkfifo(fifo, 0666) == -1) {
        perror("Error al crear el FIFO.");
        exit(EXIT_FAILURE);
    }

    // Abrimos el pipe de suscripciones
    int fdSSC = open(configuracion.pipeSSC, O_WRONLY);
    if (fdSSC == -1) {
        perror("Error al abrir pipeSSC.");
        unlink(fifo);
        exit(EXIT_FAILURE);
    }

    printf("Conexión establecida.\n");
    printf("Bienvenido al Sistema Nacional de Noticias!\n");

    suscribirse(fdSSC, fifo);

    // Cerramos el pipe y eliminamos el FIFO
    close(fdSSC);
    unlink(fifo);
    borrarFifos("FIFO_");

    return 0;
}