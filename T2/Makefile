allg: #verify if still runs with directory changes
	gcc -g -o ./bin/main *.c

test: #verify if still runs with directory changes
	valgrind --leak-check=full -s ./bin/main

test2:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./bin/main
	
all:
	gcc -c -g ./src/arquivo.c -I ./include -o ./obj/arquivo.o
	gcc -c -g ./src/funcoesFornecidas.c -I ./include -o ./obj/funcoesFornecidas.o
	gcc -c -g ./src/registro.c -I ./include -o ./obj/registro.o
	gcc -c -g ./src/pilha.c -I ./include -o ./obj/pilha.o
	gcc -c -g ./src/grafo.c -I ./include -o ./obj/grafo.o
	gcc -c -g ./src/funcionalidades.c -I ./include -o ./obj/funcionalidades.o
	gcc -g ./apps/main.c ./obj/*.o -I ./include -o ./bin/main

run:
	./bin/main

runwindows:
	.\bin\main

crw:  #compile and run for windows
	gcc -c ./src/arquivo.c -I ./include -o ./obj/arquivo.o
	gcc -c ./src/funcoesFornecidas.c -I ./include -o ./obj/funcoesFornecidas.o
	gcc -c ./src/registro.c -I ./include -o ./obj/registro.o
	gcc -c -g ./src/pilha.c -I ./include -o ./obj/pilha.o
	gcc -c -g ./src/grafo.c -I ./include -o ./obj/grafo.o
	gcc -c ./src/funcionalidades.c -I ./include -o ./obj/funcionalidades.o
	gcc ./apps/main.c ./obj/*.o -I ./include -o ./bin/main
	.\bin\main

crl: #compile and run for linux
	gcc -c ./src/arquivo.c -I ./include -o ./obj/arquivo.o
	gcc -c ./src/funcoesFornecidas.c -I ./include -o ./obj/funcoesFornecidas.o
	gcc -c ./src/registro.c -I ./include -o ./obj/registro.o
	gcc -c -g ./src/pilha.c -I ./include -o ./obj/pilha.o
	gcc -c -g ./src/grafo.c -I ./include -o ./obj/grafo.o
	gcc -c ./src/funcionalidades.c -I ./include -o ./obj/funcionalidades.o
	gcc ./apps/main.c ./obj/*.o -I ./include -o ./bin/main
	./bin/main

push:
	git pull
	git push

merge:
	git add .
	git commit -m "merge"
	git pull
	git push

fino:
	git add .
	git commit -m "ajuste fino"
	git pull
	git push

limpeza:
	git add .
	git commit -m "limpeza de codigo"
	git pull
	git push