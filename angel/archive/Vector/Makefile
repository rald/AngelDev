vector: main.c vector.c vector.h
	gcc main.c vector.c -o vector -g

.PHONY: check clean

check:
	valgrind --leak-check=full --show-leak-kinds=all ./vector

clean:
	rm vector
