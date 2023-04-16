#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "../execution_time/time.h" 
#include "./normalize_formula2.h"
#include "../validations/validation.h"

typedef struct {
    int start_row;
    int end_row;
    Matrix* M;
    Vector* vrz;
    Vector* std;
    pthread_mutex_t* mutex;
} thread_args_t;

typedef struct {
    Vector* v;
    float vrz;
    float std;
    int start_index;
    int end_index;
    pthread_mutex_t* mutex;
} ThreadArgs;

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

// Define the function to be executed by each thread
void* normalize_vector_formula_2_thread(void* arg) {
    ThreadArgs* args = (ThreadArgs*) arg;
    // Iterate over the assigned range of the vector
    for (int i = args->start_index; i < args->end_index; ++i) {
        // Acquire the lock to ensure mutual exclusion
        pthread_mutex_lock(args->mutex);
        // Perform the normalization calculation
        args->v->elements[i] = (args->v->elements[i] - args->vrz) / args->std;
        // Release the lock
        pthread_mutex_unlock(args->mutex);
    }

    return NULL;
}

void main_normalize_formula_2(int rows, int cols, int n,int file){
   validate_data_operation_with_one_matrix(rows, cols, n);
    if(rows==1){
        main_normalize_vector_formula2(cols, n, file);
    }else{
        main_normalize_matrix_formula2(rows, cols, n, file);
    }

}

void main_normalize_vector_formula2(int cols,int n, int file){
    Vector* v=NULL;
    if(file==1){
        v = create_vector_from_file("op1.txt",cols);
    }else{
        v = create_vector(cols);
        init_vector_rand(v);
    }
    printf("El vector es: \n");
    print_vector(v);
    float vrz =vrz_vector(v);
    printf("La media es: %f\n",vrz);    
    float std = std_vector(v);
    printf("La desviacion estandar es: %f\n",std);
    Vector* v1 = create_vector(cols);
    copy_vector(v1,v);
    normalize_vector_formula2_without_parallel_programming(v1, vrz,std);
    Vector* v2 = create_vector(cols);
    copy_vector(v2,v);
    normalize_vector_formula2_with_parallel_programming(v2, vrz,std,n);
    free_vector(v);
}
void main_normalize_matrix_formula2(int rows, int cols, int n, int file){
    Matrix* M=NULL;
    if(file==1){
        M = create_matrix_from_file("op1.txt",rows, cols);
    }else{
        M = create_matrix(rows, cols);
        init_matrix_rand(M);
    }
    print_matrix(M); 
    printf("La media de las columnas es: \n");
    Vector* vrz = matrix_col_vrz(M);
    print_vector(vrz);
    printf("La desviacion estandar de las columnas es: \n");
    Vector* std = matrix_col_std(M);
    print_vector(std);  
    Matrix* M1 = create_matrix(rows, cols);
    copy_matrix(M1,M);
    normalize_matrix_formula2_without_parallel_programming(M1, vrz,std);
    Matrix* M2 = create_matrix(rows, cols);
    copy_matrix(M2,M);
    normalize_matrix_formula2_with_parallel_programming(M2, vrz,std,n);
    free_matrix(M);
}
void normalize_vector_formula2_without_parallel_programming(Vector* v, float vrz, float std){
    printf("\nNormalize vector formula 2 without Parallel programming\n");
    struct timeval start, end;
    gettimeofday(&start, 0);
    normalize_vector_formula_2(v,vrz,std);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_vector(v);
    free_vector(v);
}
void normalize_matrix_formula2_without_parallel_programming(Matrix* M, Vector* vrz, Vector* std){
    printf("\nNormalize matrix  formula 2 without Parallel programming\n");
    struct timeval start, end;
    gettimeofday(&start, 0);
    normalize_matrix_column_formula_2(M, vrz,std);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}

// Define the parallel version of the function
void normalize_vector_formula2_with_parallel_programming(Vector* v, float vrz, float std, int num_threads) {
    printf("\nNormalize vector formula 2 with Parallel programming\n");
    // Calculate the number of elements per thread
    int chunk_size = v->size / num_threads;
    // Initialize the mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    // Initialize an array to hold the thread objects
    pthread_t threads[num_threads];
    // Initialize an array to hold the thread arguments
    ThreadArgs args[num_threads];
    // Assign each thread its range of the vector and other arguments
    struct timeval start, end;
    gettimeofday(&start, 0);
    for (int i = 0; i < num_threads; ++i) {
        args[i].v = v;
        args[i].vrz = vrz;
        args[i].std = std;
        args[i].mutex = &mutex;
        args[i].start_index = i * chunk_size;
        args[i].end_index = (i == num_threads - 1) ? v->size : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, normalize_vector_formula_2_thread, &args[i]);
    }
    // Join all threads to wait for them to complete
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_vector(v);
    free_vector(v);
}

void normalize_matrix_formula2_with_parallel_programming(Matrix* M, Vector* vrz, Vector* std, int num_threads) {
    printf("\nNormalize matrix formula 2 with Parallel programming\n");
    struct timeval start, end;
    pthread_t threads[num_threads];
    thread_args_t thread_args[num_threads];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    int chunk_size = M->rows / num_threads;
    int extra_rows = M->rows % num_threads;
    int start_row = 0;
    gettimeofday(&start, 0);
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