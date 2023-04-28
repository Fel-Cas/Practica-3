# Practica 3 Sistemas Operativos

## Descripción
Se realizarán operaciones con matrices, utilizando paralelismo y concurrencia, y comparando el tiempo de ejecución de las operaciones con y sin paralelismo.

## ¿Cómo ejecutar?
Para ejecutar el programa, se debe ejecutar el siguiente comando: 
gcc -Wall -o matrices matrices.c -lm

## ¿Cómo funciona?
El programa recibe como parámetros el número de filas y columnas de las matrices, y el número de hilos a utilizar a traves de las flags.

- -o: Operación a realizar. Las operaciones disponibles son:
    - 1: Calcular la media de cada columna de una matriz
    - 2: Calcular la varianza de cada columna de una matriz
    - 3: Calcular la desviacion estandar de cada columna de una matriz
    - 4: Calcular el valor mínimo y el valor máximo de cada columna de una matriz.
    - 5: Sumar dos matrices.
    - 6: Realizar el producto punto de dos matrices.
    - 7: Multiplicar un escalar por una matriz
    - 8: Normalizar una matriz columna por columna de acuerdo al metodo 1.
    - 9: Normalizar una matriz columna por columna de acuerdo al metodo 2.
- -f: Número de filas de la primera matriz.
- -c: Número de columnas de la segunda matriz.
- -r: Número de filas de la segunda matriz.
- -s: Número de columnas de la segunda matriz.
- -e: Escalar a multiplicar por la matriz.
- -p: el path del archivo con los operandos de la operación a realizar (Si se indica esta opción, no es necesario indicar los tamaños de los operandos y estos deben deducirse del archivo). Si no se asigna este valor, las matrices se generan aleatoriamente.
- -n: Número de hilos a utilizar.

## Ejemplos
- Calcular la media de cada columna de una matriz de 3x3 con 2 hilos:
```
./matrices -o 1 -f 3 -c 3 -n 2
```
- Calcular la varianza de cada columna de una matriz de 3x3 con 2 hilos:
```
./matrices -o 2 -f 3 -c 3 -n 2
```
- Calcular la desviacion estandar de cada columna de una matriz de 3x3 con 2 hilos:
```
./matrices -o 3 -f 3 -c 3 -n 2
```
- Calcular el valor mínimo y el valor máximo de cada columna de una matriz de 3x3 con 2 hilos:
```
./matrices -o 4 -f 3 -c 3 -n 2
```
- Sumar dos matrices de 3x3 con 2 hilos:
```
./matrices -o 5 -f 3 -c 3 -r 3 -s 3 -n 2
```
- Realizar el producto punto de dos matrices de 3x3 con 2 hilos:
```
./matrices -o 6 -f 3 -c 3 -r 3 -s 3 -n 2
```
- Multiplicar un escalar con valor 3, por una matriz de 3x3 con 2 hilos:
```
./matrices -o 7 -f 3 -c 3 -e 3 -n 2
```
- Normalizar una matriz columna por columna de acuerdo al metodo 1, de una matriz de 3x3 con 2 hilos:
```
./matrices -o 8 -f 3 -c 3 -n 2
```
- Normalizar una matriz columna por columna de acuerdo al metodo 2, de una matriz de 3x3 con 2 hilos:
```
./matrices -o 9 -f 3 -c 3 -n 2
```
- Leer las flags y las matrices desde el archivo "file.txt":
```
./matrices -p file.txt
```