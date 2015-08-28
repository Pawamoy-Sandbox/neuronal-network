#ifndef __LAYER_H
#define __LAYER_H

#include <vector>
#include "Neuron.h"

class Layer
{
	public:
		Layer();
		Layer(int size);
		~Layer();
		
		void AddNeuron();
		void AddNeuron(int number);
		void AddNeuron(Neuron* n);
		void DelNeuron(int i);
		Neuron* GetNeuron(int i);
		int LayerSize();

	private:
		std::vector<Neuron*> neuron;
};

#endif // __LAYER_H
