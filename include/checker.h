#include <iostream>
#include <xml_util.h>
#include <vector>

class Checker : public Singleton<Checker>
{
	public:
		Checker();
		~Checker();
	//int check(int round, AnswerData& ad, AnswerData& ak);
		double check(int round, AnswerData& answerData);
		
		void setAnswerKey(int round, AnswerData& answerKey);
	
	private:
		vector<int, AnswerData> vint;

		// special methods for rounds :D
		//double checkR1(AnswerData& answerData);
		//double checkR2(AnswerData& answerData);
		//double checkR3(AnswerData& answerData);
		//double checkR4(AnswerData& answerData);
	
};
