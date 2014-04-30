#ifndef __INDIVIDU_H__
#define __INDIVIDU_H__

class Individu
{
	private:
		float m_x;
		float m_y;
		float m_error; // somme des erreurs par rapport aux équations
		
	public:
		Individu();
		~Individu();
		
		Individu* Croisement(Individu *individu);
		Individu* Clonage();
		void Mutation();
		
		float GetX();
		float GetY();
		float GetError();
		
		void SetError(float error);
	
	private:
		Individu(float x, float y);
		void Init();
};

#endif
