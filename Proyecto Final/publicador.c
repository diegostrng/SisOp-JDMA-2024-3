/**********************
        Pontificia Universidad Javeriana
        Sistemas Operativos
    Autores: Juan Diego Muñoz Angulo, David Roa Neisa,
            Jorge Humberto Sierra Laiton, Michelle Gabriela Cruz Pino,
            Sofia Carolina Mantilla Vega
    Fecha: 13 Noviembre 2024
    Tema: Sistema de Noticias
          Proyecto Sistemas Operativos
          Sistema de Emisión de Noticias

**********************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

/*

    Este programa implementa un publicador que publica noticias y las envia al sistema.
    Las noticias se leen de un archivo de texto y se publican.

*/

#define MAX_BUFFER 256

// Estructura para almacenar la configuración, esta en todos los programas
typedef struct {
    char *pipePSC;
    char *file;
    int tiempo;
} Config;

// Función para procesar los argumentos de la línea de comandos
// Recibe: argumentos de la línea de comandos, configuración
// Retorna: nada
void argumentos(int argc, char *argv[], Config *configuracion)
{
    if (argc != 7) {
        fprintf(stderr, "Uso: %s -p pipePSC -f file -t tiempo\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    configuracion->pipePSC = argv[2];
    configuracion->file = argv[4];
    configuracion->tiempo = atoi(argv[6]);
}

// Función para verificar el formato de la noticia
// Recibe una cadena de caracteres
// Retorna: 1 si el formato es válido, 0 en caso contrario
int verificar(char *linea)
{

    // Eliminar espacios al principio y al final de la línea
    char *start = linea;
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r' || *start == '\f' || *start == '\v') start++;
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r' || *end == '\f' || *end == '\v')) end--;
    *(end + 1) = '\0';

    // Verificar longitud mínima
    if (strlen(start) < 3) return 0;

    // Verificar primer carácter
    if (strchr("PAESC", start[0]) == NULL) return 0;

    // Verificar segundo carácter
    if (start[1] != ':') return 0;

    // Verificar último carácter
    if (start[strlen(start) - 1] != '.') return 0;

    return 1;
}

// Función para publicar una noticia
// Recibe: file descriptor del pipe, nombre del archivo y tiempo de publicación
// Retorna: nada
void publicar(int fdPSC, const char *file, int tiempo){

    FILE *archivo = fopen(file, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de noticias.");
        close(fdPSC);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (verificar(buffer)) {
            write(fdPSC, buffer, strlen(buffer) + 1);
            sleep(tiempo);
        } else {
            fprintf(stderr, "Formato incorrecto: %s \n", buffer);
            printf("\n");
        }
    }

    fclose(archivo);
}


int main(int argc, char *argv[]) {
    Config configuracion;
    argumentos(argc, argv, &configuracion);

    int fdPSC = open(configuracion.pipePSC, O_WRONLY);
    if (fdPSC == -1)
    {
        perror("Error al abrir el pipe");
        exit(EXIT_FAILURE);
    }

    printf("\n");
    printf("Publicando noticia...\n");
    publicar(fdPSC, configuracion.file, configuracion.tiempo);

    close(fdPSC);
    printf("\n");
    printf("Noticia publicada exitosamente.\n");
    printf("\n");
    
    return 0;
}
