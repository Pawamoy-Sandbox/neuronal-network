#include <iostream>
#include <cstdlib>
#include <vector>
#include <assert.h>
#include <stdio.h>

#include "Individu.h"

#define OP ^

enum FIT {
	BEST = 0,
	AVG
};

using namespace std;
float* best_fitness;
float* average_fitness;

int pow(int a, int b)
{
	return b <= 0 ? 1 : a * pow(a, b-1);
}

float Out(Individu *ind, const int In1, const int In2)
{
	float Hd[3];
	float Out = 0;
	
	// Calcul de Hd1, Hd2, Hd3, puis de Out
	for(int i = 0; i < 3; i++)
	{
		Hd[i] = In1*ind->GetWih(1, i+1) + In2*ind->GetWih(2, i+1);
		Hd[i] >= 0.5 ? Hd[i] = 1 : Hd[i] = 0;
		Out += Hd[i]*ind->GetWoh(i+1, 1);
	}
	
	float error = (In1 OP In2) - Out;
	error = (error < 0) ? -error : error;
	
	return (error <= CLOSE_TO_ZERO ? 0 : error);
}

float Out2(Individu *ind, const int In1, const int In2)
{
	float Hd[3];
	float Out = 0;
	
	// Calcul de Hd1, Hd2, Hd3, puis de Out
	for(int i = 0; i < 3; i++)
	{
		Hd[i] = In1*ind->GetWih(1, i+1) + In2*ind->GetWih(2, i+1);
		Hd[i] >= 0.5 ? Hd[i] = 1 : Hd[i] = 0;
		Out += Hd[i]*ind->GetWoh(i+1, 1);
	}
	
	return (Out < 0.5 ? 0 : 1);
}

int Evolution(const float pc, const float pm, const int nb_generations, const int nb_individus)
{	
	vector<Individu*> ind;
	float fitness_sum;
	Individu* winner = NULL;
	
	// création de nb_individus individus, valeurs au hasard
	for(int i = 0; i < nb_individus; i++)
	{
		ind.push_back(new Individu());
		
		// calcul de la somme des erreurs
		float error = Out(ind.back(), 0, 0) + Out(ind.back(), 1, 0) + Out(ind.back(), 0, 1) + Out(ind.back(), 1, 1);
		ind.back()->SetError(error);
	}
	
	for(int n = 0; n < nb_generations; n++)
	{
		fitness_sum = 0.0;
		best_fitness[n] = 10;
		
		// vérification d'un meilleur individu et sauvegarde des fitness
		for(int i = 0; i < nb_individus; i++)
		{
			float error = ind.at(i)->GetError();
			
			// sauvegarde du meilleur fitness
			if (error < best_fitness[n])
				best_fitness[n] = error;
			
			// ajout du fitness à la somme courante
			fitness_sum += error;
			
			// meilleur individu trouvé ?
			if (error == 0)
				winner = ind.at(i);
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
			cout << "  0,0: " << Out2(winner, 0, 0) << endl;
			cout << "  0,1: " << Out2(winner, 0, 1) << endl;
			cout << "  1,0: " << Out2(winner, 1, 0) << endl;
			cout << "  1,1: " << Out2(winner, 1, 1) << endl;
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
				ind.push_back(ind.at(i1)->Croisement(ind.at(i2)));
			}
			else // clonage
			{
				ind.push_back(ind.at(rand() % nb_individus)->Clonage());
			}

			if((float)rand()/RAND_MAX < pm) // mutation de l'enfant ou clone
			{
				ind.back()->Mutate();
			}
			
			// calcul de la somme des erreurs
			float error = Out(ind.back(), 0, 0) + Out(ind.back(), 1, 0) + Out(ind.back(), 0, 1) + Out(ind.back(), 1, 1);
			ind.back()->SetError(error);
		}
		
		// réduction à nb_individus individus
		while((int)ind.size() != nb_individus)
		{
			// choix de 2 individus au hasard
			int i1, i2;
			i1 = rand() % nb_individus;
			while((i2 = rand() % nb_individus) == i1);
			
			// fight!
			if(ind.at(i1)->GetError() > ind.at(i2)->GetError())
			{
				delete ind.at(i1);
				ind.erase(ind.begin() + i1);
			}
			else
			{
				delete ind.at(i2);
				ind.erase(ind.begin() + i2);
			}
		}
	}
	
	for(int i = 0; i < (int)ind.size(); i++)
	{
		delete ind.at(i);
	}
	
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
	size = (int)pow(2, 16)*10;
	
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

	delete [] best_fitness;
	delete [] average_fitness;

	return 0;
}
