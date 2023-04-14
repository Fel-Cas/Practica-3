#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void get_execution_time(struct timeval start, struct timeval end) {
    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
     printf("Tiempo de ejecución: %f microsegundos\n", elapsed_time);
}