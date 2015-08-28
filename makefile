CC = g++
CFLAGS = -Wall -O2

all : subject_optimized generic_network

subject_optimized : subject_optimized.o Individu.o
	$(CC) $(CFLAGS) -o $@ $^

generic_network : generic_network.o Neuron.o Layer.o Network.o BooleanTable.o
	$(CC) $(CFLAGS) -o $@ $^
	
subject_optimized.o : subject_optimized.cpp Individu.h
generic_network.o : generic_network.cpp Network.h
Individu.o : Individu.cpp Individu.h
Neuron.o : Neuron.cpp Neuron.h
Layer.o : Layer.cpp Layer.h Neuron.h
Network.o : Network.cpp Network.h Layer.h BooleanTable.h
BooleanTable.o : BooleanTable.cpp BooleanTable.h

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

clean :
	rm *.o

rmproper : clean
	rm subject_optimized generic_network *.png
