aaaa:
	gcc -c ./funcoesBasicas.c -o ./funcoesBasicas.o
	gcc -c ./funcoesFornecidas.c -o ./funcoesFornecidas.o
	gcc -c ./T_zero.c -o ./T_zero

all:
	gcc -Wall -g -I. *.c -o T_zero

run:
	./T_zero

test:
	valgrind ./T_zero