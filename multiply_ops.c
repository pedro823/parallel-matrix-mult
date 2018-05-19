#include "multiply_ops.h"
#include "matrix_ops.h"
#include "test.h"
#include "utils.h"
#include <omp.h>
#include <stdio.h>

matrix* multiply_matrix(matrix* A, matrix* B) {
  // TODO
  int sum;
  if (!matrix_check(A, B)) {
    return NULL;
  }
  matrix* mat = emalloc(sizeof(matrix));
  mat->hei = A->hei;
  mat->len = B->len;
  mat->m = (double **) emalloc(sizeof(double *) * A->hei);
  // Transposes matrix B to remove most cache misses
  transpose_matrix(B);
  #pragma omp parallel for private(sum)
  for (int i = 0; i < A->hei; i++) {
    mat->m[i] = (double *) emalloc(sizeof(double) * B->hei);
    #pragma omp parallel for private(sum)
    for (int j = 0; j < B->hei; j++) {
      sum = 0;
      for (int k = 0; k < A->len; k++) {
        sum += A->m[i][k] * B->m[j][k];
      }
      mat->m[i][j] = sum;
    }
  }
  return mat;
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
  mat->m = (double **) emalloc(sizeof(double *) * A->hei);
  for (int i = 0; i < A->hei; i++) {
    mat->m[i] = (double *) emalloc(sizeof(double) * B->len);
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

matrix* sequential_multiply_with_transpose(matrix* A, matrix* B) {
  int sum;
  if (!matrix_check(A, B)) {
    return NULL;
  }
  matrix* mat = emalloc(sizeof(matrix));
  mat->hei = A->hei;
  mat->len = B->len;
  mat->m = (double **) emalloc(sizeof(double *) * A->hei);
  // Transposes matrix B to remove cache misses
  transpose_matrix(B);
  for (int i = 0; i < A->hei; i++) {
    mat->m[i] = (double *) emalloc(sizeof(double) * B->hei);
    for (int j = 0; j < B->hei; j++) {
      sum = 0;
      for (int k = 0; k < A->len; k++) {
        sum += A->m[i][k] * B->m[j][k];
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
#include <time.h>
int main() {
  matrix *A, *B, *C, *D;
  A = read_matrix_from_file("test_A.mat");
  B = read_matrix_from_file("test_B.mat");

  double sequential_time_s = omp_get_wtime();
  C = sequential_multiply(A, B);
  double sequential_time_e = omp_get_wtime();
  double sequential_time = sequential_time_e - sequential_time_s;
  printf("Time elapsed in sequential: %fs\n", sequential_time);

  double transpose_time_s = omp_get_wtime();
  D = sequential_multiply_with_transpose(A, B);
  double transpose_time_e = omp_get_wtime();
  double transpose_time = transpose_time_e - transpose_time_s;
  printf("Time elapsed with transposition: %fs\n", transpose_time);
  free_matrix(D);
  transpose_matrix(B);

  double distr_time_s = omp_get_wtime();
  D = multiply_matrix(A, B);
  double distr_time_e = omp_get_wtime();
  float distr_time = distr_time_e - distr_time_s;
  printf("%d\n", aprox_equal(C, D));
  printf("Time elapsed in distributed: %fs\n", distr_time);

  // write_matrix_to_file(C, "test_C.mat");

  free_matrix(A);
  free_matrix(B);
  free_matrix(C);
  free_matrix(D);
  return 0;
}
#endif
