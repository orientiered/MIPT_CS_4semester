CC = g++

bind.exe: build/obj.o build/animal.o build/dog.o build/cat.o build/dynbind.o
	$(CC)	$^ -o $@

build/dynbind.o: dynbind.c obj.h animal.h cat.h dog.h
	$(CC) -c $< -o $@

build/obj.o: obj.c obj.h
	$(CC) -c $< -o $@

build/animal.o: animal.c obj.h animal.h
	$(CC) -c $< -o $@

build/dog.o: dog.c obj.h animal.h dog.h
	$(CC) -c $< -o $@

build/cat.o: cat.c obj.h animal.h cat.h
	$(CC) -c $< -o $@

.PHONY: clean
clean:
	rm build/*
