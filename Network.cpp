#include "Network.h"
#include <stdio.h>

Network::Network()
{
    in_layer = new Layer();
    out_layer = new Layer();
    hidden_layer = std::vector<Layer*>();
}

Network::Network(Model model)
{
    in_layer = new Layer(model.network->InLayer()->LayerSize());
    out_layer = new Layer(model.network->OutLayer()->LayerSize());

	for (int i=1; i<=model.network->HiddenLayerNumber(); i++)
        AddLayer(model.network->GetLayer(i)->LayerSize());

	for (int i=0; i<(int)model.connect.size(); i++)
        Connect(model.connect.at(i), 0);
}

Network::~Network()
{
	delete in_layer;
	
	for (int i=0; i<HiddenLayerNumber(); i++)
		delete hidden_layer.at(i);
	hidden_layer.clear();
	
	delete out_layer;
}

void Network::Init()
{
    // antécédents neurones cachés
    for (int i=1; i<=HiddenLayerNumber(); i++)
        for (int j=1; j<=GetLayer(i)->LayerSize(); j++)
            for (int k=1; k<=GetLayer(i)->GetNeuron(j)->PriorNumber(); k++)
                GetLayer(i)->GetNeuron(j)->GetPrior(k)->weight = 2*((float)rand()/RAND_MAX)-1;

    // antécédents sorties
    for (int i=1; i<=OutLayer()->LayerSize(); i++)
        for (int j=1; j<=OutLayer()->GetNeuron(i)->PriorNumber(); j++)
            OutLayer()->GetNeuron(i)->GetPrior(j)->weight = 2*((float)rand()/RAND_MAX)-1;
}

void Network::AddLayer()
{
    hidden_layer.push_back(new Layer());
}

void Network::AddLayer(int size)
{
    hidden_layer.push_back(new Layer(size));
}

void Network::DelLayer(int i)
{
	hidden_layer.erase(hidden_layer.begin() + i-1);
}

Layer* Network::GetLayer(int i)
{
	return hidden_layer.at(i-1);
}

Layer* Network::InLayer()
{
	return in_layer;
}

Layer* Network::OutLayer()
{
	return out_layer;
}

int Network::HiddenLayerNumber()
{
	return hidden_layer.size();
}

void Network::Connect(Connection c, float weight)
{
	Neuron *prior, *successor;
	
	switch (c.tp)
	{
		case IN:
            prior = in_layer->GetNeuron(c.np);
			break;
			
		case HIDDEN:
            prior = GetLayer(c.lp)->GetNeuron(c.np);
			break;
			
		case OUT:
		default:
			return;
	}
	
	switch (c.ts)
	{
		case OUT:
            successor = out_layer->GetNeuron(c.ns);
			break;
			
		case HIDDEN:
            successor = GetLayer(c.ls)->GetNeuron(c.ns);
			break;
			
		case IN:
		default:
			return;
	}
	
	if (successor != NULL && prior != NULL)
		successor->AddPrior(prior, weight);
}

void Network::Connect(Neuron* prior, float weight, Neuron* successor)
{
	successor->AddPrior(prior, weight);
}

float Network::Test(bool in_state[], bool out_state[])
{
	float error_range = 0.0;
	
	// on affecte les états (0 ou 1) aux entrées
    for (int i=1; i<=in_layer->LayerSize(); i++)
        in_layer->GetNeuron(i)->SetState(in_state[i-1]);
		
	// on calcule les sorties (effet de bord) et la somme des marges d'erreur
    for (int i=1; i<=out_layer->LayerSize(); i++)
        error_range += out_layer->GetNeuron(i)->ComputeState() - (int)out_state[i-1];
		
	// on renvoie la valeur absolue de la somme des marges d'erreur
	return (error_range < 0 ? -error_range : error_range);
}

void Network::SetFitness(float f)
{
	fitness = f;
}

float Network::GetFitness()
{
    return (fitness <= CLOSE_TO_ZERO ? 0 : fitness);
}

void Network::ComputeFitness(BooleanTable bt)
{
	float error_sum = 0.0;
	
	for (int i=0; i<bt.Size(); i++)
		error_sum += Test(bt.In(i), bt.Out(i));
		
	SetFitness(error_sum);
}

void Network::ShowResult(BooleanTable bt)
{	
	bool *in, *out;
	
	for (int i=0; i<bt.Size(); i++)
	{
		in = bt.In(i);
		out = bt.Out(i);
		
		// entrées
		for (int j=0; j<bt.InSize(i); j++)
			printf("%d,", in[j]);
		printf("\b: ");
		// test		
		Test(in, out);
		// sorties
		for (int j=1; j<=OutLayer()->LayerSize(); j++)
			printf("%d,", (int)OutLayer()->GetNeuron(j)->State());
		printf("\b \n");
		
		delete in;
		delete out;
	}
}

void Network::ShowLayers()
{
    printf("IN layer: %d neurons\n", in_layer->LayerSize());
    for (int i=1; i<=HiddenLayerNumber(); i++)
        printf("HIDDEN layer %d: %d neurons\n", i, GetLayer(i)->LayerSize());
    printf("OUT layer: %d neurons\n", out_layer->LayerSize());
}

void Network::ShowWeights()
{
	for (int i=1; i<=HiddenLayerNumber(); i++)
		for (int j=1; j<=GetLayer(i)->LayerSize(); j++)
			for (int k=1; k<=GetLayer(i)->GetNeuron(j)->PriorNumber(); k++)
				printf("w%02d-%02d-%02d: %f\n", i, j, k, GetLayer(i)->GetNeuron(j)->GetPrior(k)->weight);
	
	for (int i=1; i<=OutLayer()->LayerSize(); i++)
		for (int j=1; j<=OutLayer()->GetNeuron(i)->PriorNumber(); j++)
			printf("w%02d-%02d   : %f\n", i, j, OutLayer()->GetNeuron(i)->GetPrior(j)->weight);
}

Network* Network::CrossOver(Model model)
{
    Network* result = new Network(model);
    float weight;

    // antécédents neurones cachés
    for (int i=1; i<=HiddenLayerNumber(); i++)
    {
        for (int j=1; j<=GetLayer(i)->LayerSize(); j++)
        {
            for (int k=1; k<=GetLayer(i)->GetNeuron(j)->PriorNumber(); k++)
            {
                weight = (rand() % 2 == 0) ?
                            GetLayer(i)->GetNeuron(j)->GetPrior(k)->weight :
                            model.network->GetLayer(i)->GetNeuron(j)->GetPrior(k)->weight ;
                result->GetLayer(i)->GetNeuron(j)->GetPrior(k)->weight = weight;
            }
        }
    }

    // antécédents sorties
    for (int i=1; i<=OutLayer()->LayerSize(); i++)
    {
        for (int j=1; j<=OutLayer()->GetNeuron(i)->PriorNumber(); j++)
        {
            weight = (rand() % 2 == 0) ?
                        OutLayer()->GetNeuron(i)->GetPrior(j)->weight :
                        model.network->OutLayer()->GetNeuron(i)->GetPrior(j)->weight ;
            result->OutLayer()->GetNeuron(i)->GetPrior(j)->weight = weight;
        }
    }

    return result;
}

Network* Network::Clone(Model model)
{
    Network* result = new Network(model);

    // antécédents neurones cachés
    for (int i=1; i<=HiddenLayerNumber(); i++)
        for (int j=1; j<=GetLayer(i)->LayerSize(); j++)
            for (int k=1; k<=GetLayer(i)->GetNeuron(j)->PriorNumber(); k++)
                result->GetLayer(i)->GetNeuron(j)->GetPrior(k)->weight = GetLayer(i)->GetNeuron(j)->GetPrior(k)->weight;

    // antécédents sorties
    for (int i=1; i<=OutLayer()->LayerSize(); i++)
        for (int j=1; j<=OutLayer()->GetNeuron(i)->PriorNumber(); j++)
            result->OutLayer()->GetNeuron(i)->GetPrior(j)->weight = OutLayer()->GetNeuron(i)->GetPrior(j)->weight;

    return result;
}

void Network::Mutate()
{
    int rand_out = rand() % OutLayer()->LayerSize() + 1;
    int rand_prior = rand() % OutLayer()->GetNeuron(rand_out)->PriorNumber() + 1;

    Prior* prior = OutLayer()->GetNeuron(rand_out)->GetPrior(rand_prior);
    prior->weight += LO + (float)(rand()) / ((float)(RAND_MAX / (HI-LO)));
    if (prior->neuron != NULL) prior->neuron->MutatePriors();
}
