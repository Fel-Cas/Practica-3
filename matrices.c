#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./matrix/matrix.c"
#include "./execution_time/execution_time.c"
#include "./params/parametros.c"
#include "./matrix_scalar/matrix_scalar.c"
#include "./normalize_formula1/normalize_formula1.c"
#include "./normalize_formula2/normalize_formula2.c"
#include "./file/file.c"
#include "./matrices_sum/matrices_sum.c"
#include "./matrices_dot/matrices_dot.c"
#include "./validations/validations.c"
#include "./column_mean/column_mean.c"
#include "./column_variance/column_variance.c"
#include "./utils/minorValue.c"
#include "./standard_deviation/standard_deviation.c"
#include "./min_max/min_max.c"


int select_operation(int o, int f, int c, int r, int s, double e, int n, int file);

int main(int argc, char *argv[])
{
    int o = 0, f = 0, c = 0, r = 0, s = 0, n = 0, file = 0;
    char p[200] = "";
    double e = 0;
    get_params(argc, argv, &o, &f, &c, &e, &r, &s, p, &n);
    if (strcmp(p, "") == 0)
    {
        select_operation(o, f, c, r, s, e, n, file);
    }
    else
    {
        file = 1;
        read_file(&o, &f, &c, &e, &r, &s, p, &n);
        select_operation(o, f, c, r, s, e, n, file);
    }
    return 0;
}
int select_operation(int o, int f, int c, int r, int s, double e, int n, int file)
{
    switch (o)
    {
    case 1:
        matrix_columns_mean(f, c, n, file);
        return 0;
    case 2:
        matrix_columns_variance(f, c, n, file);
        return 0;
    case 3:
        calculate_standard_deviation_by_column(f, c, n, file);
        return 0;
    case 4:
        calculate_min_max_by_columns(f, c, n, file);
        return 0;
    case 5:
        add_two_matrices(f, c, r, s, n, file);
        return 0;
    case 6:
        dot_two_matrices(f, c, r, s, n, file);
        return 0;
    case 7:
        multiply_matrix_by_scalar(f, c, e, n, file);
        return 0;
    case 8:
        main_normalize_formula_1(f, c, n, file);
        return 0;
    case 9:
        main_normalize_formula_2(f, c, n, file);
        return 0;
    default:
        printf("Operacion no valida %d ", o);
        exit(EXIT_FAILURE);
    }
}
