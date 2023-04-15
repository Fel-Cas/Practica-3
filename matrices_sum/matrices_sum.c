#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "../execution_time/time.h"
#include "./matrices_sum.h"
#include <pthread.h>

void add_two_matrices(int m1_rows, int m1_cols, int m2_rows, int m2_cols, int n)
{
    // Se crea la matriz 1
    Matrix *M = create_matrix(m1_rows, m1_cols);
    // Se inicializa la matriz 1 con numeros aleatorios
    init_matrix_rand(M);
    // Se imprime la matriz 1
    print_matrix(M);

    // Se crea la matriz 2
    Matrix *N = create_matrix(m1_rows, m1_cols);
    // Se inicializa la matriz 2 con numeros aleatorios
    init_matrix_rand(N);
    // Se imprime la matriz 2
    print_matrix(N);

    Matrix* R=add_matrix(M,N);
    print_matrix(R);

    free_matrix(M);
}
