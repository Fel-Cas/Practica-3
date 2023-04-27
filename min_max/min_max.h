#ifndef MIN_MAX_COLUMNS
#define MIN_MAX_COLUMNS
#include "../matrix/matrix.h"
void* min_max_cols_thread(void* arg);
void min_max_cols_range(Matrix* matrix, void* min_max, int start_col, int end_col);
void min_max_cols_parallel(Matrix* matrix, double* min_values, double* max_values, int num_threads);
void min_max_cols(Matrix* matrix, double* min_values, double* max_values);
int calculate_min_max_by_columns(int rows, int cols, int num_threads);
#endif