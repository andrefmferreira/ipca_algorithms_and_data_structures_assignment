all: main

main: main.o misc.o statisticFun.o import.o
	gcc -g -o main main.o misc.o statisticFun.o import.o

main.o: main.c
	gcc -g -o main.o main.c -c

misc.o: ./bin/misc.c ./bin/misc.h

	gcc -g -o misc.o ./bin/misc.c -c

statisticFun.o: ./bin/statisticFun.c ./bin/statisticFun.h
	gcc -g -o statisticFun.o ./bin/statisticFun.c -c

import.o: ./bin/import.c ./bin/import.h
	gcc -g -o import.o ./bin/import.c -c

clean:
	del /Q /S *.o *~ main
	#se linux usar:
	#rm -rf *o *~ main

#compilar com o comando: mingw32-make all
#limpar com o comando: mingw32-make clean