/******************************************
*                                          *
* Autor: Juan Diego Muñoz Angulo           *
* Materia: Sistemas Operativos             *
* Fecha: 24-10-24                          *
* Tema: POSIX                              *
*                                          *
*******************************************/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int counter = 0;
int max = 4;

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condc_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVarProd = PTHREAD_COND_INITIALIZER;
pthread_cond_t condVarCons = PTHREAD_COND_INITIALIZER;

// Función que ejecuta el hilo productor
void *productor(void *arg) {
    while(1) {

        pthread_mutex_lock(&condp_mutex);

        while(counter >= 10) {
            pthread_cond_wait(&condVarProd, &condp_mutex);
        }
        
        pthread_mutex_unlock(&condp_mutex);

        pthread_mutex_lock(&counter_mutex);
        counter++;
        
        // Señala al consumidor que el contador ha cambiado
        pthread_cond_signal(&condVarCons);
        
        printf("Soy productor %d valor contador = %d\n", (int)pthread_self(), counter);
        
        // Desbloquea el mutex del productor
        pthread_mutex_unlock(&condp_mutex);
        pthread_mutex_unlock(&counter_mutex);

        if(counter == 5) {
            sleep(1);
        }
    }
    return NULL;
}

// Función que ejecuta el hilo consumidor
void *consumidor(void *arg) {
    while(1) {

        sleep(1);

        pthread_mutex_lock(&condc_mutex);
    
        while(counter <= 0) {
            // Señala al productor que el contador ha cambiado
            pthread_cond_signal(&condVarProd);
            
            // Espera a que el productor cambie el contador
            pthread_cond_wait(&condVarCons, &condc_mutex);
        }
        pthread_mutex_unlock(&condc_mutex);
        pthread_mutex_lock(&counter_mutex);
        

        if(counter > 0) {
            printf("Soy consumidor %d valor contador = %d\n", (int)pthread_self(), counter);
            counter--;

            pthread_cond_signal(&condVarProd);
        }

        pthread_mutex_unlock(&counter_mutex);
        pthread_mutex_unlock(&condc_mutex);
    }
    return NULL;
}

int main() {

    pthread_t proHilo[max], conHilo[max];
    int i;
    
    // Crea los hilos productores y consumidores
    for(i = 0; i < 5; i++) {
        pthread_create(&proHilo[i], NULL, &productor, NULL);
        pthread_create(&conHilo[i], NULL, &consumidor, NULL);
    }
    
    // Espera a que todos los hilos terminen
    for(i = 0; i < 5; i++) {
        pthread_join(proHilo[i], NULL);
        pthread_join(conHilo[i], NULL);
    }

    return 0;
}