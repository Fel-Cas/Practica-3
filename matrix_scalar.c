#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "execution_time.c"
#include "matrix.c"

void multiply_matrix_by_scalar(int rows, int cols, double scalar);
void validate_data(int rows, int cols, double scalar);
void normal_multiply_matrix_by_scalar(Matrix* M,double scalar);
void parallel_multiply_matrix_by_scalar(Matrix* M,double scalar);


void multiply_matrix_by_scalar(int rows, int cols, double scalar){
    Matrix* M = create_matrix(rows, cols);
    init_matrix_rand(M);
    print_matrix(M);
    normal_multiply_matrix_by_scalar(M,scalar);
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