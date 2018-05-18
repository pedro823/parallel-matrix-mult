#pragma once
#define __MULTIPLY_OPS__

#include "matrix_ops.h"

/*
  Multiplies matrices the fastest way possible.
 */
matrix* multiply_matrix(matrix* A, matrix* B);

/*
  Multiplies matrices sequentially.
 */
matrix* sequential_multiply(matrix* A, matrix* B);

/*
  Checks if two matrices are multipliable.
 */
int matrix_check(matrix* A, matrix* B);
