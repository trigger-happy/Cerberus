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
	if(round == 1)
	{
		/*
			Put the code for calculating the score
			of the first round here.
		*/
	}
	else if (round == 2)
	{
		/*
			Put the code for calculating the score
			for the second round here.
		*/
	}
	else if (round == 3 || round == 4)
	{
		/*
			Put the code for calculating the score
			for the third and fourth round here.
		*/
	}
	else
	{
		// throw an exception
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


