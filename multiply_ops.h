#pragma once
#define __MULTIPLY_OPS__

#include "matrix_ops.h"

/*
  Thread function for line multiplication.
 */
void* thread_line_multiply(void* multiply_input);

/*
  Multiplies matrices the fastest way possible with pthreads.
 */
matrix* multiply_matrix_pthreads(matrix* A, matrix* B);

/*
  Multiplies matrices the fastest way possible with openMP.
 */
matrix* multiply_matrix_openMP(matrix* A, matrix* B);

/*
  Multiplies matrices sequentially.
 */
matrix* sequential_multiply(matrix* A, matrix* B);

/*
  Checks if two matrices are multipliable.
 */
int matrix_check(matrix* A, matrix* B);
