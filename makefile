main : main.o Individu.o Equation.o
	g++ -Wall -o main main.o Individu.o Equation.o
	
main.o : main.cpp Individu.h Equation.h
Individu.o : Individu.cpp Individu.h
Equation.o : Equation.cpp Equation.h

%.o : %.cpp
	g++ -Wall -c $<

clean :
	rm *.o main
