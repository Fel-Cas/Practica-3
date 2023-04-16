#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "validation.h"
//función que valida que los datos de entrada sean correctos
void validate_data_operation_with_one_matrix(int rows, int columns, int n){
    //validar que las filas y columnas sean mayores a 0
    if(rows <= 0 || columns <= 0){
        printf("The number of rows and columns must be greater than 0\n");
        exit(EXIT_FAILURE);
    }
    //validar que el número de hilos sea mayor a 0
    if(n <= 0){
        printf("Number of threads must be greater than 0\n");
        exit(EXIT_FAILURE);
    }
}