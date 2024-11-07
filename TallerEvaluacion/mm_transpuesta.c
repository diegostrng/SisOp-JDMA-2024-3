/**************************************************************
		Pontificia Universidad Javeriana
	Autor: J. Corredor
	Fecha: Febrero 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


#define DATA_SIZE (1024*1024*64*3) // Tamaño del bloque de memoria para las matrices

pthread_mutex_t MM_mutex; // Mutex para sincronización entre hilos
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC; // Punteros a las matrices A B C

struct parametros{
    int nH;
    int idH;
    int N;
};

struct timeval start, stop;

// Función para llenar las matrices
// Recibe: el tamaño de la matriz (SZ)
// Sirve para: inicializar las matrices mA y mB con valores específicos y mC con ceros
void llenar_matriz(int SZ){ 
    srand48(time(NULL));
    for(int i = 0; i < SZ*SZ; i++){
            mA[i] = 1.1*i;
            mB[i] = 2.2*i;
            mC[i] = 0;
        }	
}

// Función para imprimir la matriz
// Recibe: el tamaño de la matriz (sz) y un puntero a la matriz (matriz)
// Sirve para: imprimir la matriz si su tamaño es menor a 12
void print_matrix(int sz, double *matriz){
    if(sz < 12){ // Solo imprime la matriz si el tamaño es menor a 12
            for(int i = 0; i < sz*sz; i++){
                     if(i%sz==0) printf("\n");
                    printf(" %.3f ", matriz[i]);
            }	
        printf("\n>-------------------->\n");
    }
}

// Función para marcar el inicio del tiempo
// Sirve para: marcar el inicio del tiempo de ejecución
void inicial_tiempo(){
    gettimeofday(&start, NULL); // Marca el inicio del tiempo
}

// Función para marcar el final del tiempo
// Sirve para: marcar el final del tiempo de ejecución y calcular el tiempo transcurrido
void final_tiempo(){
    gettimeofday(&stop, NULL); // Marca el final del tiempo
    stop.tv_sec -= start.tv_sec;
    printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec)); // Calcula y muestra el tiempo transcurrido en microsegundos
}


// Función para la multiplicación de matrices usando hilos
// Recibe: un puntero a una estructura de parámetros (variables)
// Sirve para: realizar la multiplicación de matrices en paralelo
void *mult_thread(void *variables){
    struct parametros *data = (struct parametros *)variables;
    
    int idH = data->idH;
    int nH  = data->nH;
    int N   = data->N;
    int ini = (N/nH)*idH;
    int fin = (N/nH)*(idH+1); 

    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
            double *pA, *pB, sumaTemp = 0.0;
            pA = mA + (i*N); 
            pB = mB + (j*N);
            for (int k = 0; k < N; k++, pA++, pB++){
                sumaTemp += (*pA * *pB); // Multiplica y acumula los valores
            }
            mC[i*N+j] = sumaTemp;
        }
    }

    pthread_mutex_lock (&MM_mutex); // Bloquea el mutex
    pthread_mutex_unlock (&MM_mutex); // Desbloquea el mutex
    pthread_exit(NULL); // Termina el hilo
}

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;	
    }
    int SZ = atoi(argv[1]);
    int n_threads = atoi(argv[2]);

    pthread_t p[n_threads];
    pthread_attr_t atrMM;

    mA = MEM_CHUNK;
    mB = mA + SZ*SZ;
    mC = mB + SZ*SZ;

    llenar_matriz(SZ); //Llena las matrices con valores iniciales
    print_matrix(SZ, mA); //Imprime  A
    print_matrix(SZ, mB); //Imprime B

    inicial_tiempo();
    pthread_mutex_init(&MM_mutex, NULL); // Inicializa el mutex
    pthread_attr_init(&atrMM); // Inicializa los atributos de los hilos
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); 

    for (int j=0; j<n_threads; j++){
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
        datos->idH = j;
        datos->nH  = n_threads;
        datos->N   = SZ;
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);
    }

    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);
    final_tiempo();
    
    print_matrix(SZ, mC); // Imprime C

    pthread_attr_destroy(&atrMM); // Destruye los atributos
    pthread_mutex_destroy(&MM_mutex); // Elimina el mutex
    pthread_exit (NULL);
}
