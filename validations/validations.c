#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "validation.h"

void validate_data_operation_with_one_matrix(int rows, int columns, int n){
    if(rows <= 0 || columns <= 0){
        printf("The number of rows and columns must be greater than 0\n");
        exit(EXIT_FAILURE);
    }
    if(n <= 0){
        printf("Number of threads must be greater than 0\n");
        exit(EXIT_FAILURE);
    }
}