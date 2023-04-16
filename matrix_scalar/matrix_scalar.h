#ifndef MATRIX_SCALAR_H
#define MATRIX_SCALAR_H
#include "../matrix/matrix.h"
void multiply_matrix_by_scalar(int rows, int cols, double scalar, int n, int file);
void normal_multiply_matrix_by_scalar(Matrix* M,double scalar);
void parallel_multiply_matrix_by_scalar(Matrix* M,double scalar, int n);
#endif