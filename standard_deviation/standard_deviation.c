#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include "./standard_deviation.h"
#include "../execution_time/time.h"

// Estructura para los argumentos del hilo
typedef struct {
    int start;
    int end;
    int column;
    double mean;
    Matrix* matriz;
    double result;
    pthread_mutex_t* mutex;
} ThreadArgs;

// Función para calcular la media de una columna de la matriz
double calcularMediaColumna(Matrix* matriz, int columna) {
    double suma = 0.0;
    for (int i = 0; i < matriz->rows; i++) {
        suma += matriz->elements[i][columna];
    }
    return suma / matriz->rows;
    free_matrix(matriz);
}

// Función para calcular la desviación estándar de una columna de la matriz
double calcularDesviacionEstandarColumna(Matrix* matriz, int columna) {
    double media = calcularMediaColumna(matriz, columna);
    double sumaCuadrados = 0.0;
    for (int i = 0; i < matriz->rows; i++) {
        double diferencia = matriz->elements[i][columna] - media;
        sumaCuadrados += diferencia * diferencia;
    }
    return sqrt(sumaCuadrados / matriz->rows);
    free_matrix(matriz);
}

// Función de hilo para calcular la desviación estándar de una columna de una matriz
void* calcularDesviacionEstandarColumnaHilo(void* dato) {
    ThreadArgs* datos = (ThreadArgs*) dato;

    // Bloquear el mutex antes de actualizar la variable que se comparte
    pthread_mutex_lock(datos->mutex);

    datos->result = calcularDesviacionEstandarColumna(datos->matriz, datos->column);

    // Desbloquear el mutex después de actualizar la variable que se comparte
    pthread_mutex_unlock(datos->mutex);
    pthread_exit(NULL);
}

int calculate_standard_deviation_by_column(int rows, int cols, int num_threads) {
    // Crear y llenar la matriz
    Matrix* matriz = create_matrix(rows, cols);
    //Se inicializa la matriz con numeros aleatorios
    init_matrix_rand(matriz);
    //Se imprime la matriz
    print_matrix(matriz);

    Matrix* matriz2 = create_matrix(rows, cols);
    copy_matrix(matriz2, matriz);

    // Obtener el tiempo de inicio
    struct timeval start_time;
    gettimeofday(&start_time, 0);

    // Calcular la desviación estándar sin paralelismo
    for (int j = 0; j < matriz2->cols; j++) {
        double resultado = calcularDesviacionEstandarColumna(matriz2, j);
        printf("Desviación estándar de la columna %d (sin paralelismo): %.2f\n", j, resultado);
    }

    // Obtener el tiempo de fin
    struct timeval end_time;
    gettimeofday(&end_time, 0);

    // Calcular el tiempo de ejecución sin paralelismo
    printf("Sin paralelismo: \n");
    get_execution_time(start_time, end_time);

    Matrix* matriz3 = create_matrix(rows, cols);
    copy_matrix(matriz3, matriz);

    // Obtener el tiempo de inicio
    gettimeofday(&start_time, 0);

    // Calcular la desviación estándar usando pthread
    pthread_t threads[num_threads];
    ThreadArgs data[num_threads];
    int elementsByThread = cols / num_threads;
    // Se inicializa el lock
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // Crear hilos y calcular la desviación estándar en paralelo
    for (int j = 0; j < num_threads; j++) {
        data[j].start = j * elementsByThread;
        
        if(j == num_threads - 1){
            data[j].end = cols;
        }else{
            data[j].end = data[j].start + elementsByThread;
        }
        
        data[j].column = j;
        data[j].matriz = matriz3;
        pthread_create(&threads[j], NULL, calcularDesviacionEstandarColumnaHilo, &data[j]);
    }

    // Esperar a que todos los hilos terminen
    for (int j = 0; j < num_threads; j++) {
        pthread_join(threads[j], NULL);
        printf("Desviación estándar de la columna %d: %.2f\n", j+1, data[j].result);
    }
    // Se destruye el lock
    pthread_mutex_destroy(&mutex);
    // Obtener el tiempo de fin
    gettimeofday(&end_time, 0);

    // Calcular el tiempo de ejecución con pthread
    printf("Con pthread \n");
    get_execution_time(start_time, end_time);

    free_matrix(matriz);

    return 0;
}