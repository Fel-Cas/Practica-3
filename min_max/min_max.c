#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "./min_max.h"
#include "../execution_time/time.h"

// Define los argumentos que se le pasan a cada hilo en el método min_max_cols_thread
typedef struct {
    struct Matrix* matrix;
    double* min_values;
    double* max_values;
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
void* min_max_cols_thread(void* arg) {
    // Castear argumento
    MinMaxArgs* thread_arg = (MinMaxArgs*) arg;
    Matrix* matrix = thread_arg->matrix;
    MinMax* min_max = thread_arg->min_values;
    int start_col = thread_arg->start_col;
    int end_col = thread_arg->end_col;

    // Calcular valores mínimo y máximo para el rango de columnas del hilo
    min_max_cols_range(matrix, min_max, start_col, end_col);

    // Liberar memoria
    free(thread_arg);

    // Salida del hilo
    pthread_exit(NULL);
}

void min_max_cols_range(Matrix* matrix, void* min_max, int start_col, int end_col) {
    MinMax* thread_arg = (MinMax*) min_max;
    int i, j;
    double min_value = matrix->elements[0][start_col];
    double max_value = matrix->elements[0][start_col];

    // Recorrer rango de columnas y actualizar valores mínimo y máximo
    for (j = start_col; j < end_col; j++) {
        for (i = 0; i < matrix->rows; i++) {
            if (matrix->elements[i][j] < min_value) {
                min_value = matrix->elements[i][j];
            }
            if (matrix->elements[i][j] > max_value) {
                max_value = matrix->elements[i][j];
            }
        }
    }

    // Bloquear mutex y actualizar valores mínimo y máximo compartidos
    pthread_mutex_lock(&thread_arg->mutex);
    if (min_value < thread_arg->min_value) {
        thread_arg->min_value = min_value;
    }
    if (max_value > thread_arg->max_value) {
        thread_arg->max_value = max_value;
    }
    // Se desbloquea el mutex    
    pthread_mutex_unlock(&thread_arg->mutex);
}

// Método que utiliza paralelismo para calcular los valores mínimos y máximos de todas las columnas de una matriz
void min_max_cols_parallel(Matrix* matrix, double* min_values, double* max_values, int num_threads){
    pthread_t threads[num_threads];
    MinMaxArgs* thread_args[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int cols_per_thread = matrix->cols / num_threads;
    int extra_cols = matrix->cols % num_threads;
    int start_col = 0;
    for (int i = 0; i < num_threads; i++) {
        int end_col = start_col + cols_per_thread;
        if (i == num_threads - 1) {
            end_col += extra_cols;
        }
        thread_args[i]->matrix = matrix;
        thread_args[i]->min_values = min_values;
        thread_args[i]->max_values = max_values;
        thread_args[i]->start_col = start_col;
        thread_args[i]->end_col = end_col;
        thread_args[i]->mutex = &mutex;
        pthread_create(&threads[i], NULL, min_max_cols_thread, &thread_args[i]);
        start_col = end_col;
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Método que calcula los valores mínimos y máximos de todas las columnas de una matriz sin paralelismo
void min_max_cols(Matrix* matrix, double* min_values, double* max_values){
    for (int j = 0; j < matrix->cols; j++) {
        double min_value = matrix->elements[0][j];
        double max_value = matrix->elements[0][j];
        for (int i = 1; i < matrix->rows; i++) {
            if (matrix->elements[i][j] < min_value) {
                min_value = matrix->elements[i][j];
            }
            if (matrix->elements[i][j] > max_value) {
                max_value = matrix->elements[i][j];
            }
        }
        min_values[j] = min_value;
        max_values[j] = max_value;
    }
}

int calculate_min_max_by_columns(int rows, int cols, int num_threads) {
    // Crear y llenar la matriz
    Matrix* matrix = create_matrix(rows, cols);
    //Se inicializa la matriz con numeros aleatorios
    init_matrix_rand(matrix);
    //Se imprime la matriz
    print_matrix(matrix);

    //Se crea una copia de la matrix para no modificar la original
    Matrix* matrix2 = create_matrix(rows, cols);
    copy_matrix(matrix2, matrix);

    // Calcula los valores mínimos y máximos de cada columna sin paralelismo
    double min_values[matrix2->cols];
    double max_values[matrix2->cols];

    struct timeval start_time;    
    gettimeofday(&start_time, 0); // Se inicializa el tiempo

    min_max_cols(matrix2, min_values, max_values);

    struct timeval end_time;
    gettimeofday(&end_time, 0);

    // Imprime la matriz y los valores mínimos y máximos de cada columna
    printf("Matrix:\n");
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%f ", matrix->elements[i][j]);
        }
        printf("\n");
    }
    printf("Min/max values of each column (without parallelism):\n");
    for (int j = 0; j < matrix->cols; j++) {
        printf("Column %d: %f (min), %f (max)\n", j, min_values[j], max_values[j]);
    }

    Matrix* matrix3 = create_matrix(rows, cols);
    copy_matrix(matrix3, matrix);

    // Calcula los valores mínimos y máximos de cada columna utilizando paralelismo
    for (int j = 0; j < matrix3->cols; j++) {
        min_values[j] = matrix3->elements[0][j]; // Inicializa el valor mínimo de la columna con el primer elemento
        max_values[j] = matrix3->elements[0][j]; // Inicializa el valor máximo de la columna con el primer elemento
        printf("valores mínimimos %f y máximos %f", min_values[j], max_values[j]);
    }
    gettimeofday(&start_time, 0);
    min_max_cols_parallel(matrix3, min_values, max_values, num_threads);
    gettimeofday(&end_time, 0);
    // Imprime los nuevos valores mínimos y máximos de cada columna
    printf("Min/max values of each column (with parallelism):\n");
    for (int j = 0; j < matrix3->cols; j++) {
        printf("Column %d: %f (min), %f (max)\n", j, min_values[j], max_values[j]);
    }

    // Calcular el tiempo de ejecución con pthread
    printf("Con pthread \n");
    get_execution_time(start_time, end_time);

    // Libera la memoria utilizada por la matriz
    free(matrix);

    return 0;
}