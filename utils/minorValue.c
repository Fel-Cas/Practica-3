#include "./minorValue.h"

int minor_value(int n_threads, int n_rows){
    if(n_threads>n_rows){
        return n_rows;
    }else{
        return n_threads;
    }
}