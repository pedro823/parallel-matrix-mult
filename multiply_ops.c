#include "multiply_ops.h"
#include "matrix_ops.h"
#include "test.h"
#include "utils.h"
#include <omp.h>
#include <stdio.h>
#include <pthread.h>

void* thread_line_multiply(void* multiply_input) {
    thread_input* input = (thread_input *)multiply_input;
    matrix* A = input->A;
    matrix* B = input->B;
    matrix* C = input->C;
    ulli id = input->i;
    ulli st = (id * A->hei) / 32;
    ulli ed = ((id + 1) * A->hei) / 32;
    double sum;
    
    for (ulli i = st; i < ed; i++) {
	for (ulli j = 0; j < B->hei; j++) {
	    sum = 0;
	    for (ulli k = 0; k < A->len; k++) {
		sum += A->m[i][k] * B->m[j][k];
	    }
	    C->m[i][j] = sum;
	}
    }
    free(input);
    return NULL;
}

matrix* multiply_matrix_pthreads(matrix* A, matrix* B) {
    // Executes in parallel with pthreads
    if (!matrix_check(A, B)) {
	return NULL;
    }
    matrix* mat = emalloc(sizeof(matrix));
    mat->hei = A->hei;
    mat->len = B->len;
    mat->m = (double **) emalloc(sizeof(double *) * A->hei);
    // Transposes matrix B to remove most cache misses
    transpose_matrix(B);
  
    pthread_t* threads = emalloc(sizeof(pthread_t) * 32);
  
    for (ulli i = 0; i < A->hei; i++) {
	mat->m[i] = (double *) emalloc(sizeof(double) * B->hei);
    }

    for (ulli i = 0; i < 32; i++) {
	thread_input* cur_input = create_input(A, B, mat, i);
	if (pthread_create(&threads[i], NULL, thread_line_multiply, cur_input)) {
	    fprintf(stderr, "Could not create thread\n");
	    sigtrap();
	}
    }
  
    for (ulli i = 0; i < 32; i++) {
	if (pthread_join(threads[i], NULL)) {
	    fprintf(stderr, "Could not join thread\n");
	    sigtrap();
	}
    }
  
    return mat;
}

matrix* multiply_matrix_openMP(matrix* A, matrix* B) {
    // Executes in parallel with OpenMP
    double sum;
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
    for (ulli i = 0; i < A->hei; i++) {
	mat->m[i] = (double *) emalloc(sizeof(double) * B->hei);
#pragma omp parallel for private(sum)
	for (ulli j = 0; j < B->hei; j++) {
	    sum = 0;
	    for (ulli k = 0; k < A->len; k++) {
		sum += A->m[i][k] * B->m[j][k];
	    }
	    mat->m[i][j] = sum;
	}
    }
    return mat;
}

matrix* sequential_multiply(matrix* A, matrix* B) {
    // O(n^3)
    double sum;
    if (!matrix_check(A, B)) {
	return NULL;
    }
    matrix* mat = emalloc(sizeof(matrix));
    mat->hei = A->hei;
    mat->len = B->len;
    mat->m = (double **) emalloc(sizeof(double *) * A->hei);
    for (ulli i = 0; i < A->hei; i++) {
	mat->m[i] = (double *) emalloc(sizeof(double) * B->len);
	for (ulli j = 0; j < B->len; j++) {
	    sum = 0;
	    for (ulli k = 0; k < A->len; k++) {
		sum += A->m[i][k] * B->m[k][j];
	    }
	    mat->m[i][j] = sum;
	}
    }
    return mat;
}

matrix* sequential_multiply_with_transpose(matrix* A, matrix* B) {
    double sum;
    if (!matrix_check(A, B)) {
	return NULL;
    }
    matrix* mat = emalloc(sizeof(matrix));
    mat->hei = A->hei;
    mat->len = B->len;
    mat->m = (double **) emalloc(sizeof(double *) * A->hei);
    // Transposes matrix B to remove cache misses
    transpose_matrix(B);
    for (ulli i = 0; i < A->hei; i++) {
	mat->m[i] = (double *) emalloc(sizeof(double) * B->hei);
	for (ulli j = 0; j < B->hei; j++) {
	    sum = 0;
	    for (ulli k = 0; k < A->len; k++) {
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

    double parallelOMP_time_s = omp_get_wtime();
    D = multiply_matrix_openMP(A, B);
    double parallelOMP_time_e = omp_get_wtime();
    float parallelOMP_time = parallelOMP_time_e - parallelOMP_time_s;
    printf("Time elapsed in parallel with OpenMP: %fs\n", parallelOMP_time);
    printf("%d\n", aprox_equal(C, D));
    free_matrix(D);
    transpose_matrix(B);
  
    double parallelPthreads_time_s = omp_get_wtime();
    D = multiply_matrix_pthreads(A, B);
    double parallelPthreads_time_e = omp_get_wtime();
    float parallelPthreads_time = parallelPthreads_time_e - parallelPthreads_time_s;
    printf("Time elapsed in parallel with Pthreads: %fs\n", parallelPthreads_time);
    printf("%d\n", aprox_equal(C, D));
  
  
    write_matrix_to_file(C, "test_C.mat");

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    free_matrix(D);
    return 0;
}
#endif
