#include <iostream>
#include <xml_util.h>

class Checker : public Singleton<Checker>
{
	public:
		Checker();
		~Checker();
	//int check(int round, AnswerData& ad, AnswerData& ak);
		double check(int round, AnswerData& answerData);
		
		void setAnswerKey(int round, AnswerData& answerKey);
	
	private:
		AnswerData answerKey1;
		AnswerData answerKey2;
		AnswerData answerKey3;
		AnswerData answerKey4;

		// special methods for rounds :D
		double checkR1(AnswerData& answerData);
		double checkR2(AnswerData& answerData);
		double checkR34(AnswerData& answerData);
	
};