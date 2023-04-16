#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "./normalize_formula1.h"
#include "../execution_time/time.h" 
#include "../validations/validation.h"

typedef struct {
    Matrix* M;
    Vector* mayores;
    Vector* menores;
    int start_row;
    int end_row;
    pthread_mutex_t* lock;
} NormalizeArgs;

typedef struct {
    Vector* v;
    float max;
    float min;
    int start;
    int end;
    pthread_mutex_t* mutex;
}ThreadData;

void* normalize_vector_formula_1_thread(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    for (int i = data->start; i < data->end; i++) {
        pthread_mutex_lock(data->mutex);
        data->v->elements[i] = (data->v->elements[i] - data->min) / (data->max - data->min);
        pthread_mutex_unlock(data->mutex);
    }
    pthread_exit(NULL);
}

void* normalize_formula_1(void* args_ptr) {
    NormalizeArgs* args = (NormalizeArgs*) args_ptr;

    for (int i = args->start_row; i < args->end_row; ++i) {
        for (int j = 0; j < args->M->cols; ++j) {
            pthread_mutex_lock(args->lock);
            args->M->elements[i][j] = (args->M->elements[i][j] - args->menores->elements[j]) / (args->mayores->elements[j] - args->menores->elements[j]);
            pthread_mutex_unlock(args->lock);
        }
    }

    return NULL;
}

void main_normalize_formula_1(int rows, int cols, int n, int file){
    validate_data_operation_with_one_matrix(rows, cols, n);
    if(rows==1){
        main_normalize_vector_formula1(cols, n, file);
    }else{
        main_normalize_matrix_formula1(rows, cols, n, file);
    }
    
}

void main_normalize_vector_formula1(int cols,int n, int file){
    Vector* V=NULL;
    if(file==1){
       V=create_vector_from_file("opr1.txt",cols);
    }else{
        V=create_vector(cols);
        init_vector_rand(V);
    }
    print_vector(V);
    float max=max_vector(V);
    printf("El numero mayor es: %f \n", max);
    float min=min_vector(V);
    printf("El numero menor es: %f \n", min);
    Vector* V1= create_vector(cols);
    copy_vector(V1,V);
    normalize_vector_formula1_without_parallel_programming(V1, max, min);
    Vector* V2= create_vector(cols);
    copy_vector(V2,V);
    normalize_vector_formula1_with_parallel_programming(V2, max, min, n);       
    free_vector(V);
}

void main_normalize_matrix_formula1(int rows, int cols, int n, int file){
    struct Matrix* M=NULL;
    if( file==1){
        M =create_matrix_from_file("op1.txt",rows,cols);
    }else{
        //Se crea la matriz
        M = create_matrix(rows, cols);
        //Se inicializa la matriz con numeros aleatorios
        init_matrix_rand(M);
    }
    //Se obtienen los numeros mayores y menores de cada columna
    Vector* larger_numbers = matrix_col_max(M);
    printf("Vector de numeros mayores: \n");
    print_vector(larger_numbers);
    Vector* minors_numbers = matrix_col_min(M);
    printf("Vector de numeros menores: \n");
    print_vector(minors_numbers);
    //Se imprime la matriz
    print_matrix(M);   
    //Creamos otra matriz auxiliar para realizar las operaciones
    Matrix* M1 = create_matrix(rows, cols);
    //Copiamos la matriz original a la matriz que creamos
    copy_matrix(M1,M);
    //Llamamos a la funcion que realiza la normalizacion de la matriz por un escalar sin usar paralelismo
    normalize_matrix_formula1_without_parallel_programming(M1, larger_numbers, minors_numbers);
    Matrix* M2 = create_matrix(rows, cols);
    //Se copia la matriz original a la matriz auxiliar
    copy_matrix(M2,M);
    //Se llama a la funcion que realiza la normalicón de la matriz por un escalar usando paralelismo
    normalize_matrix_formula1_with_parallel_programming(M2,larger_numbers, minors_numbers, n);
    //Se libera la memoria de las matrices
    free_matrix(M);
}

void normalize_matrix_formula1_without_parallel_programming(Matrix* M, Vector* larger_numbers, Vector* minors_numbers){
    printf("\nNormalize matrix  formula 1 without Parallel programming\n");
    struct timeval start, end;
    gettimeofday(&start, 0);
    normalize_matrix_column_formula_1(M, larger_numbers, minors_numbers);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}

void normalize_vector_formula1_without_parallel_programming(Vector* v, float max, float min){
    printf("\nNormalize vector formula 1 without Parallel programming\n");
    struct timeval start, end;
    gettimeofday(&start, 0);
    normalize_vector_formula_1(v, max, min);
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_vector(v);
    free_vector(v);
}

void normalize_matrix_formula1_with_parallel_programming(Matrix* M, Vector* mayores, Vector* menores, int n) {
    printf("\nNormalize matrix formula 1 with Parallel programming\n");
    struct timeval start, end;
    const int num_threads = n;  // Número de hilos
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);    
    pthread_t threads[num_threads];
    NormalizeArgs args[num_threads];
    int chunk_size = M->rows / num_threads;
    int extra_rows = M->rows % num_threads;
    gettimeofday(&start, 0);
    for (int i = 0; i < num_threads; ++i) {
        args[i].M = M;
        args[i].mayores = mayores;
        args[i].menores = menores;
        args[i].lock = &lock;

        int start_row = i * chunk_size;
        int end_row = (i + 1) * chunk_size;

        if (i == num_threads - 1) {
            end_row += extra_rows;
        }

        args[i].start_row = start_row;
        args[i].end_row = end_row;

        pthread_create(&threads[i], NULL, normalize_formula_1, &args[i]);
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

void normalize_vector_formula1_with_parallel_programming(Vector* v, float max, float min, int num_threads) {
    ThreadData data[num_threads];
    pthread_t threads[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int elements_per_thread = v->size / num_threads;
    struct timeval start, end;
    gettimeofday(&start, 0);
    for (int i = 0; i < num_threads; i++) {
        data[i].v = v;
        data[i].max = max;
        data[i].min = min;
        data[i].mutex = &mutex;

        if (i == num_threads - 1) {
            data[i].start = i * elements_per_thread;
            data[i].end = v->size;
        } else {
            data[i].start = i * elements_per_thread;
            data[i].end = (i + 1) * elements_per_thread;
        }
        pthread_create(&threads[i], NULL, normalize_vector_formula_1_thread, (void*) &data[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_vector(v);
    free_vector(v);
}