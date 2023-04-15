#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "./normalize_formula1.h"
#include "../execution_time/time.h" 

typedef struct {
    Matrix* M;
    Vector* mayores;
    Vector* menores;
    int start_col;
    int end_col;
    pthread_mutex_t* lock;
} NormalizeArgs;

void* normalize_columns_formula_1(void* args_ptr) {
    NormalizeArgs* args = (NormalizeArgs*) args_ptr;

    for (int j = args->start_col; j < args->end_col; ++j) {
       

        for (int i = 0; i < args->M->rows; ++i) {
            pthread_mutex_lock(args->lock);
            args->M->elements[i][j] = (args->M->elements[i][j] - args->menores->elements[j]) / (args->mayores->elements[j] - args->menores->elements[j]);
            pthread_mutex_unlock(args->lock);
        }

        
    }

    return NULL;
}

void main_normalize_formula_1(int rows, int cols){
    //Se crea la matriz
    Matrix* M = create_matrix(rows, cols);
    //Se inicializa la matriz con numeros aleatorios
    init_matrix_rand(M);
    //Se obtienen los numeros mayores y menores de cada columna
    Vector* larger_numbers = matrix_col_max(M);
    Vector* minors_numbers = matrix_col_min(M);
    //Se imprime la matriz
    print_matrix(M);   
    //Creamos otra matriz auxiliar para realizar las operaciones
    Matrix* M1 = create_matrix(rows, cols);
    //Copiamos la matriz original a la matriz que creamos
    copy_matrix(M1,M);
    //Llamamos a la funcion que realiza la normalizacion de la matriz por un escalar sin usar paralelismo
    normalize_matrix(M1, larger_numbers, minors_numbers);
    Matrix* M2 = create_matrix(rows, cols);
    //Se copia la matriz original a la matriz auxiliar
    copy_matrix(M2,M);
    //Se llama a la funcion que realiza la normalicón de la matriz por un escalar usando paralelismo
    normalize_matrix_with_parallel_programming(M2,larger_numbers, minors_numbers);
    //Se libera la memoria de las matrices
    free_matrix(M);
}

void normalize_matrix(Matrix* M, Vector* larger_numbers, Vector* minors_numbers){
    printf("\nNormalize matrix  formula 1 without Parallel programming\n");
    struct timeval start, end;
    gettimeofday(&start, 0);
    normalize_matrix_column_formula_1(M, larger_numbers, minors_numbers);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}

void normalize_matrix_with_parallel_programming(Matrix* M, Vector* larger_numbers, Vector* minors_numbers){
     printf("\nNormalize matrix  formula 1 with Parallel programming\n");

    struct timeval start, end;
    const int num_threads = 4;  // Número de hilos
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    gettimeofday(&start, 0);
    pthread_t threads[num_threads];
    NormalizeArgs args[num_threads];

    int chunk_size = M->cols / num_threads;
    int extra_cols = M->cols % num_threads;

    for (int i = 0; i < num_threads; ++i) {
        args[i].M = M;
        args[i].mayores = larger_numbers;
        args[i].menores = minors_numbers;
        args[i].lock = &lock;

        int start_col = i * chunk_size;
        int end_col = (i + 1) * chunk_size;

        if (i == num_threads - 1) {
            end_col += extra_cols;
        }

        args[i].start_col = start_col;
        args[i].end_col = end_col;

        pthread_create(&threads[i], NULL, normalize_columns_formula_1, &args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    gettimeofday(&end, 0);
    //Se imprime el tiempo de ejecucion y el resultado de la multiplicación
    get_execution_time(start, end);
    //Se imprime la matriz
    print_matrix(M);
    //Se libera la memoria de la matriz
    free_matrix(M);
}
