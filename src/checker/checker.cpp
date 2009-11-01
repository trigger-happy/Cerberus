/*
Copyright (C) 2009 Nikolai Banasihan and Vernon Gutierrez

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/



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

/*
	The checker assumes that the sequence of questions
	are also the same as the sequence of answers.
*/
//#include <iostream>
//#include <vector>
#include <cmath>

#include "checker.h"
#include "data_types.h"


using namespace std;

// make sure that this is a singleton

Checker::Checker()
{
	m_qset = new vector<Question>();
}


Checker::~Checker()
{
	delete m_qset;
}

double
Checker::score(AnswerData &answerData)
{
	double value = 0;

	for(unsigned int i = 0; i < answerData.size(); i++)
	{
		// check type of question
		Question::Type questionType = answerData[i].ans_type;
		if(questionType == Question::IDENTIFICATION)
		{
			value += m_qset->at(i).score * (double)m_qset->at(i).checkAnswer(answerData[i].id_answer);
			
		}
		else if (questionType == Question::CHOOSE_ONE)
		{
			value += floor(m_qset->at(i).score * (double)m_qset->at(i).checkAnswer(answerData[i].multi_choice));
		}
		else if (questionType == Question::CHOOSE_ANY)
		{
			value += m_qset->at(i).score * m_qset->at(i).checkAnswer(answerData[i].multi_choice);
		}
		else
		{	
			// throw an exception
		}
		
	}
	
	return value;
}


/*double
Checker::score(Answer& answer)
{
	double value;
	
	// first check what type of question
	
	// call the proper method yehey
	// if IDENTIFICATION or CHOOSE_ONE, 
	// cast the value that the method returned
	if(question.type == IDENTIFICATION)
	{
		value = (double)question.checkAnswer();
	}
	else if (question.type == CHOOSE_ONE)
	{
		value = (double)question.checkAnswer();
	}
	else if (question.type == CHOOSE_ANY)
	{
		value = question.checkAnswer();
	}
	
	// multiply by the score
	value *= question.score;
	// return the value
	return value;
}*/

//
/*
	Just a little warning though, if I select reset, it will definitely
	erase the objects contained in it! Make sure that this function is only
	called when everything needs to be reset.
*/
bool
Checker::resetQuestionSet()
{
	if(m_qset->empty())
	{
		m_qset->clear();
		return true;
	}
	else
	{
		return false;
	}
	
}

// requires that the inputs must be sequential
bool
Checker::addQuestion(Question& myQuestion)
{
	m_qset->push_back(myQuestion);
	return true;
}

// allows you to add a whole vector of Questions (not require but nice to have)
/*
bool
Checker::addQuestionSet( vector<Question> &myQSet)
{
	m_qset = myQSet;
	return true;
}*/
