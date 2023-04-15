#ifndef FORMULA2_H
#define FORMULA2_H
#include "../matrix/matrix.h"
void main_normalize_formula_2(int rows, int cols);
void execute_normalize_formula_2_normal(Matrix* M, Vector* vrz, Vector* std);
void execute_normalize_formula_2_parallel(Matrix* M, Vector* vrz, Vector* std);
#endif //FORMULA2_H