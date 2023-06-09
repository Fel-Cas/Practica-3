#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "../execution_time/time.h"
#include "./matrix_scalar.h"
#include "../validations/validation.h"
#include "../utils/minorValue.h"
#include <pthread.h>


//Estructura de datos para pasar los argumentos a la funcion que realiza la multiplicacion de la matriz por un escalar usando paralelismo
typedef struct {
    Matrix* M;
    double k;
    int start_row;
    int end_row;
    pthread_mutex_t* mutex;
} ScalarMatrixArgs;

//Funcion que realiza la multiplicacion de la matriz por un escalar usando paralelismo
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

void multiply_matrix_by_scalar(int rows, int cols, double scalar, int n,int file){
    validate_data_operation_with_one_matrix(rows, cols, n);
    Matrix* M=NULL;
    //se valida si se va a leer la matriz de un archivo
    if(file==1){
        M=create_matrix_from_file("op1.txt",rows,cols);
    }else{
        //Se crea la matriz
        M = create_matrix(rows, cols);
        //Se inicializa la matriz con numeros aleatorios
        init_matrix_rand(M);
    }
    printf("La matrix es la siguiente: \n");
    //Se imprime la matriz
    print_matrix(M);
    printf("El escalar es: %f \n",scalar);
    //Creamos otra matriz auxiliar para realizar las operaciones
    Matrix* M1 = create_matrix(rows, cols);
    //Copiamos la matriz original a la matriz que creamos
    copy_matrix(M1,M);
    //Llamamos a la funcion que realiza la multiplicacion de la matriz por un escalar sin usar paralelismo
    normal_multiply_matrix_by_scalar(M1,scalar);
    //Se crea otra matriz auxiliar
    Matrix* M2 = create_matrix(rows, cols);
    //Se copia la matriz original a la matriz auxiliar
    copy_matrix(M2,M);
    //Se llama a la funcion que realiza la multiplicacion de la matriz por un escalar usando paralelismo
    parallel_multiply_matrix_by_scalar(M2,scalar,n);
    //Se libera la memoria de las matrices
    free_matrix(M);
    
}

//Funcion para realizar la multiplicacion de la matriz por un escalar sin usar paralelismo
void normal_multiply_matrix_by_scalar(Matrix* M,double scalar){
    printf("\nNormal multiply matrix by scalar\n");
    //Se obtiene el tiempo de inicio
    struct timeval start, end;
    gettimeofday(&start, 0);
    //Se realiza la multiplicacion de la matriz por un escalar
    scalar_matrix(M,scalar);
    //Se obtiene el tiempo de finalizacion
    gettimeofday(&end, 0);
    //Se imprime el tiempo de ejecucion y el resultado de la multiplicación
    get_execution_time(start, end);
    print_matrix(M);
    free_matrix(M);
}

//Funcion para realizar la multiplicacion de la matriz por un escalar usando paralelismo
void parallel_multiply_matrix_by_scalar(Matrix* M,double scalar, int n){

    printf("\nParallel multiply matrix by scalar\n");
    struct timeval start, end;
    const int num_threads =minor_value(n,M->rows);  // Número de hilos
    //Se crea un arreglo de hilos
    pthread_t threads[num_threads];
    //se inicializa la estructura de datos con el número de hilos a utilizar
    ScalarMatrixArgs thread_args[num_threads];
    //se inicializa el lock
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

    //Se obtiene el tiempo de inicio
    gettimeofday(&start, 0);
    //Se divide la matriz en partes iguales para cada hilo
    const int chunk_size = M->rows / num_threads;
    //Se le asigna a cada hilo una parte de la matriz
    int start_row = 0;
    for (int i = 0; i < num_threads; ++i) {
        int end_row = start_row + chunk_size;
        if (i == num_threads - 1) {
            end_row = M->rows;
        }
        //Se inicializan los datos necesarios para cada hilo
        thread_args[i] = (ScalarMatrixArgs) { .M = M, .k = scalar, .start_row = start_row, .end_row = end_row, .mutex = &mutex };
        pthread_create(&threads[i], NULL, scalar_matrix_parallel, &thread_args[i]);
        start_row = end_row;
    }

    // Esperamos a que todos los hilos terminen
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex); // Destruimos el mutex
    //Se obtiene el tiempo de finalizacion
    gettimeofday(&end, 0);
    //Se imprime el tiempo de ejecucion y el resultado de la multiplicación
    get_execution_time(start, end);
    //Se imprime la matriz
    print_matrix(M);
    //Se libera la memoria de la matriz
    free_matrix(M);
}