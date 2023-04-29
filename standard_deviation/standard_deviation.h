#ifndef STANDARD_DEVIATION_COLUMNS
#define STANDARD_DEVIATION_COLUMNS
#include "../matrix/matrix.h"
void calcular_desviacion_estandar_sin_paralelismo(Matrix* matriz);
void* calcularDesviacionEstandarColumnaHilo(void* dato);
void calcularDesviacionEstandarParalelo(Matrix *matriz, int num_threads);
void calculate_standard_deviation_by_column(int rows, int cols, int num_threads, int file);
#endif