#ifndef __NEURON_H
#define __NEURON_H

#include <vector>
#include <cstdlib>

#define LO -0.1
#define HI 0.1

typedef struct str_prior Prior;

class Neuron
{
	public:
		Neuron();
		~Neuron();
		
		void AddPrior(Neuron* n, float w);
		void DelPrior(int i);
        Prior* GetPrior(int i);
		int PriorNumber();
		bool State();
		float ComputeState();
		void SetState(bool s);
        void MutatePriors();
		
	private:
        std::vector<Prior*> prior;
		bool active;
};

struct str_prior {
	Neuron* neuron;
	float weight;
};


#endif // __NEURON_H
