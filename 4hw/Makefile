#Makefile for task assembler 4

main: main.o VectorOperations.o Matrix.o SystemLinearEquations.o LinearVector.o
	g++ -o main main.o VectorOperations.o Matrix.o SystemLinearEquations.o LinearVector.o
main.o: main.cpp SystemLinearEquations.h Matrix.h LinearVector.h
	g++ -c -g main.cpp
SystemLinearEquations.o: SystemLinearEquations.h SystemLinearEquations.cpp Matrix.h LinearVector.h
	g++ -c -g SystemLinearEquations.cpp 
Matrix.o: Matrix.cpp Matrix.h VectorOperations.h
	g++ -c -g Matrix.cpp
LinearVector.o: LinearVector.cpp LinearVector.h VectorOperations.h
	g++ -c -g LinearVector.cpp
VectorOperations.o: VectorOperations.h VectorOperations.S
	gcc -c -g VectorOperations.S -o VectorOperations.o 
clean: 
	rm -f *.o main
