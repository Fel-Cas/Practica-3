#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "../execution_time/time.h" 
#include "./normalize_formula2.h"

typedef struct {
    int start_row;
    int end_row;
    Matrix* M;
    Vector* vrz;
    Vector* std;
    pthread_mutex_t* mutex;
} thread_args_t;

void* normalize_matrix_column_formula_2_row_thread(void* args) {
    thread_args_t* thread_args = (thread_args_t*) args;
    for (int i = thread_args->start_row; i < thread_args->end_row; ++i) {
        for (int j = 0; j < thread_args->M->cols; ++j) {
            pthread_mutex_lock(thread_args->mutex);
            thread_args->M->elements[i][j] = (thread_args->M->elements[i][j] - thread_args->vrz->elements[j]) / thread_args->std->elements[j];
            pthread_mutex_unlock(thread_args->mutex);
        }
    }
    pthread_exit(NULL);
}

void main_normalize_formula_2(int rows, int cols, int n){
    Matrix* M = create_matrix(rows, cols);
    init_matrix_rand(M);
    Vector* vrz = matrix_col_vrz(M);
    Vector* std = matrix_col_std(M);
    print_matrix(M);   
    Matrix* M1 = create_matrix(rows, cols);
    copy_matrix(M1,M);
    execute_normalize_formula_2_normal(M1, vrz,std);
    Matrix* M2 = create_matrix(rows, cols);
    copy_matrix(M2,M);
    normalize_matrix_column_formula_2_parallel(M2, vrz,std,n);
    free_matrix(M);

}

void execute_normalize_formula_2_normal(Matrix* M, Vector* vrz, Vector* std){
    printf("\nNormalize matrix  formula 2 without Parallel programming\n");
    struct timeval start, end;
    gettimeofday(&start, 0);
    normalize_matrix_column_formula_2(M, vrz,std);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}

void normalize_matrix_column_formula_2_parallel(Matrix* M, Vector* vrz, Vector* std, int num_threads) {
    printf("\nNormalize matrix formula 2 with Parallel programming\n");
    struct timeval start, end;
    pthread_t threads[num_threads];
    thread_args_t thread_args[num_threads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    gettimeofday(&start, 0);
    int chunk_size = M->rows / num_threads;
    int extra_rows = M->rows % num_threads;
    int start_row = 0;
    for (int i = 0; i < num_threads; ++i) {
        int rows = chunk_size + (i < extra_rows);
        thread_args[i].start_row = start_row;
        thread_args[i].end_row = start_row + rows;
        thread_args[i].M = M;
        thread_args[i].vrz = vrz;
        thread_args[i].std = std;
        thread_args[i].mutex = &mutex;
        start_row += rows;
        pthread_create(&threads[i], NULL, normalize_matrix_column_formula_2_row_thread, &thread_args[i]);
    }
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}