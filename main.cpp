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
	
	return (Out >= 0.5 ? 1 : 0);
	//~ return Out;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	Individu ind;
	float vitesse = 0.1;
	float error_sum = 1;
	int i, it = 0;
	int out1, out2, out3, out4;
	bool mutate = true;
	
	cout << "Initialisation: " << endl;
	cout << "  Considéré comme égal à zéro: [" << -CLOSE_TO_ZERO << ", " << CLOSE_TO_ZERO << "]" << endl;
	cout << "  Intervalle de mutation: [" << LO << ", " << HI << "]" << endl;
	cout << "  Nombre d'itérations dans un cycle: " << CYCLE << endl;
	cout << "  Nombre de cycles dans la boucle: " << NB_CYCLE << endl;
	cout << endl;
	
	cout << "Poids de départ: " << endl;
	ind.ShowWeights();
	cout << endl;
	
	if (argc > 1)
		if (strcmp(argv[1], "--no-mutation") == 0)
			mutate = false;
	
	while (it<NB_CYCLE*CYCLE)
	{
		i = 0;
		
		while (error_sum != 0)
		{
			if (mutate) ind.Mutate();
			
			error_sum = 0;
			//~ error_sum += Test(ind, 0, 0, vitesse);
			error_sum += Test(ind, 0, 1, vitesse);
			error_sum += Test(ind, 1, 0, vitesse);
			error_sum += Test(ind, 1, 1, vitesse);
			
			i++;
			if (i>=CYCLE) break;
		}
		
		it += i;
		if (error_sum == 0) break;
		cout << "Marge d'erreur (" << it/CYCLE << "M):\t" << error_sum << endl;
	}
	
	if (it > CYCLE) cout << endl;
	cout << "Poids trouvés après " << it << " itérations" << std::endl;
	ind.ShowWeights();
	cout << std::endl;
	
	cout << "Marge d'erreur du dernier test: " << ind.GetExactError() << endl;
	cout << "Sortie pour chaque couple d'entrées avec les poids trouvés: " << endl;
	
	out1 = Out(ind, 0, 0);
	out2 = Out(ind, 0, 1);
	out3 = Out(ind, 1, 0);
	out4 = Out(ind, 1, 1);
	
	i = 0;
	if (Out(ind, 0, 0) == 0) i++;
	if (Out(ind, 0, 1) == 1) i++;
	if (Out(ind, 1, 0) == 1) i++;
	if (Out(ind, 1, 1) == 0) i++;
	
	cout << "  0,0: " << out1 << endl;
	cout << "  0,1: " << out2 << endl;
	cout << "  1,0: " << out3 << endl;
	cout << "  1,1: " << out4 << endl;
	cout << endl;
	
	cout << "Pourcentage de réussite: " << i*25 << "%" << endl;
	
	return 0;
}
