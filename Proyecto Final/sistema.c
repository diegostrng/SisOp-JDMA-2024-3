/**********************
        Pontificia Universidad Javeriana
    Autor: Juan Diego Muñoz Angulo, David Roa Neisa,
            Jorge Humberto Sierra Laiton, Michelle Gabriela Cruz Pino,
            Sofia Carolina Mantilla Vega
    Fecha: 13 Noviembre 2024
    Materia: Sistemas Operativos
    Tema: Sistema de Noticias
          Proyecto Sistemas Operativos
          Sistema de Emisión de Noticias

**********************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>

/*
    En este programa se implementa un sistema de emisión de noticias que recibe noticias de los publicadores y las envía a los suscriptores.
    Los suscriptores se suscriben a los temas. El sistema de emisión de noticias recibe las noticias y las segmenta por tema.
    Luego, envía las noticias a los suscriptores que estén suscritos a ese tema.

*/

#define MAX_BUFFER 256
#define MAX_SUBSCRIPTORES 50
#define MAX_TEMAS 5

typedef struct {
    char temas[MAX_TEMAS];
    int contadorTemas;
    char pipeName[MAX_BUFFER];
    int fd;
} Suscriptor;

typedef struct {
    char *pipePSC;
    char *pipeSSC;
    int tiempoDado;
} Config;

Suscriptor suscriptores[MAX_SUBSCRIPTORES];
int numeroSuscriptores = 0;

// Abre el pipe de suscripciones
// Recibe: configuración y puntero a fdSSC
// Retorna: nada
void abrirPipeSubs(Config *configuracion, int *fdSSC) {
    *fdSSC = open(configuracion->pipeSSC, O_RDONLY);
    if (*fdSSC == -1) {
        perror("Error abriendo pipe de suscripciones");
        pthread_exit(NULL);
    }
}

// Lee una suscripción del pipe
// Recibe: fdSSC y buffer de suscripción
// Retorna: nada
void leerSuscripcion(int fdSSC, char *suscripcion)
{
    read(fdSSC, suscripcion, MAX_BUFFER);
    suscripcion[strcspn(suscripcion, "\n")] = '\0';
}

// Verifica si el tema existe
// Recibe: tema
// Retorna: booleano
bool existeTema(char tema) {
    return tema == 'A' || tema == 'E' || tema == 'C' || tema == 'P' || tema == 'S';
}

// Agrega un nuevo tema a un suscriptor
// Recibe: puntero a Suscriptor y tema
// Retorna: nada
void nuevoTema(Suscriptor *Suscriptor, char tema)
{
    if(Suscriptor->contadorTemas >= MAX_TEMAS) {
        return;
    }

    for(int j = 0; j < Suscriptor->contadorTemas; j++) {
        if (Suscriptor->temas[j] == tema) {
            printf("El suscriptor ya está suscrito al tema %c.\n", tema);
            printf("\n");
            return;
        }
    }

    Suscriptor->temas[Suscriptor->contadorTemas++] = tema;
    printf("Tema %c agregado para un suscriptor.\n", tema);
    printf("\n");
}

// Registra un nuevo suscriptor
// Recibe: nombre del pipe y tema
// Retorna: nada
void registroSuscriptor(char *pipeName, char tema)
{
    Suscriptor *nuevo_subscriptor = &suscriptores[numeroSuscriptores];

    // Copiar el nombre del pipe
    strncpy(nuevo_subscriptor->pipeName, pipeName, MAX_BUFFER - 1);
    nuevo_subscriptor->pipeName[MAX_BUFFER - 1] = '\0';

    // Inicializar temas y contador
    nuevo_subscriptor->temas[0] = tema;
    nuevo_subscriptor->contadorTemas = 1;

    // Abrir el pipe
    nuevo_subscriptor->fd = open(pipeName, O_WRONLY);
    if (nuevo_subscriptor->fd == -1) {
        perror("Error abriendo pipe del suscriptor");
        return;
    }

    // Determinar el nombre del tema
    char *temaNombre = "Desconocido";
    switch (tema) {
        case 'A': temaNombre = "Arte"; break;
        case 'P': temaNombre = "Politica"; break;
        case 'E': temaNombre = "Espectaculo"; break;
        case 'C': temaNombre = "Ciencia"; break;
        case 'S': temaNombre = "Sucesos"; break;
    }

    // Incrementamos el contador de suscriptores
    numeroSuscriptores++;

    // Imprimimos mensaje
    printf("Nuevo Suscriptor agregado al tema: %s\n", temaNombre);
    printf("\n");
}

// Ajusta la suscripción de un suscriptor
// Recibe: suscripción
// Retorna: nada
void ajustarSuscripcion(char *suscripcion) {
    char tema;
    char pipeName[MAX_BUFFER];
    sscanf(suscripcion, "%c %s", &tema, pipeName);

    if (!existeTema(tema)) {
        printf("El tema dado (%c) no existe.\n", tema);
        printf("\n");
        return;
    }

    bool bandera = false;
    for (int i = 0; i < numeroSuscriptores; i++) {
        if (strcmp(suscriptores[i].pipeName, pipeName) == 0)
        {
            nuevoTema(&suscriptores[i], tema);
            bandera = true;
            break;
        }
    }

    if (!bandera) {
        if (numeroSuscriptores < MAX_SUBSCRIPTORES) {
            registroSuscriptor(pipeName, tema);
        } else {
            printf("Suscripción rechazada. No hay espacio para más suscriptores. \n");
            printf("\n");
        }
    }
}

// Procesa las suscripciones de los suscriptores
// Recibe: argumento de configuración
// Retorna: nada
void *procesarSuscriptores(void *arg) {
    Config *config = (Config *)arg;
    int fdSSC;
    abrirPipeSubs(config, &fdSSC);

    char suscripcion[MAX_BUFFER];
    while(true)
    {
        leerSuscripcion(fdSSC, suscripcion);
        if (strcmp(suscripcion, "salida") == 0) {
            break;
        }
        ajustarSuscripcion(suscripcion);
    }

    close(fdSSC);
    return NULL;
}

// Abre el pipe de publicaciones
// Recibe: configuración y puntero a PPSCfd
// Retorna: nada
void abrirPipePublic(Config *configuracion, int *PPSCfd)
{
    *PPSCfd = open(configuracion->pipePSC, O_RDONLY);

    if (*PPSCfd == -1) {
        perror("Error abriendo pipePSC.");
        pthread_exit(NULL);
    }
}

// Procesa una publicación
// Recibe: PPSCfd, buffer y tiempoDado
// Retorna: bytes leídos
int procesarPublicacion(int PPSCfd, char *buffer, int tiempoDado) {
    int bytesRead = read(PPSCfd, buffer, MAX_BUFFER);
    if (bytesRead == 0) {
        for (int h = 0; h < tiempoDado; h++) {
            sleep(1);
            bytesRead = read(PPSCfd, buffer, MAX_BUFFER);
            if (bytesRead > 0) break;
        }
    }
    return bytesRead;
}

// Envía una noticia a los suscriptores
// Recibe: buffer de la noticia
// Retorna: nada
void enviarNoticia(char *buffer) {
    char tema = buffer[0];
    const char *temaNombre;

    switch (tema)
    {
        case 'A':
            temaNombre = "Arte";
            break;
        case 'P':
            temaNombre = "Politica";
            break;
        case 'E':
            temaNombre = "Espectaculo";
            break;
        case 'C':
            temaNombre = "Ciencia";
            break;
        case 'S':
            temaNombre = "Sucesos";
            break;
        default:
            temaNombre = "Desconocido";
            break;
    }

    //Imprimimos la noticia con el nombre del tema
    printf("Nueva noticia de %s: %s\n", temaNombre, buffer + 3);
    printf("\n");

    int enviadas = 0;
    for (int i = 0; i < numeroSuscriptores; i++) {
        for (int j = 0; j < suscriptores[i].contadorTemas; j++) {
            if (suscriptores[i].temas[j] == tema)
            {
                
                enviadas++;
                write(suscriptores[i].fd, buffer, strlen(buffer) + 1);
                printf("Enviando noticia de %s a un suscriptor. [%d]\n", temaNombre, enviadas);
                
            }
        }
    }

    if(enviadas == 0)
    {
        printf("\n");
    }
    else
    {
        printf("...\n");
        printf("\n");
    }
    // Esto solo es para que se vea bonita la impresion

}

// Procesa las noticias
// Recibe: argumento de configuración
// Retorna: nada
void *procesoNoticias(void *arg) {
    Config *configuracion = (Config *)arg;
    int PPSCfd;
    abrirPipePublic(configuracion, &PPSCfd);

    char buffer[MAX_BUFFER];
    while (true) {
        int bytesRead = procesarPublicacion(PPSCfd, buffer, configuracion->tiempoDado);
        if (bytesRead == 0)
        {
            printf("\n");
            printf("Esta emisión de noticias ha terminado. \n");
            printf("Puede presionar CTRL + Z para terminar... \n");
            printf("\n");
            break;
        }
        
        buffer[strcspn(buffer, "\n")] = '\0';
        enviarNoticia(buffer);
    }

    for(int i = 0; i < numeroSuscriptores; i++) {
        write(suscriptores[i].fd, "salida", 7);
    }
    close(PPSCfd);
    return NULL;
}

// Procesa los argumentos de la línea de comandos
// Recibe: argumento, valor y configuración
// Retorna: nada
void procesarArgumentos(char *arg, char *valor, Config *configuracion)
{
    if (strcmp(arg, "-p") == 0) {
        configuracion->pipePSC = valor;
    } else if (strcmp(arg, "-s") == 0) {
        configuracion->pipeSSC = valor;
    } else if (strcmp(arg, "-t") == 0) {
        configuracion->tiempoDado = atoi(valor);
    }
}

// Muestra un mensaje de error si faltan argumentos
// Recibe: nombre del programa
// Retorna: nada
void faltanArgumentos(char *nombrePrograma)
{
    fprintf(stderr, "Uso: %s -p pipePSC -s pipeSSC -t tiempoDado\n", nombrePrograma);
    exit(EXIT_FAILURE);
}

// Procesa los argumentos de la línea de comandos
// Recibe: argc, argv y configuración
// Retorna: nada
void argumentos(int argc, char *argv[], Config *configuracion) {
    if (argc < 7) {
        faltanArgumentos(argv[0]);
    }
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 < argc) {
            procesarArgumentos(argv[i], argv[i + 1], configuracion);
        } else {
            faltanArgumentos(argv[0]);
        }
    }
}




int main(int argc, char *argv[]) {
    Config configuracion = {NULL, NULL, 0};

    argumentos(argc, argv, &configuracion);

    // Creamos las pipes
    if (mkfifo(configuracion.pipePSC, 0666) == -1 && errno != EEXIST) {
        perror("Error creando pipe de publicadores");
        exit(EXIT_FAILURE);
    }
    if (mkfifo(configuracion.pipeSSC, 0666) == -1 && errno != EEXIST) {
        perror("Error creando pipe de suscriptores");
        exit(EXIT_FAILURE);
    }

    printf("\n");
    printf("Bienvenido al Sistema de Emisión de Noticias.\n");
    printf("El tiempo de transmisión será de %d segundos.\n", configuracion.tiempoDado);
    printf("\n");

    pthread_t hiloNoticias, hiloSuscriptores;
    pthread_create(&hiloSuscriptores, NULL, procesarSuscriptores, &configuracion);
    pthread_create(&hiloNoticias, NULL, procesoNoticias, &configuracion);

    // Esperamos a que los hilos terminen
    pthread_join(hiloSuscriptores, NULL);
    pthread_join(hiloNoticias, NULL);

    // Eliminamos las pipes
    unlink(configuracion.pipePSC);
    unlink(configuracion.pipeSSC);

    return 0;
}