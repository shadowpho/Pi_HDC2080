all: main.o C_HDC2080.o
	gcc -o all main.o C_HDC2080.o
main.o: test_HDC2080.c C_HDC2080.h
	gcc -c -g test_HDC2080.c -o main.o
C_HDC2080.o: C_HDC2080.c C_HDC2080.h
	gcc -c -g C_HDC2080.c
clean:
	rm all *.o
