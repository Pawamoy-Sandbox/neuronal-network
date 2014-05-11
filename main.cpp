#include <iostream>
#include <cstdlib>
#include <vector>
#include <string.h>

#include "Individu.h"

#define OP ^

using namespace std;

float Learn(Individu &ind, const int In1, const int In2, const float vitesse)
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
	ind.SetError((In1 OP In2) - Out);
	
	/* Mise à jour des poids */
	for(int i = 0; i < 3; i++)
	{
		ind.SetWih(ind.GetWih(1, i+1) + In1*ind.GetError()*vitesse, 1, i+1);
		ind.SetWih(ind.GetWih(2, i+1) + In2*ind.GetError()*vitesse, 2, i+1);
		ind.SetWoh(ind.GetWoh(i+1, 1) + Hd[i]*ind.GetError()*vitesse, i+1, 1);
	}
	
	return ind.GetPositiveError();
}

float Check(Individu &ind, const int In1, const int In2)
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
	
	Out = (In1 OP In2) - Out;
	return (Out < 0 ? -Out : Out);
}

float Out(Individu &ind, const int In1, const int In2)
{
	float Out = Check(ind, In1, In2);
	return (Out < 0.5 ? 0 : 1);
}

float ErrorSum(Individu &ind)
{
	float sum = 0;
	sum += Check(ind, 0, 0);
	sum += Check(ind, 1, 1);
	sum += Check(ind, 0, 1);
	sum += Check(ind, 1, 0);
	return sum;
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
		vitesse = 0.4;
		pm = 0.4;
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

/* ---------------- Boucle d'apprentissage ! ---------------- */
	while (it<NB_CYCLE*CYCLE)
	{
		int i = 0;
		while (error_sum != 0)
		{
			/* Mutation */
			if ((float)rand()/RAND_MAX < pm)
				ind.Mutate();
			
			switch (rand()%4)
			{
				case 0:
					//~ Learn(ind, 0, 0, vitesse);
					//~ error_sum = ErrorSum(ind);
					break;
				case 1:
					Learn(ind, 0, 1, vitesse);
					error_sum = ErrorSum(ind);
					break;
				case 2:
					Learn(ind, 1, 0, vitesse);
					error_sum = ErrorSum(ind);
					break;
				case 3:
					Learn(ind, 1, 1, vitesse);
					error_sum = ErrorSum(ind);
					break;
			}
			
			i++;
			if (i>=CYCLE) break;
		}
		
		it += i;
		if (error_sum == 0)
			break;
		else
			cout << "Marge d'erreur (" << it/CYCLE << " cycles):\t" << error_sum << endl;
	}
/* ---------------- Fin boucle d'apprentissage ! ---------------- */
	
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
