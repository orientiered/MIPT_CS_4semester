CC = gcc

spoly.exe: build/animal.o build/dog.o build/cat.o build/static_poly.o
	$(CC)	$^ -o $@

build/static_poly.o: static_poly.c animal.h cat.h dog.h
	$(CC) -c $< -o $@

build/animal.o: animal.c animal.h
	$(CC) -c $< -o $@

build/dog.o: dog.c animal.h dog.h
	$(CC) -c $< -o $@

build/cat.o: cat.c animal.h cat.h
	$(CC) -c $< -o $@

.PHONY: clean
clean:
	rm build/*
