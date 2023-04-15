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

Vector* matrix_col_mean(const Matrix* M);
Vector* matrix_col_sum(const Matrix* M);
//Vector* matrix_col_max(const Matrix* M);
//Vector* matrix_col_min(const Matrix* M);
//Vector* matrix_col_vrz(const Matrix* M);
//Vector* matrix_col_std(const Matrix* M);
void scalar_matrix(Matrix* M, double k);
void scalar_vector(Vector* V, double k);

void print_vector(const Vector* v);
void print_matrix(const Matrix* M);

#endif
