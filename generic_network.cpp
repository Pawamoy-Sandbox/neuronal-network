#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "Network.h"

#define DEF_IN_NUMBER 2
#define DEF_OUT_NUMBER 1

#define ARBITRARY_FITNESS 1000

enum FIT {
	BEST = 0,
	AVG
};

using namespace std;
float* best_fitness;
float* average_fitness;
Model model;
Network* net_model;
BooleanTable table;

int pow(int a, int b)
{
	return b <= 0 ? 1 : a * pow(a, b-1);
}

bool iequals(const string& a, const string& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

bool ReadNetworkModel(const char* file)
{
	string s;
	int l_size, line = 0, lp, np, ls, ns;
	NEURON_TYPE tp=IN, ts=IN;
	bool read_in = false, read_out = false;
	
	ifstream fo(file , std::ios::in);
	
	if (!fo) {
        std::cerr << "Unable to open " << file << " in read-mode" << std::endl;
		return false;
	}
	
	net_model = new Network();
	model.network = net_model;
	
	while (fo >> s)
	{
		line++;
		
		if (iequals(s, "hl"))
		{
			fo >> l_size;
			while (l_size > 0)
			{
				model.network->AddLayer(l_size);
				fo >> l_size;
			}
		}
		else if (iequals(s, "in"))
		{
			if (!read_in)
			{
				fo >> l_size;
				model.network->InLayer()->AddNeuron(l_size);
				read_in = true;
			}
		}
		else if (iequals(s, "out"))
		{
			if (!read_out)
			{
				fo >> l_size;
				model.network->OutLayer()->AddNeuron(l_size);
				read_out = true;
			}
		}
		else if (iequals(s, "connect"))
		{
			fo >> s;
			
			if (iequals(s, "in"))
			{
				tp = IN;
				lp = 1;
				fo >> np;
			}
			else if (iequals(s, "hl"))
			{
				tp = HIDDEN;
				fo >> lp >> np;
			}
			else if (iequals(s, "out"))
			{
				cerr << "Line " << line << ": Impossible to connect an 'OUT' neuron prior to another" << endl; 
				continue;
			}
			
			fo >> s;
		
			if (iequals(s, "to"))
				fo >> s;
				
			if (iequals(s, "in"))
			{
				cerr << "Ligne " << line << ": Impossible to connect a neuron prior to an 'IN' neuron" << endl;
				continue;
			}
			else if (iequals(s, "hl"))
			{
				ts = HIDDEN;
				fo >> ls >> ns; 
			}
			else if (iequals(s, "out"))
			{
				ts = OUT;
				ls = 1;
				fo >> ns;
			}
			
			model.connect.push_back((Connection){tp, ts, lp, np, ls, ns});
		}
	}
	
	if (!read_in)
		model.network->InLayer()->AddNeuron(DEF_IN_NUMBER);
		
	if (!read_out)
		model.network->OutLayer()->AddNeuron(DEF_OUT_NUMBER);
	
	fo.close();
	return true;
}

bool ReadBooleanTable(const char* file)
{
	bool b;
	string s;
	ifstream fo(file , std::ios::in);
	int in_c, out_c, line = 0;
	int in_max = 0, out_max = 0;
	int comb, in_diff, out_diff;
	bool read_one = false;
	
	if (!fo) {
        std::cerr << "Unable to open " << file << " in read-mode" << std::endl;
		return false;
	}
	
	vector<bool> in, out;
	
	// tant qu'on lit un booléen
	while (fo >> b)
	{
		// init in et out count, line count
		in_c = out_c = 0;
		line++;
		
		in_c++;
		in.push_back(b);
		
		fo >> s;
		while (iequals(s, ","))
		{
			// lecture en boucle des entrées et in count
			in_c++;
			fo >> b;
			in.push_back(b);
			fo >> s;
		}
		
		// vérification du nombre d'entrées
		if (in_c != in_max)
		{
			if (!read_one)
			{
				in_max = in_c;
			}
			else
			{
				std::cerr << "Number of IN entries doesn't match previous lines, in " << file << " at line " << line << std::endl;
				fo.close();
				return false;
			}
		}
		
		// lecture de la première sortie
		out_c++;
		fo >> b;
		out.push_back(b);
		
		fo >> s;
		while (iequals(s, ","))
		{
			// lecture en boucle des sorties et out count
			out_c++;
			fo >> b;
			out.push_back(b);
			fo >> s;
		}
		
		// vérification du nombre de sorties
		if (out_c != out_max)
		{
			if (!read_one)
			{
				out_max = out_c;
			}
			else
			{
				std::cerr << "Number of OUT entries doesn't match previous lines, in " << file << " at line " << line << std::endl;
				fo.close();
				return false;
			}
		}
		
		table.Add(in, out);
		in.clear();
		out.clear();
		
		// on a lu une entrée de la table, les autres doivent correspondre (nb in && nb out)
		read_one = true;
	}
	
	// fin lecture, vérif (nb in)^2 -1 == nb entrées, sinon warning
	comb = pow(2, in_max);
	if (comb != table.Size())
		std::cerr << "Warning: Number of rows in boolean table doesn't match the number of combination with IN entries (" << comb << ")" << endl;
		
	// on ajoute des neurones aux couches IN et OUT du réseau s'il en manque comparé aux lignes de la table
	in_diff = in_max - model.network->InLayer()->LayerSize();
	if (in_diff > 0)
		model.network->InLayer()->AddNeuron(in_diff);
	out_diff = out_max - model.network->OutLayer()->LayerSize();
	if (out_diff > 0)
		model.network->OutLayer()->AddNeuron(out_diff);
	
	fo.close();
	return true;
}

int Evolution(const float pc, const float pm, const int nb_generations, const int nb_individus)
{	
    vector<Network*> net = vector<Network*>();
	float fitness_sum;
    Network *network, *winner = NULL;
    if (!ReadNetworkModel("network_model"))
		return -1;
	if (!ReadBooleanTable("boolean_table"))
		return -1;

	// création de nb_individus individus, valeurs au hasard
	for(int i = 0; i < nb_individus; i++)
    {
        network = new Network(model);
        net.push_back(network);
		network->ComputeFitness(table);
	}

	for(int n = 0; n < nb_generations; n++)
	{
		fitness_sum = 0.0;
		best_fitness[n] = ARBITRARY_FITNESS;
		
		// vérification d'un meilleur individu et sauvegarde des fitness
		for(int i = 0; i < nb_individus; i++)
		{
            float error = net.at(i)->GetFitness();
			
			// sauvegarde du meilleur fitness
			if (error < best_fitness[n])
				best_fitness[n] = error;
			
			// ajout du fitness à la somme courante
			fitness_sum += error;
			
			// meilleur individu trouvé ?
			if (error == 0)
				winner = net.at(i);
		}
		
		// mise à jour de la moyenne des marge d'erreur
		average_fitness[n] = fitness_sum / nb_individus;
		
		if (winner != NULL)
		{ // on renvoie l'individu satisfaisant
			cout << "Best neural network found at generation " << n+1;
			cout << " (" << (n+1)*nb_individus << " networks brewed)" << endl;
			cout << endl << "Weights: " << endl;
			winner->ShowWeights();
			cout << endl << "Control test: " << endl;
			winner->ShowResult(table);
			cout << endl;
			return n;
		}
		
		// création de nb_individus enfants
		for(int i = 0; i < nb_individus; i++)
		{
			if((float)rand()/RAND_MAX < pc) // croisement
			{
				// choix de 2 individus au hasard
				int i1, i2;
				i1 = rand() % nb_individus;
				while((i2 = rand() % nb_individus) == i1);
				
				// croisement de ces 2 individus
                model.network = net.at(i2);
                net.push_back(net.at(i1)->CrossOver(model));
			}
			else // clonage
			{
                model.network = net.at(rand() % nb_individus);
                net.push_back(model.network->Clone(model));
			}

			if((float)rand()/RAND_MAX < pm) // mutation de l'enfant ou clone
            {
                net.back()->Mutate();
			}
			
			// calcul de la somme des erreurs
            net.back()->ComputeFitness(table);
		}
		
		// réduction à nb_individus individus
		while((int)net.size() != nb_individus)
		{
			// choix de 2 individus au hasard
			int i1, i2;
			i1 = rand() % nb_individus;
			while((i2 = rand() % nb_individus) == i1);
			
			// fight!
            if(net.at(i1)->GetFitness() > net.at(i2)->GetFitness())
            {
				delete net.at(i1);
				net.erase(net.begin() + i1);
			}
			else
			{
				delete net.at(i2);
				net.erase(net.begin() + i2);
			}
		}
	}
	
	for(int i = 0; i < (int)net.size(); i++)
		delete net.at(i);
	
	return nb_generations-1;
}

bool DrawFitness(const char* foutput, int nb_gen, FIT f)
{
	FILE *file = NULL, *gnu = NULL;
	int i, size, xtics;
	//~ float ayrange ,byrange;
	char out[256];
	bool ret;
	
	file = fopen(foutput, "w");
	
	// writing data-file
	if (file != NULL)
	{
		for (i=0; i<=nb_gen; i++)
			fprintf(file, "%d %f\n", i, (f==BEST?best_fitness[i]:average_fitness[i]));
			
		fclose(file);
	}
	else
	{
		printf("\tError while opening %s\n", foutput);
		return false;
	}
	
	// preparing writing of gnuplot script
	sprintf(out, "%s.gnu", foutput);
	
	xtics = nb_gen;
	size = pow(2, 16)*10;
	
	while (size!=10)
	{
		if (xtics>size)
		{
			xtics = size/10;
			break;
		}
		
		size /= 2;
	}
	
	gnu = fopen(out, "w");
	
	if (gnu != NULL)
	{
		fprintf(gnu, "set title 'Fitness Evolution'\n");
		fprintf(gnu, "set xlabel 'Generation'\n");
		fprintf(gnu, "set ylabel 'Fitness'\n");
		fprintf(gnu, "set terminal png size 800, 600\n");
		fprintf(gnu, "set terminal png enhanced\n");
		fprintf(gnu, "set terminal png font arial 10\n");
		fprintf(gnu, "set output '%s.png'\n", foutput);
		fprintf(gnu, "set autoscale\n");
		fprintf(gnu, "set xtics %d\n", xtics);
		fprintf(gnu, "set grid\n");
		fprintf(gnu, "plot \"%s\" using 1:2 title '%s fitness' with impulses lt 2 lw 2\n", foutput, (f==BEST?"best":"average"));
		fclose(gnu);
	}
	else
	{
		printf("\tError while opening %s\n", out);
		return false;
	}
	
	sprintf(out, "gnuplot < %s.gnu", foutput);
	ret = system(out);
	
	sprintf(out, "rm %s %s.gnu", foutput, foutput);
	ret &= system(out);
	
	return ret;
}

int main(int argc, char *argv[])
{
	float pc; // probabilté croisement
	float pm; // probabilté mutation
	int nb_generations; // nombre de générations max
	int nb_individus; // nombre d'individus
	
	if(argc != 5) // valeurs par défaut
	{
		pc = 0.85;
		pm = 0.1;
		nb_generations = 1000000;
		nb_individus = 100;
	}
	else // usage : ./main pc pm nb_generations
	{
		pc = atof(argv[1]);
		pm = atof(argv[2]);
		nb_generations = atoi(argv[3]);
		nb_individus = atoi(argv[4]);
	}
	
	best_fitness = new float[nb_generations];
	average_fitness = new float[nb_generations];
	
	srand(time(NULL));

    int stop = Evolution(pc, pm, nb_generations, nb_individus);

	if (stop != -1)
	{
		cout << "Average fitness of generation " << stop+1 << ": ";
		cout << average_fitness[stop] << endl;

		DrawFitness("BEST_FITNESS", stop, BEST);
		DrawFitness("AVERAGE_FITNESS", stop, AVG);
	}

	delete net_model;
	delete [] best_fitness;
	delete [] average_fitness;

	return 0;
}
