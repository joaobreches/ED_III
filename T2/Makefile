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

crl: #compile and run for linux
	clear
	make all
	make run

push:
	git pull
	git push
	clear

merge:
	git add .
	git commit -m "merge"
	git pull
	git push
	clear

fino:
	git add .
	git commit -m "ajuste fino"
	git pull
	git push
	clear

limpeza:
	git add .
	git commit -m "limpeza de codigo"
	git pull
	git push
	clear

comentarios:
	git add .
	git commit -m "comentarios"
	git pull
	git push
	clear

erro10:
	git add .
	git commit -m "resolvendo erro f10"
	git pull
	git push
	clear