#include "Neuron.h"

Neuron::Neuron()
{
    prior = std::vector<Prior*>();
	active = false;
}

Neuron::~Neuron()
{
	for (int i=0; i<PriorNumber(); i++)
		delete prior.at(i);
		
	prior.clear();
}

void Neuron::AddPrior(Neuron* n, float w)
{
	Prior* p = new Prior;
	p->neuron = n;
	p->weight = w;
    prior.push_back(p);
}

void Neuron::DelPrior(int i)
{
	delete prior.at(i-1);
	prior.erase(prior.begin() + i-1);
}

Prior* Neuron::GetPrior(int i)
{
	return prior.at(i-1);
}

int Neuron::PriorNumber()
{
	return prior.size();
}

bool Neuron::State()
{
	return active;
}

void Neuron::SetState(bool s)
{
	active = s;
}

float Neuron::ComputeState()
{
	float tmp_sum = 0;
	
	if (PriorNumber() > 0)
	{
		for (int i=1; i<=PriorNumber(); i++)
		{
            GetPrior(i)->neuron->ComputeState();
            tmp_sum += (int)GetPrior(i)->neuron->State() * GetPrior(i)->weight;
		}
		
		SetState(tmp_sum >= 0.5 ? true : false);
		return tmp_sum;
	}
	
	return (float)State();
}

void Neuron::MutatePriors()
{
    for (int i=1; i<=PriorNumber(); i++)
    {
        GetPrior(i)->weight += LO + (float)(rand()) / ((float)(RAND_MAX / (HI-LO)));
        if (GetPrior(i)->neuron != NULL) GetPrior(i)->neuron->MutatePriors();
    }
}
