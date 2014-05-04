main : main.o Individu.o
	g++ -Wall -g -o main main.o Individu.o
	
main.o : main.cpp Individu.h
Individu.o : Individu.cpp Individu.h

%.o : %.cpp
	g++ -Wall -g -c $<

clean :
	rm *.o main
