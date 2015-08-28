#include "Layer.h"

Layer::Layer()
{
}

Layer::Layer(int size)
{
	AddNeuron(size);
}

Layer::~Layer()
{
	for (int i=0; i<LayerSize(); i++)
		delete neuron.at(i);
		
	neuron.clear();
}

void Layer::AddNeuron()
{
	neuron.push_back(new Neuron());
}

void Layer::AddNeuron(int number)
{
	for (int i=0; i<number; i++)
		AddNeuron();
}
void Layer::AddNeuron(Neuron* n)
{
	neuron.push_back(n);
}

void Layer::DelNeuron(int i)
{
	neuron.erase(neuron.begin() + i-1);
}

Neuron* Layer::GetNeuron(int i)
{
	return neuron.at(i-1);
}

int Layer::LayerSize()
{
	return neuron.size();
}
