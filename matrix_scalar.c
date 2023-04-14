#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "execution_time.c"
#include "matrix.c"
#include <pthread.h>

typedef struct {
    Matrix* M;
    double k;
    int start_row;
    int end_row;
    pthread_mutex_t* mutex;
} ScalarMatrixArgs;

void* scalar_matrix_parallel(void* arg) {
    ScalarMatrixArgs* args = (ScalarMatrixArgs*) arg;
    for (int i = args->start_row; i < args->end_row; ++i) {
        for (int j = 0; j < args->M->cols; ++j) {
            pthread_mutex_lock(args->mutex);
            args->M->elements[i][j] *= args->k;
            pthread_mutex_unlock(args->mutex);
        }
    }
    return NULL;
}

void multiply_matrix_by_scalar(int rows, int cols, double scalar);
void validate_data(int rows, int cols, double scalar);
void normal_multiply_matrix_by_scalar(Matrix* M,double scalar);
void parallel_multiply_matrix_by_scalar(Matrix* M,double scalar);


void multiply_matrix_by_scalar(int rows, int cols, double scalar){
    Matrix* M = create_matrix(rows, cols);
    init_matrix_rand(M);
    print_matrix(M);
    Matrix* M1 = create_matrix(rows, cols);
    copy_matrix(M1,M);
    normal_multiply_matrix_by_scalar(M1,scalar);
    Matrix* M2 = create_matrix(rows, cols);
    copy_matrix(M2,M);
    parallel_multiply_matrix_by_scalar(M2,scalar);
    free_matrix(M);
    
}

void normal_multiply_matrix_by_scalar(Matrix* M,double scalar){
    printf("\nNormal multiply matrix by scalar\n");
    struct timeval start, end;
    gettimeofday(&start, NULL);
    scalar_matrix(M,scalar);
    gettimeofday(&end, NULL);
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}

void parallel_multiply_matrix_by_scalar(Matrix* M,double scalar){
    printf("\nParallel multiply matrix by scalar\n");
    struct timeval start, end;
    const int num_threads = 4;  // Número de hilos
    pthread_t threads[num_threads];
    ScalarMatrixArgs thread_args[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

    gettimeofday(&start, NULL);
    const int chunk_size = M->rows / num_threads;
    int start_row = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_row = start_row + chunk_size;
        if (i == num_threads - 1) {
            end_row = M->rows;
        }
        thread_args[i] = (ScalarMatrixArgs) { .M = M, .k = scalar, .start_row = start_row, .end_row = end_row, .mutex = &mutex };
        pthread_create(&threads[i], NULL, scalar_matrix_parallel, &thread_args[i]);
        start_row = end_row;
    }

    // Esperamos a que todos los hilos terminen
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex); // Destruimos el mutex
    gettimeofday(&end, NULL);
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}