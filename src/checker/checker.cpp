/*
Copyright (C) 2009 Niko Banasihan and Vernon Gutierrez

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
#include <iostream>
#include "xml_util.h"
#include "data_types.h"
#include "checker.h"

using namespace std;

Checker::Checker()
{
}


Checker::~Checker()
{
}

double
Checker::check(int round, QuestionData& qdata, AnswerData& answerData)
{
	double score;
	if(round == 1)
	{
		AnswerData *answerKey = vint[round];
		for(int i=0; i<answerKey->size(); i++)
		{
			if(answerData[i].compare(answerKey[i]))
				score+=qdata.questions[i].score;
		}
	}
	else if (round == 2)
	{
		AnswerData *answerKey = vint[round];
		/*
			Put the code for calculating the score
			for the second round here.
		*/
		for(int i=0; i<answerKey->end(); i++)
		{
			
		}
		
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
	
	return score;
}

void
Checker::setAnswerKey(int round, AnswerData& answerKey)
{
	if(round > 0 && round <= 4)
	{
		vint.push_back(pair<int, AnswerData>(round, answerKey) );
	}
	else
	{
		// throw and exception
	}
}


