#include <iostream>
#include <cstdlib>

#include "Individu.h"

Individu::Individu()
{
	Init();
}

Individu::Individu(float x, float y):
m_x(x),
m_y(y),
m_error(0)
{
}

Individu::~Individu()
{
}

void Individu::Init()
{
	int a = -1000;
	int b = 1000;
	/* réels entre -10 et 10 */
	m_x = (b-a)*((float)rand()/RAND_MAX) + a;
	m_y = (b-a)*((float)rand()/RAND_MAX) + a;
	
	m_error = 0;
}


Individu* Individu::Croisement(Individu *individu)
{
	/* enfant = (ind1 + ind2) / 2*/
	return new Individu((m_x + individu->GetX()) / 2, (m_y + individu->GetY()) / 2);
}

Individu* Individu::Clonage()
{
	return new Individu(m_x, m_y);
}

void Individu::Mutation()
{
	if(rand() % 2) /* choix de l'allèle */
		m_x += 1 - 2*((float)rand()/RAND_MAX);
	else
		m_y += 1 - 2*((float)rand()/RAND_MAX);
}

float Individu::GetX()
{
	return m_x;
}

float Individu::GetY()
{
	return m_y;
}

float Individu::GetError()
{
	return m_error;
}

void Individu::SetError(float error)
{
	m_error = error;
}
