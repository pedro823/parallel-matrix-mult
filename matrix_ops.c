#include <stdio.h>
#include "matrix_ops.h"
#include "utils.h"

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
  mat->m = (float **) emalloc(sizeof(float *) * h);
  for (int i = 0; i < h; i++) {
    mat->m[i] = (float *) emalloc(sizeof(float) * l);
    for (int j = 0; j < l; j++) {
      fscanf(f, "%f", &mat->m[i][j]);
    }
  }
  fclose(f);
  return mat;
}

void transpose_matrix(matrix* mat) {
  if (mat == NULL) return;
  int m = mat->hei, n = mat->len;
  float temp;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      temp = mat->m[i][j];
      mat->m[i][j] = mat->m[j][i];
      mat->m[j][i] = temp;
    }
  }
  int temp_dim = mat->hei;
  mat->hei = mat->len;
  mat->len = temp_dim;
}

#ifndef __MAIN_FUNC__
#define __MAIN_FUNC__
// Unit testing
int main() {
  matrix* mat = read_matrix_from_file("test.mat");
  printf("%d %d\n", mat->hei, mat->len);
  for (int i = 0; i < mat->hei; i++) {
    for (int j = 0; j < mat->len; j++) {
      printf("%f ", mat->m[i][j]);
    }
    printf("\n");
  }
  transpose_matrix(mat);
  printf("%d %d\n", mat->hei, mat->len);
  for (int i = 0; i < mat->hei; i++) {
    for (int j = 0; j < mat->len; j++) {
      printf("%f ", mat->m[i][j]);
    }
    printf("\n");
  }
  return 0;
}
#endif
