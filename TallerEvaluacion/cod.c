/******************************************
*                                          *
* Autor: Juan Diego Muñoz Angulo           *
* Materia: Sistemas Operativos             *
* Fecha: 07-11-24                          *
* Tema: Parcial 3                          *
*                                          *
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int turno = 0;

void* procFuncion(void* arg)
{
    long id = (long)arg;

    while(1)
    {
        pthread_mutex_lock(&mutex);

        if(turno == id)
        {
            printf("Hilo %ld iniciado\n", id);

            for (int i = 0; i < 300; i++)
            {
                usleep(6000);
            }

            printf("Hilo %ld acabado \n", id);
            pthread_mutex_unlock(&mutex);
            turno++;
            break;
        }

        pthread_mutex_unlock(&mutex);
        usleep(1000);
    }

    return NULL;
}

int main()
{
    long ids[5] = {0, 1, 2, 3, 4};
    pthread_t threads[5];
    
    for(int i = 0; i < 5; i++)
    {
        if(pthread_create(&threads[i], NULL, procFuncion, (void*)ids[i]) != 0)
        {
            return 1;
        }
    }

    for(int i = 0; i < 5; i++)
    {

        pthread_join(threads[i], NULL);
    
    }

    return 0;
}