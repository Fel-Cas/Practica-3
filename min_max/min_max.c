#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "./min_max.h"
#include "../execution_time/time.h"

// Define los argumentos que se le pasan a cada hilo en el método min_max_cols_thread
typedef struct {
    struct Matrix* matriz;
    Vector* min_values;
    Vector* max_values;
    int start_col;
    int end_col;
    pthread_mutex_t* mutex;
} MinMaxArgs;

typedef struct {
    double min_value;
    double max_value;
    pthread_mutex_t mutex;
} MinMax;

// Método que se ejecuta en cada hilo para calcular los valores mínimos y máximos de las columnas que le corresponden


void* min_cols_thread(void* arg){
    MinMaxArgs* args = (MinMaxArgs*) arg;

    for (int i = 0; i < args->matriz->cols; ++i) {
        double min = args->matriz->elements[0][i];
        for (int j = 0; j < args->matriz->rows; ++j) {
            if (min > args->matriz->elements[j][i]) {
                pthread_mutex_lock(args->mutex);
                min = args->matriz->elements[j][i];
                pthread_mutex_unlock(args->mutex);
            }
        }
        pthread_mutex_lock(args->mutex);
        args->min_values->elements[i] = min;
        pthread_mutex_unlock(args->mutex);
    }
    pthread_exit(NULL);
}

void* max_cols_thread(void* arg){
    MinMaxArgs* args = (MinMaxArgs*) arg;

    for (int i = 0; i < args->matriz->cols; ++i) {
        double max = args->matriz->elements[0][i];
        for (int j = 0; j < args->matriz->rows; ++j) {
            if (max < args->matriz->elements[j][i]) {
                pthread_mutex_lock(args->mutex);
                max = args->matriz->elements[j][i];
                pthread_mutex_unlock(args->mutex);
            }
        }
        pthread_mutex_lock(args->mutex);
        args->max_values->elements[i] = max;
        pthread_mutex_unlock(args->mutex);
    }
    pthread_exit(NULL);
}

// Método que utiliza paralelismo para calcular los valores mínimos y máximos de todas las columnas de una matriz
void min_max_cols_parallel(Matrix* matriz, int num_threads){
    struct timeval start_time, end_time;
    gettimeofday(&start_time, 0);

    pthread_t threads[num_threads];
    MinMaxArgs min_max_args[num_threads];
    Vector* min_values = create_vector(matriz->cols);
    Vector* max_values = create_vector(matriz->cols);
    // Se inicializa el lock
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
        min_max_args[i] = (MinMaxArgs){.start_col = start_col, .end_col = end_col, .matriz = matriz, .min_values = min_values, .max_values = max_values, .mutex = &mutex};
        pthread_create(&threads[i], NULL, min_cols_thread, &min_max_args[i]);
        start_col = end_col;
        pthread_create(&threads[i], NULL, max_cols_thread, &min_max_args[i]);
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
    printf("\nCon pthread \n");
    get_execution_time(start_time, end_time);

    printf("Valores máximos: \n");
    print_vector(max_values);

    printf("Valores mínimos: \n");
    print_vector(min_values);
}

// Método que calcula los valores mínimos y máximos de todas las columnas de una matriz sin paralelismo
void min_max_cols_without_parallelism(Matrix* matrix){
    struct timeval start_time, end_time;    
    gettimeofday(&start_time, 0); // Se inicializa el tiempo

    // Método que encuentra los valores máximos de la columna
    Vector *max_col = matrix_col_max(matrix);

    Vector *min_col = matrix_col_min(matrix);

    gettimeofday(&end_time, 0);

    printf("\nSin paralelismo: \n");
    get_execution_time(start_time, end_time);

    printf("Valores máximos: \n");
    print_vector(max_col);

    printf("Valores mínimos: \n");
    print_vector(min_col);

}

void calculate_min_max_by_columns(int rows, int cols, int num_threads) {
    // Crear y llenar la matriz
    Matrix* matrix = create_matrix(rows, cols);
    //Se inicializa la matriz con numeros aleatorios
    init_matrix_rand(matrix);
    //Se imprime la matriz
    print_matrix(matrix);

    min_max_cols_without_parallelism(matrix);
    
    min_max_cols_parallel(matrix, num_threads);

    free(matrix);
}