#ifndef __EQUATION_H__
#define __EQUATION_H__

class Equation
{
	private:
		char m_sign;
		float m_weightX;
		float m_weightY;
		float m_val;
		
	public:
		Equation();
		~Equation();
		
		void Init(char sign, float weightX, float weightY, float val);
		float Error(const float x, const float y);
};

#endif
