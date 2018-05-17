FLAGS = -pthread -Wall -pedantic -g
OBJS = main.o frog.o
EXEC = main

.PHONY: clean

test: utils.o matrix_ops.o
	$(CC) $(FLAGS) $^ -o test
	./test

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@


%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	-rm -f *.o *~
