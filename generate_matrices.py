#!/bin/env python3
from sys import argv, exit
from random import SystemRandom
random = SystemRandom()

def print_usage(args):
    print('Usage:', args[0], 'outfile', '<M>', '<N>')

def print_matrix_to_file(matrix, outfile):
    m, n = len(matrix), len(matrix[0])
    with open(outfile, 'w') as f:
        f.write(str(m) + ' ' + str(n) + '\n')
        for i, v in enumerate(matrix):
            for j, item in enumerate(v):
                f.write(' '.join([str(i), str(j), str(round(item, 3))]) + '\n')
            #     str_item =  ' ' * (idx != 0) + str(round(item, 2))
            #     f.write(str_item)
            # f.write('\n')

def generate_matrix(m, n):
    matrix = list()
    for i in range(m):
        matrix.append(list())
        for j in range(n):
            matrix[i].append(random.random() * 50)
    return matrix

if len(argv) != 4:
    print_usage(argv)
    exit(0)

print_matrix_to_file(generate_matrix(int(argv[2]), int(argv[3])), argv[1])
