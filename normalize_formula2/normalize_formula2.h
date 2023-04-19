#ifndef FORMULA2_H
#define FORMULA2_H
#include "../matrix/matrix.h"
void main_normalize_formula_2(int rows, int cols, int n,int file);
void main_normalize_matrix_formula2(int rows, int cols, int n, int file);
void normalize_matrix_formula2_without_parallel_programming(Matrix* M, Vector* vrz, Vector* std);
void normalize_matrix_formula2_with_parallel_programming(Matrix* M, Vector* vrz, Vector* std, int num_threads);
#endif //FORMULA2_H