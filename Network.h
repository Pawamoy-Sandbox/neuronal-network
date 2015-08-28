#ifndef __NETWORK_H
#define __NETWORK_H

#include <vector>
#include <stdio.h>
#include <cstdlib>
#include "Layer.h"
#include "BooleanTable.h"

#define CLOSE_TO_ZERO 0.00001

enum NEURON_TYPE {
	IN = 0,
	HIDDEN,
	OUT
};

typedef struct str_connect
{
	NEURON_TYPE tp, ts;
	int lp, np, ls, ns;
} Connection;

typedef struct str_model Model;

class Network
{
	public:
		Network();
		Network(Model model);
		~Network();
        void Init();
		void AddLayer();
		void AddLayer(int size);
		void DelLayer(int i);
        Layer* GetLayer(int i);
        Layer* InLayer();
        Layer* OutLayer();
		int HiddenLayerNumber();
		void Connect(Connection c, float weiht);
		void Connect(Neuron* prior, float weight, Neuron* successor);
		float Test(bool in_state[], bool out_state[]);
		void SetFitness(float f);
		float GetFitness();
		void ComputeFitness(BooleanTable bt);
        void ShowLayers();
        void ShowWeights();
        void ShowResult(BooleanTable bt);

        Network* CrossOver(Model model);
        Network* Clone(Model model);
        void Mutate();
	
	private:
        Layer* in_layer;
        Layer* out_layer;
        std::vector<Layer*> hidden_layer;
		float fitness;
};

struct str_model
{
	Network* network;
	std::vector<Connection> connect;
};

#endif // __NETWORK_H
