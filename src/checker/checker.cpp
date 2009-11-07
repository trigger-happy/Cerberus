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


/*
	The checker assumes that the sequence of questions
	are also the same as the sequence of answers.
*/

#include <cmath>
#include <iostream>
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
		Question::Type questionType = m_qset->at(i).type;
		if(questionType == Question::IDENTIFICATION)
		{
			value += m_qset->at(i).score * (double)m_qset->at(i).checkAnswer(answerData[i].id_answer);
		}
		else if (questionType == Question::CHOOSE_ONE)
		{
			if(answerData[i].multi_choice.size() > 0) // does not check if there is nothing inside
			{
			value += floor(m_qset->at(i).score * (double)m_qset->at(i).checkAnswer(answerData[i].multi_choice[0]));
			}
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



/*
	Just a little warning though, if I select reset, it will definitely
	erase the objects contained in it! Make sure that this function is only
	called when everything needs to be reset.
*/
bool
Checker::resetQuestionSet()
{
	if(!m_qset->empty())
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
Checker::addQuestionSet( vector<Question> &myQSet )
{
	m_qset = myQSet;
	return true;
}*/
