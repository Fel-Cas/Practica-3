#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parametros.c"
#include "matrix_scalar.c"


int main(int argc, char *argv[]){
    int o=0,f=0,c=0,r=0,s=0,p=0;
    double e=0;
    get_params(argc,argv,&o,&f,&c,&e,&r,&s,&p);
    if(p==0){
        switch(o){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                 multiply_matrix_by_scalar(f,c,e);
                break;
            case 8:
                break;
            case 9:
                break;
            default:
                printf("Operacion no valida %d ",o);
                exit(EXIT_FAILURE);
            }
        }
    return 0;
}

