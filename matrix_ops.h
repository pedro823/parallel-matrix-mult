#pragma once
#define __READ_MATRIX__

typedef struct {
  int hei; // Height of the matrix: number of lines
  int len; // Length of the matrix: number of rows
  float **m; // Pointer to the matrix.
} matrix;

/*
  Reads matrix from a file.
 */
matrix* read_matrix_from_file(char* file_name);

/*
  Writes matrix to a file. Returns amount of bytes written.
 */
int write_matrix_to_file(matrix* mat, char* file_name);

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
