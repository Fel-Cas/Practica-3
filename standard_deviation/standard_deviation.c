#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "./standard_deviation.h"
#include "../execution_time/time.h"

// Estructura para los argumentos del hilo
typedef struct {
    int start;
    int end;
    Matrix* matriz;
    Vector* result;
    pthread_mutex_t* mutex;
} ThreadArgs;

void calcular_desviacion_estandar_sin_paralelismo(Matrix* matriz){
    // Obtener el tiempo de inicio
    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);

    // Calcular la desviación estándar sin paralelismo
    Vector *std_col = matrix_col_std(matriz);

    // Obtener el tiempo de fin
    gettimeofday(&end_time, 0);

    // Calcular el tiempo de ejecución sin paralelismo
    printf("Sin paralelismo: \n");
    get_execution_time(start_time, end_time);
    print_vector(std_col);
}

// Función de hilo para calcular la desviación estándar de una columna de una matriz
void* calcularDesviacionEstandarColumnaHilo(void* dato) {
    ThreadArgs* datos = (ThreadArgs*) dato;

    Vector* mean = matrix_col_mean(datos->matriz);
    for (int i = 0; i < datos->matriz->cols; ++i) {
        double sum = 0.0;
        for (int j = 0; j < datos->matriz->rows; ++j) {
            // Bloquear el mutex antes de actualizar la variable que se comparte
            pthread_mutex_lock(datos->mutex);
            sum += pow(datos->matriz->elements[j][i] - mean->elements[i], 2);
            // Desbloquear el mutex después de actualizar la variable que se comparte
            pthread_mutex_unlock(datos->mutex);
        }
        pthread_mutex_lock(datos->mutex);
        datos->result->elements[i] = sqrt(sum / (datos->matriz->cols-1));
        pthread_mutex_unlock(datos->mutex);
    }
    pthread_exit(NULL);
    return NULL;
}

void calcularDesviacionEstandarParalelo(Matrix *matriz, int num_threads){

    // Obtener el tiempo de inicio
    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);

    pthread_t threads[num_threads];
    ThreadArgs data[num_threads];
    Vector* result = create_vector(matriz->cols);
    // Se inicializa el lock
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // Crear hilos y calcular la desviación estándar en paralelo
    // Se divide la matriz en partes iguales para cada hilo
    const int chunk_size = matriz->cols / num_threads;
    // Se le asigna a cada hilo una parte de la matriz
    int start_col = 0;
    for (int i = 0; i < num_threads; ++i)
    {
        int end_col = start_col + chunk_size;
        if (i == num_threads - 1)
        {
            end_col = matriz->cols;
        }
        // Se inicializan los datos necesarios para cada hilo
        data[i] = (ThreadArgs){.start = start_col, .end = end_col, .matriz = matriz, .result = result, .mutex = &mutex};
        pthread_create(&threads[i], NULL, calcularDesviacionEstandarColumnaHilo, &data[i]);
        start_col = end_col;
    }

    // Esperar a que todos los hilos terminen
    for (int j = 0; j < num_threads; j++) {
        pthread_join(threads[j], NULL);
    }
    
    // Se destruye el lock
    pthread_mutex_destroy(&mutex);
    // Obtener el tiempo de fin
    gettimeofday(&end_time, 0);

    // Calcular el tiempo de ejecución con pthread
    printf("Con pthread \n");
    print_vector(result);
    get_execution_time(start_time, end_time);
}

int calculate_standard_deviation_by_column(int rows, int cols, int num_threads) {
    // Crear y llenar la matriz
    Matrix* matriz = create_matrix(rows, cols);
    //Se inicializa la matriz con numeros aleatorios
    init_matrix_rand(matriz);
    //Se imprime la matriz
    print_matrix(matriz);

    calcular_desviacion_estandar_sin_paralelismo(matriz);

    // Calcular la desviación estándar usando pthread
    calcularDesviacionEstandarParalelo(matriz, num_threads);

    free_matrix(matriz);

    return 0;
}