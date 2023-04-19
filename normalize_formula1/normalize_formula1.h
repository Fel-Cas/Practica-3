#ifndef FORMULA1_H
#define FORMULA1_H
#include "../matrix/matrix.h"
void main_normalize_formula_1(int rows, int cols, int n, int file);
void main_normalize_matrix_formula1(int rows, int cols, int n, int file);
void normalize_matrix_formula1_without_parallel_programming(Matrix* M, Vector* larger_numbers, Vector* minors_numbers);
void normalize_matrix_formula1_with_parallel_programming(Matrix* M, Vector* larger_numbers, Vector* minors_numbers, int n);
#endif