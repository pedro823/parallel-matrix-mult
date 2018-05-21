FLAGS = -pthread -Wall -pedantic -g -fopenmp -lm

.PHONY: clean, test

all: utils.o multiply_ops.o matrix_ops.o main.o
	$(CC) $(FLAGS) $^ -o main

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@


%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	-rm -f *.o *~ test
