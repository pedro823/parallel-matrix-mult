#pragma once
#define __READ_MATRIX__

#ifndef ulli
typedef unsigned long long int ulli;
#endif

typedef struct {
  ulli hei; // Height of the matrix: number of lines
  ulli len; // Length of the matrix: number of rows
  double **m; // Pointer to the matrix.
} matrix;

typedef struct {
    matrix *A; // Left Matrix
    matrix *B; // Right Matrix
    matrix *C; // Result Matrix
    ulli i; // Thread index
} thread_input;

/*
  Creates and fill input
 */
thread_input* create_input(matrix* A, matrix* B, matrix* C, ulli i);

/*
  Reads matrix from a file.
 */
matrix* read_matrix_from_file(char* file_name);

/*
  Writes matrix to a file. Returns amount of bytes written.
 */
ulli write_matrix_to_file(matrix* mat, char* file_name);

/*
  Transposes a matrix.
 */
void transpose_matrix(matrix* mat);

/*
  Frees a matrix
 */
void free_matrix(matrix* mat);

/*
  Prints a matrix for debug purposes.
 */
void print_matrix(matrix* m);

/*
  Checks if a matrix is aprox. equal to another
 */
int aprox_equal(matrix* A, matrix* B);
