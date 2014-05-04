#include <iostream>
#include <cstdlib>
#include <vector>
#include <string.h>

#include "Individu.h"

using namespace std;

float Test(Individu &ind, const int In1, const int In2, const float vitesse)
{
	float Hd[3];
	float Out = 0;
	
	/* Calcul de Hd1, Hd2, Hd3, puis de Out */
	for(int i = 0; i < 3; i++)
	{
		Hd[i] = In1*ind.GetWih(1, i+1) + In2*ind.GetWih(2, i+1);
		Hd[i] >= 0.5 ? Hd[i] = 1 : Hd[i] = 0;
		Out += Hd[i]*ind.GetWoh(i+1, 1);
	}
	
	/* Calcul de la marge d'erreur */
	ind.SetError((In1 ^ In2) - Out);
	
	/* Mise à jour des poids */
	for(int i = 0; i < 3; i++)
	{
		ind.SetWih(ind.GetWih(1, i+1) + In1*ind.GetError()*vitesse, 1, i+1);
		ind.SetWih(ind.GetWih(2, i+1) + In2*ind.GetError()*vitesse, 2, i+1);
		ind.SetWoh(ind.GetWoh(i+1, 1) + Hd[i]*ind.GetError()*vitesse, i+1, 1);
	}
	
	return ind.GetPositiveError();
}

float Out(Individu &ind, const int In1, const int In2)
{
	float Hd[3];
	float Out = 0;
	
	/* Calcul de Hd1, Hd2, Hd3, puis de Out */
	for(int i = 0; i < 3; i++)
	{
		Hd[i] = In1*ind.GetWih(1, i+1) + In2*ind.GetWih(2, i+1);
		Hd[i] >= 0.5 ? Hd[i] = 1 : Hd[i] = 0;
		Out += Hd[i]*ind.GetWoh(i+1, 1);
	}
	
	//~ if(Out <= CLOSE_TO_ZERO && Out >= -CLOSE_TO_ZERO) // très proche de 0
		//~ return 0;
	//~ else if(Out <= (1 + CLOSE_TO_ZERO) && Out >= (1 - CLOSE_TO_ZERO)) // très proche de 1
		//~ return 1;
	//~ else
		//~ return Out;
	return (Out < 0.5 ? 0 : 1);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	Individu ind;
	float vitesse; // vitesse d'apprentissage
	float pm; // probabilité de mutation
	float error_sum = 1; // somme des erreurs des 4 tests
	int it = 0; // nombre d'itérations
	
	if(argc != 3) // valeurs par défaut
	{
		vitesse = 0.1;
		pm = 0.8;
	}
	else // usage: ./main vitesse pm
	{
		vitesse = atof(argv[1]);
		pm = atof(argv[2]);
	}
	
	cout << "Initialisation: " << endl;
	cout << "  Considéré comme égal à zéro: [" << -CLOSE_TO_ZERO << ", " << CLOSE_TO_ZERO << "]" << endl;
	cout << "  Intervalle de mutation: [" << LO << ", " << HI << "]" << endl;
	cout << "  Nombre d'itérations dans un cycle: " << CYCLE << endl;
	cout << "  Nombre de cycles dans la boucle: " << NB_CYCLE << endl;
	cout << "  Vitesse d'apprentissage: " << vitesse << endl;
	cout << "  Probabilité de mutation: " << pm << endl;
	cout << endl;
	
	cout << "Poids de départ: " << endl;
	ind.ShowWeights();
	cout << endl;

	while (it<NB_CYCLE*CYCLE)
	{
		int i = 0;
		while (error_sum != 0)
		{
			/* Mutation */
			if((float)rand()/RAND_MAX < pm)
				ind.Mutate();
			
			error_sum = 0;
			//~ error_sum += Test(ind, 0, 0, vitesse); // ne change jamais les poids
			error_sum += Test(ind, 0, 1, vitesse);
			error_sum += Test(ind, 1, 0, vitesse);
			error_sum += Test(ind, 1, 1, vitesse);
			
			i++;
			if (i>=CYCLE) break;
		}
		
		it += i;
		if (error_sum == 0)
			break;
		else
			cout << "Marge d'erreur (" << it/CYCLE << "M):\t" << error_sum << endl;
	}
	
	if (it > CYCLE) cout << endl;
	cout << "Poids trouvés après " << it << " itérations" << std::endl;
	ind.ShowWeights();
	cout << endl;
	
	cout << "Marge d'erreur du dernier test: " << ind.GetExactError() << endl;
	cout << "Sortie pour chaque couple d'entrées avec les poids trouvés: " << endl;

	cout << "  0,0: " << Out(ind, 0, 0) << endl;
	cout << "  0,1: " << Out(ind, 0, 1) << endl;
	cout << "  1,0: " << Out(ind, 1, 0) << endl;
	cout << "  1,1: " << Out(ind, 1, 1) << endl;
	
	return 0;
}
