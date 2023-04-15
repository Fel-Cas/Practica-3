#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./matrix/matrix.c"
#include "./execution_time/execution_time.c"
#include "./params/parametros.c"
#include "./matrix_scalar/matrix_scalar.c"
#include "./normalize_formula1/normalize_formula1.c"
#include "./normalize_formula2/normalize_formula2.c"
#include "./matrices_sum/matrices_sum.c"
#include "./matrices_dot/matrices_dot.c"



int main(int argc, char *argv[]){
    int o=0,f=0,c=0,r=0,s=0,n=0;
    char p = NULL;
    double e=0;
    get_params(argc,argv,&o,&f,&c,&e,&r,&s,&p,&n);
    if(p==NULL){
        switch(o){
            case 1:
                return 0;
            case 2:
                return 0;
            case 3:
                return 0;
            case 4:
                return 0;
            case 5:
                add_two_matrices(f,c,r,s,n);
                return 0;
            case 6:
                dot_two_matrices(f,c,r,s,n);
                printf("hola");
                return 0;
            case 7:
                 multiply_matrix_by_scalar(f,c,e,n);
               return 0;
            case 8:
                  main_normalize_formula_1(f,c,n);  
                return 0;
            case 9:
                  main_normalize_formula_2(f,c,n);
                return 0;
            default:
                printf("Operacion no valida %d ",o);
                exit(EXIT_FAILURE);
            }
        }
        printf("p = %s\n", p);
    return 0;
}

