#include <iostream>
#include <cstdlib>

#include "Individu.h"

Individu::Individu()
{
	Init();
}

Individu::Individu(float wih[6], float woh[3])
{
	for(int i = 0; i < 6; i++)
	{
		m_wih[i] = wih[i];
	}
	
	for(int i = 0; i < 3; i++)
	{
		m_woh[i] = woh[i];
	}
}

Individu::~Individu()
{
}

void Individu::Init()
{	
	for(int i = 0; i < 6; i++)
	{
		m_wih[i] = 2*((float)rand()/RAND_MAX) - 1;
	}
	
	for(int i = 0; i < 3; i++)
	{
		m_woh[i] = 2*((float)rand()/RAND_MAX) - 1;
	}
	
	m_error = 2;
}

void Individu::Mutate()
{
	float mutation;
	
	switch(rand()%3)
	{
		case 0:
		for(int i = 0; i < 3; i++)
		{
			mutation = LO + (float)(rand()) / ((float)(RAND_MAX / (HI-LO)));
			SetWih(GetWih(1, i+1) + mutation, 1, i+1);
		}
		break;
		
		case 1:
		for(int i = 0; i < 3; i++)
		{
			mutation = LO + (float)(rand()) / ((float)(RAND_MAX / (HI-LO)));
			SetWih(GetWih(2, i+1) + mutation, 2, i+1);
		}
		break;
		
		case 2:
		for(int i = 0; i < 3; i++)
		{
			mutation = LO + (float)(rand()) / ((float)(RAND_MAX / (HI-LO)));
			SetWoh(GetWoh(i+1, 1) + mutation, i+1, 1);
		}
		break;
	}
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

float Individu::GetWih(int i)
{
	return m_wih[i];
}

float Individu::GetWoh(int i)
{
	return m_woh[i];
}

float Individu::GetError()
{
	return (m_error <= CLOSE_TO_ZERO && m_error >= -CLOSE_TO_ZERO ? 0 : m_error);
}

void Individu::ShowWeights()
{
	std::cout << "  wih_11: " << m_wih[0] << "\twoh_11: " << m_woh[0] << std::endl;
	std::cout << "  wih_12: " << m_wih[1] << "\twoh_21: " << m_woh[1] << std::endl;
	std::cout << "  wih_13: " << m_wih[2] << "\twoh_31: " << m_woh[2] << std::endl;
	std::cout << "  wih_21: " << m_wih[3] << std::endl;
	std::cout << "  wih_22: " << m_wih[4] << std::endl;
	std::cout << "  wih_23: " << m_wih[5] << std::endl;
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
	m_error = error;
}

Individu* Individu::Croisement(Individu *individu)
{
	float wih[6], woh[3];
	
	for(int i = 0; i < 6; i++)
	{
		wih[i] = (rand() % 2 == 0) ? m_wih[i] : individu->GetWih(i);
	}
	
	for(int i = 0; i < 3; i++)
	{
		woh[i] = (rand() % 2 == 0) ? m_woh[i] : individu->GetWoh(i);
	}
	
	return new Individu(wih, woh);
}

Individu* Individu::Clonage()
{
	return new Individu(m_wih, m_woh);
}
