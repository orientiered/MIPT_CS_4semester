CC = g++
CFLAGS = -Wall
cppoly.exe: build/animal.o build/dog.o build/cat.o build/cpp_poly.o
	$(CC) $(CFLAGS)	$^ -o $@

build/cpp_poly.o: poly.cpp animal.h cat.h dog.h
	$(CC) $(CFLAGS) -c $< -o $@

build/animal.o: animal.cpp animal.h
	$(CC) $(CFLAGS) -c $< -o $@

build/dog.o: dog.cpp animal.h dog.h
	$(CC) $(CFLAGS) -c $< -o $@

build/cat.o: cat.cpp animal.h cat.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm build/*
