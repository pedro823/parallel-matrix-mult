#include <stdio.h>
#include <math.h>
#include "matrix_ops.h"
#include "utils.h"
#include "test.h"

matrix* read_matrix_from_file(char* file_name) {
  int h, l;
  FILE* f = fopen(file_name, "r");
  if (f == NULL) {
    fprintf(stderr, "File could not be opened for reading: %s\n", file_name);
    sigtrap();
  }
  fscanf(f, "%d %d", &h, &l);
  matrix* mat = emalloc(sizeof(matrix));
  mat->hei = h;
  mat->len = l;
  mat->m = (double **) emalloc(sizeof(double *) * h);
  for (int i = 0; i < h; i++) {
    mat->m[i] = (double *) emalloc(sizeof(double) * l);
    for (int j = 0; j < l; j++) {
      fscanf(f, "%lf", &mat->m[i][j]);
    }
  }
  fclose(f);
  return mat;
}

int write_matrix_to_file(matrix* mat, char* file_name) {
  if (mat == NULL) {
    fprintf(stderr, "write_matrix_to_file: mat is NULL\n");
    sigtrap();
  }
  FILE* f = fopen(file_name, "w");
  if (f == NULL) {
    fprintf(stderr, "File could not be opened for writing: %s\n", file_name);
    sigtrap();
  }
  int bytes_written = 0;
  bytes_written += fprintf(f, "%d %d\n", mat->hei, mat->len);
  for (int i = 0; i < mat->hei; i++) {
    for (int j = 0; j < mat->len; j++) {
      bytes_written += fprintf(f, "%f", mat->m[i][j]);
      if (j != mat->len - 1) {
        bytes_written += fputc(' ', f);
      }
    }
    bytes_written += fputc('\n', f);
  }
  return bytes_written;
}

void transpose_matrix(matrix* mat) {
  if (mat == NULL) return;
  int m = mat->hei, n = mat->len;
  double** new_m = (double **) emalloc(sizeof(double *) * mat->len);
  for (int j = 0; j < n; j++) {
    new_m[j] = (double *) emalloc(sizeof(double) * mat->hei);
    for (int i = 0; i < m; i++) {
      new_m[j][i] = mat->m[i][j];
    }
  }
  for (int i = 0; i < m; i++) {
    efree(mat->m[i]);
  }
  efree(mat->m);
  mat->m = new_m;
  int temp_dim = mat->hei;
  mat->hei = mat->len;
  mat->len = temp_dim;
}

void free_matrix(matrix* mat) {
  for (int i = 0; i < mat->hei; i++) {
    efree(mat->m[i]);
  }
  efree(mat->m);
  efree(mat);
}

void print_matrix(matrix* mat) {
  printf("%d %d\n", mat->hei, mat->len);
  for (int i = 0; i < mat->hei; i++) {
    for (int j = 0; j < mat->len; j++) {
      printf("%f ", mat->m[i][j]);
    }
    printf("\n");
  }
}

int aprox_equal(matrix* A, matrix* B) {
  if (A == NULL && B == NULL) return 1;
  if (A == NULL || B == NULL) return 0;
  if (A->hei != B->hei || A->len != B->len) return 0;
  double eq;
  for (int i = 0; i < A->hei; i++) {
    for (int j = 0; j < A->len; j++) {
      eq = fabs(A->m[i][j] - B->m[i][j]);
      if (eq > 1e-5) {
        return 0;
      }
    }
  }
  return 1;
}

#ifdef TEST_MATRIX_OPS
// Unit testing
int main() {
  matrix* mat = read_matrix_from_file("test.mat");
  print_matrix(mat);
  transpose_matrix(mat);
  print_matrix(mat);
  write_matrix_to_file(mat, "test.mat");
  free_matrix(mat);
  return 0;
}
#endif
