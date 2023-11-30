<<<<<<< HEAD
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
=======
allg: #verify if still runs with directory changes
	gcc -g -o ./bin/main *.c

test: #verify if still runs with directory changes
	valgrind --leak-check=full -s ./bin/main

all:
	gcc -c ./src/funcoesFornecidas.c -I ./include -o ./obj/funcoesFornecidas.o
	gcc -c ./src/registro.c -I ./include -o ./obj/registro.o
	gcc -c ./src/funcionalidades.c -I ./include -o ./obj/funcionalidades.o
	gcc ./apps/main.c ./obj/*.o -I ./include -o ./bin/main

run:
	./bin/main

runwindows:
	.\bin\main

crw:  #compile and run for windows
	gcc -c ./src/funcoesFornecidas.c -I ./include -o ./obj/funcoesFornecidas.o
	gcc -c ./src/registro.c -I ./include -o ./obj/registro.o
	gcc -c ./src/funcionalidades.c -I ./include -o ./obj/funcionalidades.o
	gcc ./apps/main.c ./obj/*.o -I ./include -o ./bin/main
	.\bin\main

crl: #compile and run for linux
	gcc -c ./src/funcoesFornecidas.c -I ./include -o ./obj/funcoesFornecidas.o
	gcc -c ./src/registro.c -I ./include -o ./obj/registro.o
	gcc -c ./src/funcionalidades.c -I ./include -o ./obj/funcionalidades.o
	gcc ./apps/main.c ./obj/*.o -I ./include -o ./bin/main
	./bin/main
>>>>>>> 7974bcc96641a306241f6cfa4ad2e6d9d4fa9a62
