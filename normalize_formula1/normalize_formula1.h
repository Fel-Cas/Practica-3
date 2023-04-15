#ifndef FORMULA1_H
#define FORMULA1_H
#include "../matrix/matrix.h"
void main_normalize_formula_1(int rows, int cols);
void normalize_matrix(Matrix* M, Vector* larger_numbers, Vector* minors_numbers);
void normalize_matrix_with_parallel_programming(Matrix* M, Vector* larger_numbers, Vector* minors_numbers);
#endif