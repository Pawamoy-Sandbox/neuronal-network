#include <iostream>
#include <cstdlib>
#include <vector>

#include "Individu.h"
#include "Equation.h"

using namespace std;

/* Etapes :
 * - initialisation -> création de 100 individus, valeurs au hasard
 * - création de 100 enfants :
 * 		- croisement (probabilité = 0.85 par défaut) 
 * 		  avec possibilité de mutation (probabilité = 0.1 par défaut)
 * 		- clonage sinon
 * - réduction à 100 individus parmi les 200 par confrontation d'individus au hasard,
 * 	 en comparant la somme des erreurs par rapport aux équations données
 * - vérification -> est-ce que la solution est trouvée ? si non, recommencer
*/
bool Evolution(Equation equations[], float &x, float &y, const float pc, const float pm, const int nb_generations)
{	
	vector<Individu*> ind;
	
	/* création de 100 individus, valeurs au hasard */
	for(int i = 0; i < 100; i++)
	{
		ind.push_back(new Individu());
		
		/* calcul de la somme des erreurs */
		float error = equations[0].Error(ind.back()->GetX(), ind.back()->GetY()) + 
					  equations[1].Error(ind.back()->GetX(), ind.back()->GetY());
		ind.back()->SetError(error);
	}
	
	for(int n = 0; n < nb_generations; n++)
	{
		/* création de 100 enfants */
		for(int i = 0; i < 100; i++)
		{
			if((float)rand()/RAND_MAX < pc) /* croisement */
			{
				/* choix de 2 individus au hasard */
				int i1, i2;
				i1 = rand() % 100;
				while((i2 = rand() % 100) == i1);
				
				/* croisement de ces 2 individus */
				ind.push_back(ind.at(i1)->Croisement(ind.at(i2)));
				
				if((float)rand()/RAND_MAX < pm) /* mutation de l'enfant */
				{
					ind.back()->Mutation();
				}
			}
			else /* clonage */
			{
				ind.push_back(ind.at(rand() % 100)->Clonage());
			}
						
			/* calcul de la somme des erreurs */
			float error = equations[0].Error(ind.back()->GetX(), ind.back()->GetY()) + 
						  equations[1].Error(ind.back()->GetX(), ind.back()->GetY());
			ind.back()->SetError(error);
		}
		
		/* réduction à 100 individus */
		while(ind.size() != 100)
		{
			/* choix de 2 individus au hasard */
			int i1, i2;
			i1 = rand() % 100;
			while((i2 = rand() % 100) == i1);
			
			/* fight! */
			if(ind.at(i1)->GetError() > ind.at(i2)->GetError())
				ind.erase(ind.begin() + i1);
			else
				ind.erase(ind.begin() + i2);
		}
		
		/* vérification d'un meilleur individu */
		for(int i = 0; i < 100; i++)
		{
			if(ind.at(i)->GetError() == 0)
			{
				x = ind.at(i)->GetX();
				y = ind.at(i)->GetY();
				cout << "generation " << n+1 << endl;
				return true;
			}
		}
	}
	
	return false;
}

int main(int argc, char *argv[])
{
	float pc; // probabilté croisement
	float pm; // probabilté mutation
	int nb_generations; // nombre de générations max
	
	if(argc != 4) // valeurs par défaut
	{
		pc = 0.85;
		pm = 0.1;
		nb_generations = 1000;
	}
	else // usage : ./main pc pm nb_generations
	{
		pc = atof(argv[1]);
		pm = atof(argv[2]);
		nb_generations = atoi(argv[3]);
	}
	
	srand(time(NULL));
	
	float x;
	float y;
	
	Equation equations[2];
	equations[0].Init('+', 1, 1, 0); // x + y = 0
	equations[1].Init('-', 1, 1, 1); // x - y = 1
	
	/* autre equations test */
	// equations[0].Init('+', 2, 3, 4); // 2x + 3y = 4
	// equations[1].Init('+', 1, 1, 1); // x + y = 1
	
	if(Evolution(equations, x, y, pc, pm, nb_generations))
	{
		cout << "x : " << x << "\ny : " << y << endl;
	}
	else
	{
		cout << "Pas de meilleur individu." << endl;
	}
	
	return 0;
}
