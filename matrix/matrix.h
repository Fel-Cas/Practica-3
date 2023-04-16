#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

typedef struct Vector Vector;
struct Vector {
    int size;
    double* elements;
};

typedef struct Matrix Matrix;
struct Matrix {
    int rows;
    int cols;
    double** elements;
};

Vector* create_vector(int size);
Vector* create_vector_from_file(const char* file_path, int size);
Matrix* create_matrix(int rows, int cols);
Matrix* create_matrix_from_file(const char* file_path, int rows, int cols);

void init_matrix_rand(Matrix* M);
void init_vector_rand(Vector* V);
void copy_matrix(Matrix* dst, const Matrix* src);
void copy_vector(Vector* dst, const Vector* src);

void free_vector(Vector* v);
void free_matrix(Matrix* M);

Vector* add_vector(const Vector* a, const Vector* b);
Vector* dot_vector_matrix(const Vector* v, const Matrix* M);
Matrix* add_matrix(const Matrix* M, const Matrix* N);
Matrix* dot_matrix(const Matrix* M, const Matrix* N);
float max_vector(const Vector* v);
float min_vector(const Vector* v);
float vrz_vector(const Vector* v);
float std_vector(const Vector* v);
Vector* matrix_col_mean(const Matrix* M);
Vector* matrix_col_sum(const Matrix* M);
Vector* matrix_col_max(Matrix* M);
Vector* matrix_col_min(Matrix* M);
Vector* matrix_col_vrz(Matrix* M);
Vector* matrix_col_std(Matrix* M);
void scalar_matrix(Matrix* M, double k);
void scalar_vector(Vector* V, double k);

void print_vector(const Vector* v);
void print_matrix(const Matrix* M);
void normalize_matrix_column_formula_1(Matrix* M, Vector* mayores, Vector* menores);
void normalize_matrix_column_formula_2(Matrix* M, Vector* vrz, Vector* std);
void normalize_vector_formula_1(Vector* v, float max, float min);
void normalize_vector_formula_2(Vector* v, float vrz, float std);
#endif
