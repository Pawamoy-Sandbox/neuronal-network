#include <iostream>
#include <cstdlib>

#include "Individu.h"

Individu::Individu()
{
	Init();
}

Individu::~Individu()
{
}

void Individu::Init()
{	
	int a = -10;
	int b = 10;
	/* réels entre -10 et 10 */
	
	for(int i = 0; i < 6; i++)
	{
		m_wih[i] = (b-a)*((float)rand()/RAND_MAX) + a;
	}
	
	for(int i = 0; i < 3; i++)
	{
		m_woh[i] = (b-a)*((float)rand()/RAND_MAX) + a;
	}
	
	m_error = 100;
}

float Individu::GetWih(int i, int j)
{
	if((i != 1 && i != 2) || (j != 1 && j != 2 && j != 3))
	{
		std::cout << "Erreur index GetWih" << std::endl;
		return -1;
	}
	
	return m_wih[(i-1)*3+j-1];
}

float Individu::GetWoh(int i, int j)
{
	if((i != 1 && i != 2 && i != 3) || (j != 1))
	{
		std::cout << "Erreur index GetWoh" << std::endl;
		return -1;
	}
	
	return m_woh[i-1];
}

float Individu::GetError()
{
	return m_error;
}

void Individu::ShowWeights()
{
	std::cout << "wih_11 : " << m_wih[0] << std::endl;
	std::cout << "wih_12 : " << m_wih[1] << std::endl;
	std::cout << "wih_13 : " << m_wih[2] << std::endl;
	std::cout << "wih_21 : " << m_wih[3] << std::endl;
	std::cout << "wih_22 : " << m_wih[4] << std::endl;
	std::cout << "wih_23 : " << m_wih[5] << std::endl;
	std::cout << "woh_11 : " << m_woh[0] << std::endl;
	std::cout << "woh_21 : " << m_woh[1] << std::endl;
	std::cout << "woh_31 : " << m_woh[2] << std::endl;
}

void Individu::SetWih(float wih, int i, int j)
{
	if((i != 1 && i != 2) || (j != 1 && j != 2 && j != 3))
	{
		std::cout << "Erreur index SetWih" << std::endl;
	}
	
	m_wih[(i-1)*3+j-1] = wih;
}

void Individu::SetWoh(float woh, int i, int j)
{
	if((i != 1 && i != 2 && i != 3) || (j != 1))
	{
		std::cout << "Erreur index SetWoh" << std::endl;
	}
	
	m_woh[i-1] = woh;
}

void Individu::SetError(float error)
{
	if(error > -1.0e-5 && error < 1.0e-5)
		m_error = 0;
	else
		m_error = error;
}
