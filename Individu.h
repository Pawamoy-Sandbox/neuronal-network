#ifndef __INDIVIDU_H__
#define __INDIVIDU_H__

class Individu
{
	private:
		float m_wih[6];
		float m_woh[3];
		float m_error;
		
	public:
		Individu();
		~Individu();
		
		float GetWih(int i, int j);
		float GetWoh(int i, int j);
		float GetError();
		void ShowWeights();
		
		void SetWih(float wih, int i, int j);
		void SetWoh(float woh, int i, int j);
		void SetError(float error);
	
	private:
		void Init();
};

#endif
