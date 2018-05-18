#include "multiply_ops.h"
#include "matrix_ops.h"
#include "test.h"
#include "utils.h"
#include <omp.h>
#include <stdio.h>

matrix* multiply_matrix(matrix* A, matrix* B) {
  // TODO
  return NULL;
}

matrix* sequential_multiply(matrix* A, matrix* B) {
  // O(n^3)
  int sum;
  if (!matrix_check(A, B)) {
    return NULL;
  }
  matrix* mat = emalloc(sizeof(matrix));
  mat->hei = A->hei;
  mat->len = B->len;
  mat->m = (float **) emalloc(sizeof(float *) * A->hei);
  for (int i = 0; i < A->hei; i++) {
    mat->m[i] = (float *) emalloc(sizeof(float) * B->len);
    for (int j = 0; j < B->len; j++) {
      sum = 0;
      for (int k = 0; k < A->len; k++) {
        sum += A->m[i][k] * B->m[k][j];
      }
      mat->m[i][j] = sum;
    }
  }
  return mat;
}

int matrix_check(matrix* A, matrix* B) {
  if (A == NULL || B == NULL) return 0;
  return A->len == B->hei;
}

#ifdef TEST_MULTIPLY_OPS
int main() {
  matrix* A;
  matrix* B;
  matrix* C;
  A = read_matrix_from_file("test_A.mat");
  B = read_matrix_from_file("test_B.mat");
  C = sequential_multiply(A, B);
  write_matrix_to_file(C, "test_C.mat");
  free_matrix(A);
  free_matrix(B);
  free_matrix(C);
  return 0;
}
#endif
