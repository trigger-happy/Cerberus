//3 rounds
//each round has 3 categories of questions
//each category has its own weight
//each correct answer for the first round has a corresponding score
//for the second round, muliple answers are possible
//dvide the weight by the number of correct answers
//if a team gets all the correct answers, and only the correct answers, they get the full weight
//if not, if the answers selected are all correct, but they are incomplete, they will get only the fraction of the weight based
//on the number of correct answers they got for the question
//if at least one of the selected answers is wrong, no credit will be given
//the third round is the same as the second round, except that there will be an option, "None of the above", wherein the question
//must be given a correct answer that is not in the choices
#include <iostream>
#include <xml_util.h>
#include "checker.h"

using namespace std;

Checker::Checker()
{
}


Checker::~Checker()
{
}

double
Checker::check(int round, AnswerData& answerData)
{
	switch(round)
	{
		case 1:
		return checkR1(answerData);
		case 2:
		return checkR2(answerData);
		case 3:
		return checkR3(answerData);
		case 4:
		return checkR4(answerData);
		default:
		//throw an exception
	}
}

void
Checker::setAnswerKey(int round, AnswerData& answerKey)
{
	switch(round)
	{
		case 1:
		this.answerKey1 = answerKey;
		break;
		case 2:
		this.answerKey2 = answerKey;
		break;
		case 3:
		this.answerKey3 = answerKey;
		break;
		case 4:
		this.answerKey4 = answerKey;
		default:
		// throw an exception
		
	}
}

/*
	checkR1(AnswerData&)
	The checker for this one will receive
	a set of values, and the score is the total
	score for the round.
*/

double 
Checker::checkR1(AnswerData& answerData)
{
	
	
}

/*
	checkR2(AnswerData&)
	The checker for this one is open to
	the possibility of two or more answers
	in the questions. So, the procedure is
	quite different.
*/
double
Checker::checkR2(AnswerData& answerData)
{
	
}


/*
	checkR3(AnswerData&)
	The checker for this one receives one
	question at a time, while maintaining
	the mechanics of the previous round.
*/
double
Checker::checkR3(AnswerData& answerData)
{
	
}

/*
	checkR4(AnswerData&)
	This is the same as the third round, except
	that the tie-breaker is really just good
	for a few questions.

*/
double
Checker::checkR4(AnswerData& answerData)
{
	
}
