#ifndef MIN_MAX_COLUMNS
#define MIN_MAX_COLUMNS
#include "../matrix/matrix.h"
void* min_max_cols_thread(void* arg);
void* min_cols_thread(void* arg);
void* max_cols_thread(void* arg);
void min_max_cols_parallel(Matrix* matriz, int num_threads);
void min_max_cols_without_parallelism(Matrix* matrix);
int calculate_min_max_by_columns(int rows, int cols, int num_threads);
#endif