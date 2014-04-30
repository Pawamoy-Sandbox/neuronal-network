#include <iostream>
#include <cstdlib>
#include <vector>

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
		Hd[i] > 0.5 ? Hd[i] = 1 : Hd[i] = 0;
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
	
	return ind.GetError();
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	
	Individu ind;
	
	float vitesse = 0.4;

	ind.SetError(100);
	int a = 0;
	while(ind.GetError() != 0)
	{
		Test(ind, 0, 0, vitesse);
		a++;
	}
	
	ind.ShowWeights();
	cout << a << endl;

	ind.SetError(100);
	a = 0;
	while(ind.GetError() != 0)
	{
		Test(ind, 0, 1, vitesse);
		a++;
	}
	
	ind.ShowWeights();
	cout << a << endl;

	ind.SetError(100);
	a = 0;
	while(ind.GetError() != 0)
	{
		Test(ind, 1, 0, vitesse);
		a++;
	}
	
	ind.ShowWeights();
	cout << a << endl;

	ind.SetError(100);
	a = 0;
	while(ind.GetError() != 0)
	{
		Test(ind, 1, 1, vitesse);
		a++;
	}
	
	ind.ShowWeights();
	cout << a << endl;
	
	Test(ind, 0, 0, vitesse);
	Test(ind, 0, 1, vitesse);
	Test(ind, 1, 0, vitesse);
	Test(ind, 1, 1, vitesse);
	
	return 0;
}
