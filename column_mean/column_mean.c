#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "../execution_time/time.h"
#include <pthread.h>
#include "../matrix/matrix.h"


// Estructura de datos para pasar los argumentos a la funcion que realiza la suma de matrices usando paralelismo
typedef struct
{
    Vector *R;
    Matrix *M;
    int start_row;
    int end_row;
    pthread_mutex_t *mutex;
} MatricesMeanArgs;

void columns_mean(Matrix *M){
    printf("\nMean of matrices without paralellism\n");
    // Se obtiene el tiempo de inicio
    struct timeval start, end;
    gettimeofday(&start, 0);
    // Media de matrices sin paralelismo
    Vector *R = matrix_col_mean(M);
    // Se obtiene el tiempo de fin
    gettimeofday(&end, 0);
    // Se imprime el tiempo de ejecucion y el resultado de la suma
    get_execution_time(start, end);
    print_vector(R);
    free_vector(R);
}


void columns_mean_parallel(Matrix *M, int n){

}

void matrix_columns_mean(int m_rows, int m_cols, int n){
    // Se crea la matriz
    Matrix *M = create_matrix(m_rows, m_cols);
    // Se inicializa la matriz con numeros aleatorios
    init_matrix_rand(M);
    // Se imprime la matriz
    print_matrix(M);

    columns_mean(M);

    columns_mean_parallel(M, n);

    // Se libera la memoria de la matriz
    free_matrix(M);
};