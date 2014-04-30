main : main.o Individu.o
	g++ -Wall -o main main.o Individu.o
	
main.o : main.cpp Individu.h
Individu.o : Individu.cpp Individu.h

%.o : %.cpp
	g++ -Wall -c $<

clean :
	rm *.o main
