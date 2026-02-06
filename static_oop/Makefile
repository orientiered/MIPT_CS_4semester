soop.exe: build/static_oop.o build/dog.o build/cat.o
	g++	$^ -o $@

build/static_oop.o: static_oop.c cat.h dog.h
	g++ -c $< -o $@

build/dog.o: dog.c dog.h
	g++ -c $< -o $@

build/cat.o: cat.c cat.h
	g++ -c $< -o $@
