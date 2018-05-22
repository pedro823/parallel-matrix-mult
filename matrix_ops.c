#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrix_ops.h"
#include "utils.h"
#include "test.h"

thread_input* create_input(matrix* A, matrix* B, matrix* C, ulli i) {
    thread_input* cur_input = (thread_input *)emalloc(sizeof(thread_input));
    cur_input->A = A;
    cur_input->B = B;
    cur_input->C = C;
    cur_input->i = i;
    return cur_input;
}


matrix* read_matrix_from_file(char* file_name) {
  ulli h, l;
  FILE* f = fopen(file_name, "r");
  if (f == NULL) {
    fprintf(stderr, "File could not be opened for reading: %s\n", file_name);
    sigtrap();
  }
  fscanf(f, "%llu %llu", &h, &l);
  matrix* mat = emalloc(sizeof(matrix));
  mat->hei = h;
  mat->len = l;
  mat->m = (double **) emalloc(sizeof(double *) * h);
  char* line = NULL;
  size_t len = 0;
  for (ulli i = 0; i < h; i++) {
      mat->m[i] = (double *) ecalloc(l, sizeof(double));
  }
  while (getline(&line, &len, f) != -1) {
    ulli i, j;
    double v;
    if (strlen(line) > 1) { // Removes lines with just '\n'
      sscanf(line, "%llu %llu %lf", &i, &j, &v);
      mat->m[i][j] = v;
    }
  }
  if (line != NULL) {
    free(line);
  }
  fclose(f);
  return mat;
}

ulli write_matrix_to_file(matrix* mat, char* file_name) {
  if (mat == NULL) {
    fprintf(stderr, "write_matrix_to_file: mat is NULL\n");
    sigtrap();
  }
  FILE* f = fopen(file_name, "w");
  if (f == NULL) {
    fprintf(stderr, "File could not be opened for writing: %s\n", file_name);
    sigtrap();
  }
  ulli bytes_written = 0;
  bytes_written += fprintf(f, "%lld %lld\n", mat->hei, mat->len);
  for (ulli i = 0; i < mat->hei; i++) {
    for (ulli j = 0; j < mat->len; j++) {
      if (mat->m[i][j] != 0) { // Don't write out zeros
        bytes_written += fprintf(f, "%lld %lld %lf\n", i, j, mat->m[i][j]);
      }
    }
    bytes_written += fputc('\n', f);
  }
  return bytes_written;
}

void transpose_matrix(matrix* mat) {
  if (mat == NULL) return;
  ulli m = mat->hei, n = mat->len;
  double** new_m = (double **) emalloc(sizeof(double *) * mat->len);
  for (ulli j = 0; j < n; j++) {
    new_m[j] = (double *) emalloc(sizeof(double) * mat->hei);
    for (ulli i = 0; i < m; i++) {
      new_m[j][i] = mat->m[i][j];
    }
  }
  for (ulli i = 0; i < m; i++) {
    efree(mat->m[i]);
  }
  efree(mat->m);
  mat->m = new_m;
  ulli temp_dim = mat->hei;
  mat->hei = mat->len;
  mat->len = temp_dim;
}

void free_matrix(matrix* mat) {
  for (ulli i = 0; i < mat->hei; i++) {
    efree(mat->m[i]);
  }
  efree(mat->m);
  efree(mat);
}

void print_matrix(matrix* mat) {
  printf("%lld %lld\n", mat->hei, mat->len);
  for (ulli i = 0; i < mat->hei; i++) {
    for (ulli j = 0; j < mat->len; j++) {
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
  for (ulli i = 0; i < A->hei; i++) {
    for (ulli j = 0; j < A->len; j++) {
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
