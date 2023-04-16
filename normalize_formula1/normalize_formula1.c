#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "./normalize_formula1.h"
#include "../execution_time/time.h" 
#include "../validations/validation.h"
//Estructura para pasar los argumentos a la función que normaliza la matriz
typedef struct {
    Matrix* M;
    Vector* mayores;
    Vector* menores;
    int start_row;
    int end_row;
    pthread_mutex_t* lock;
} NormalizeArgs;

//Estructura para pasar los argumentos a la función que normaliza el vector
typedef struct {
    Vector* v;
    float max;
    float min;
    int start;
    int end;
    pthread_mutex_t* mutex;
}ThreadData;

//Función que normaliza el vector
void* normalize_vector_formula_1_thread(void* arg) {
    //Se Castean los argumentos
    ThreadData* data = (ThreadData*) arg;
    //Se itera sobre el rango asignado al hilo
    for (int i = data->start; i < data->end; i++) {
        //Se adquiere el lock para asegurar la exclusión mutua
        pthread_mutex_lock(data->mutex);
        //Se realiza el cálculo de la normalización
        data->v->elements[i] = (data->v->elements[i] - data->min) / (data->max - data->min);
        //Se libera el lock
        pthread_mutex_unlock(data->mutex);
    }
    //Se termina el hilo
    pthread_exit(NULL);
}

//Función principal que normaliza la matriz
void* normalize_formula_1(void* args_ptr) {
    //Se castean los argumentos
    NormalizeArgs* args = (NormalizeArgs*) args_ptr;
    //Se itera sobre el rango asignado al hilo
    for (int i = args->start_row; i < args->end_row; ++i) {
        for (int j = 0; j < args->M->cols; ++j) {
            //Se adquiere el lock para asegurar la exclusión mutua
            pthread_mutex_lock(args->lock);
            //Se realiza el cálculo de la normalización
            args->M->elements[i][j] = (args->M->elements[i][j] - args->menores->elements[j]) / (args->mayores->elements[j] - args->menores->elements[j]);
            //Se libera el lock
            pthread_mutex_unlock(args->lock);
        }
    }

    return NULL;
}

//Función principal que recibe los datos de entrada y llama a las funciones que realizan la normalización
void main_normalize_formula_1(int rows, int cols, int n, int file){
    //Se valida que los datos de entrada sean correctos
    validate_data_operation_with_one_matrix(rows, cols, n);
    //Se valida si el numero de filas es iagual a 1 o no
    if(rows==1){
        //Se llama a la función que normaliza el vector
        main_normalize_vector_formula1(cols, n, file);
    }else{
        //Se llama a la función que normaliza la matriz
        main_normalize_matrix_formula1(rows, cols, n, file);
    }
    
}
//Función que normaliza el vector
void main_normalize_vector_formula1(int cols,int n, int file){
    Vector* V=NULL;
    //Se valida si se va a leer el vector desde un archivo o no
    if(file==1){
       V=create_vector_from_file("opr1.txt",cols);
    }else{
        //Se crea el vector
        V=create_vector(cols);
        //Se inicializa el vector con numeros aleatorios
        init_vector_rand(V);
    }
    //Se imprime el vector
    print_vector(V);
    //Se obtiene el numero mayor y menor del vector
    float max=max_vector(V);
    printf("El numero mayor es: %f \n", max);
    float min=min_vector(V);
    printf("El numero menor es: %f \n", min);
    //Se crea un vector auxiliar para realizar la normalización
    Vector* V1= create_vector(cols);
    //Se copia el vector original al vector auxiliar
    copy_vector(V1,V);
    //Se llama a la función que realiza la normalización sin usar paralelismo
    normalize_vector_formula1_without_parallel_programming(V1, max, min);
    //Se crea un vector auxiliar para realizar la normalización
    Vector* V2= create_vector(cols);
    //Se copia el vector original al vector auxiliar
    copy_vector(V2,V);
    //Se llama a la función que realiza la normalización usando paralelismo
    normalize_vector_formula1_with_parallel_programming(V2, max, min, n);  
    //Se libera la memoria   
    free_vector(V);
}

//Función que llama las funciones que realizan la normalización de la matriz
void main_normalize_matrix_formula1(int rows, int cols, int n, int file){

    struct Matrix* M=NULL;
    //Se valida si se va a leer la matriz desde un archivo o no
    if( file==1){
        M =create_matrix_from_file("op1.txt",rows,cols);
    }else{
        //Se crea la matriz
        M = create_matrix(rows, cols);
        //Se inicializa la matriz con numeros aleatorios
        init_matrix_rand(M);
    }
    //Se obtienen los numeros mayores y menores de cada columna
    Vector* larger_numbers = matrix_col_max(M);
    printf("Vector de numeros mayores: \n");
    print_vector(larger_numbers);
    Vector* minors_numbers = matrix_col_min(M);
    printf("Vector de numeros menores: \n");
    print_vector(minors_numbers);
    //Se imprime la matriz
    print_matrix(M);   
    //Creamos otra matriz auxiliar para realizar las operaciones
    Matrix* M1 = create_matrix(rows, cols);
    //Copiamos la matriz original a la matriz que creamos
    copy_matrix(M1,M);
    //Llamamos a la funcion que realiza la normalizacion de la matriz por un escalar sin usar paralelismo
    normalize_matrix_formula1_without_parallel_programming(M1, larger_numbers, minors_numbers);
    Matrix* M2 = create_matrix(rows, cols);
    //Se copia la matriz original a la matriz auxiliar
    copy_matrix(M2,M);
    //Se llama a la funcion que realiza la normalicón de la matriz por un escalar usando paralelismo
    normalize_matrix_formula1_with_parallel_programming(M2,larger_numbers, minors_numbers, n);
    //Se libera la memoria de las matrices
    free_matrix(M);
}

//Función que realiza la normalización de la matriz por un escalar sin usar paralelismo
void normalize_matrix_formula1_without_parallel_programming(Matrix* M, Vector* larger_numbers, Vector* minors_numbers){
    //Se imprime el mensaje de inicio de la normalización
    printf("\nNormalize matrix  formula 1 without Parallel programming\n");
    //Se obtiene el tiempo de inicio
    struct timeval start, end;
    gettimeofday(&start, 0);
    //Se realiza la normalización
    normalize_matrix_column_formula_1(M, larger_numbers, minors_numbers);
    //Se obtiene el tiempo de finalización
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    //Se imprimen los resultados
    print_matrix(M);
    free_matrix(M);
}

//Función que realiza la normalización de un vector sin usar paralelismo
void normalize_vector_formula1_without_parallel_programming(Vector* v, float max, float min){
    printf("\nNormalize vector formula 1 without Parallel programming\n");
    //Se obtiene el tiempo de inicio
    struct timeval start, end;
    gettimeofday(&start, 0);
    //Se realiza la normalización
    normalize_vector_formula_1(v, max, min);
    //Se obtiene el tiempo de finalización
    gettimeofday(&end, 0);
    //Se imprimen los resultados
    get_execution_time(start, end);
    print_vector(v);
    free_vector(v);
}

//Función que realiza la normalización de una matriz usando paralelismo
void normalize_matrix_formula1_with_parallel_programming(Matrix* M, Vector* mayores, Vector* menores, int n) {
    printf("\nNormalize matrix formula 1 with Parallel programming\n");    
    struct timeval start, end;
    const int num_threads = n;  // Número de hilos
    pthread_mutex_t lock;
    //Se inicializan los locks
    pthread_mutex_init(&lock, NULL);    
    //Se crean los hilos
    pthread_t threads[num_threads];
    //Se crean los argumentos
    NormalizeArgs args[num_threads];
    //Se calcula la cantidad de filas que va a tener cada hilo
    int chunk_size = M->rows / num_threads;
    int extra_rows = M->rows % num_threads;
    //Se obtiene el tiempo de inicio
    gettimeofday(&start, 0);
    //Se ponen los argumentos en cada hilo
    for (int i = 0; i < num_threads; ++i) {
        args[i].M = M;
        args[i].mayores = mayores;
        args[i].menores = menores;
        args[i].lock = &lock;
        //Se calcula el rango de filas que va a tener cada hilo
        int start_row = i * chunk_size;
        int end_row = (i + 1) * chunk_size;
        if (i == num_threads - 1) {
            end_row += extra_rows;
        }
        args[i].start_row = start_row;
        args[i].end_row = end_row;
        //Se llama a la función que realiza la normalización por cada hilo
        pthread_create(&threads[i], NULL, normalize_formula_1, &args[i]);
    }
    //Se espera a que terminen los hilos
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    //Se destruyen los locks
    pthread_mutex_destroy(&lock);
    gettimeofday(&end, 0);

    //Se imprime el tiempo de ejecucion y el resultado de la multiplicación
    get_execution_time(start, end);
    //Se imprime la matriz
    print_matrix(M);
    //Se libera la memoria de la matriz
    free_matrix(M);
}
//Función que realiza la normalización de un vector usando paralelismo
void normalize_vector_formula1_with_parallel_programming(Vector* v, float max, float min, int num_threads) {
    printf("\nNormalize vector formula 1 with Parallel programming\n");
    //Se crean los argumentos
    ThreadData data[num_threads];
    pthread_t threads[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    //Se calcula la cantidad de elementos que va a tener cada hilo
    int elements_per_thread = v->size / num_threads;
    //Se obtiene el tiempo de inicio
    struct timeval start, end;
    gettimeofday(&start, 0);
    //Se ponen los argumentos en cada hilo
    for (int i = 0; i < num_threads; i++) {
        data[i].v = v;
        data[i].max = max;
        data[i].min = min;
        data[i].mutex = &mutex;
        //Se calcula el rango de elementos que va a tener cada hilo
        if (i == num_threads - 1) {
            data[i].start = i * elements_per_thread;
            data[i].end = v->size;
        } else {
            data[i].start = i * elements_per_thread;
            data[i].end = (i + 1) * elements_per_thread;
        }
        //Se llama a la función que realiza la normalización por cada hilo
        pthread_create(&threads[i], NULL, normalize_vector_formula_1_thread, (void*) &data[i]);
    }
    //Se espera a que terminen los hilos
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
     pthread_mutex_destroy(&mutex);
    //Se imprimen los resultados
    gettimeofday(&end, 0);
    get_execution_time(start, end);
    print_vector(v);
    free_vector(v);
}