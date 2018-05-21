#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include "utils.h"
#include "multiply_ops.h"
#include "matrix_ops.h"


int main(int argc, char** argv) {
    if (argc < 5) {
	fprintf(stderr, "Not enough arguments, need 5 have %d\n", argc);
	sigtrap();
    }

    if (strlen(argv[1]) != 1) {
	fprintf(stderr, "Implementation type specified must be a string of size 1: %s\n", argv[1]);
	sigtrap();
    }
    
    char implementation = argv[1][0];
    char* A_path = argv[2];
    char* B_path = argv[3];
    char* C_path = argv[4];

    if (implementation != 'p' && implementation != 'o') {
	fprintf(stderr, "Implementation type specified is not accepeted: %c\n", implementation);
	sigtrap();
    }
    
    matrix *A, *B, *C;
    A = read_matrix_from_file(A_path);
    B = read_matrix_from_file(B_path);    
    
    double start_time = omp_get_wtime();
    if (implementation == 'o')
	C = multiply_matrix_openMP(A, B);
    else if (implementation == 'p')
	C = multiply_matrix_pthreads(A, B);
    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;
    printf("Time elapsed executing in %s mode: %lf\n", (implementation == 'o' ? "OpenMP" : "Pthreads"), execution_time);

    write_matrix_to_file(C, C_path);

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    return 0;
}
