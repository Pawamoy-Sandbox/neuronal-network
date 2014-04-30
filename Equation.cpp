#include "Equation.h"

Equation::Equation()
{
}

Equation::~Equation()
{
}

void Equation::Init(char sign, float weightX, float weightY, float val)
{
	m_sign = sign;
	m_weightX = weightX;
	m_weightY = weightY;
	m_val = val;
}

float Equation::Error(const float x, const float y)
{
	float error;
	
	if(m_sign == '+')
	{
		error = m_weightX*x + m_weightY*y - m_val;
		return error < 0.0f ? -error : error; // valeur absolue erreur
	}
	else //if(m_sign == '-')
	{
		error = m_weightX*x - m_weightY*y - m_val;
		return error < 0.0f ? -error : error; // valeur absolue erreur
	}
}
