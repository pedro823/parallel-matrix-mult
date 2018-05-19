FLAGS = -pthread -Wall -pedantic -g -fopenmp -lm
OBJS = main.o frog.o
EXEC = main

.PHONY: clean, test

test: utils.o multiply_ops.o matrix_ops.o
	$(CC) $(FLAGS) $^ -o test
	./test

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@


%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	-rm -f *.o *~
