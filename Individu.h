#ifndef __INDIVIDU_H__
#define __INDIVIDU_H__

#define CLOSE_TO_ZERO 0.00001
#define LO -0.1
#define HI 0.1

class Individu
{
	private:
		float m_wih[6];
		float m_woh[3];
		float m_error;
		
	public:
		Individu();
		Individu(float wih[6], float woh[3]);
		~Individu();
		
		float GetWih(int i, int j);
		float GetWoh(int i, int j);
		float GetWih(int i);
		float GetWoh(int i);
		float GetError();
		void ShowWeights();
		Individu* Croisement(Individu *individu);
		Individu* Clonage();
		void Mutate();
		
		void SetWih(float wih, int i, int j);
		void SetWoh(float woh, int i, int j);
		void SetError(float error);
	
	private:
		void Init();
};

#endif
