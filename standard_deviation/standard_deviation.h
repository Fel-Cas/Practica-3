#ifndef STANDARD_DEVIATION_COLUMNS
#define STANDARD_DEVIATION_COLUMNS
#include "../matrix/matrix.h"
double calcularMediaColumna(Matrix* matriz, int columna);
double calcularDesviacionEstandarColumna(Matrix* matriz, int columna);
void* calcularDesviacionEstandarColumnaHilo(void* arg);
int calculate_standard_deviation_by_column(int rows, int cols, int num_threads);
#endif