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
	Usage:
	addQuestion(Question&) - use this to start inputting questions to be
	checked. Usually this is done for the start of the round. Note that only one Question
	object is added at a time. The sequence in which the Questions are pushed will also
	be in the same order in the vector of questions.
	
	resetQuestionSet() - this basically clears the contents of the question vector. This
	is expected to be called at the end of the round, or when the question set must be refreshed.
	Note that this calls clear(), which destroys all the objects that our Question set vector
	contains.
	
	score(AnswerData&) - The sequence of the Answers at the AnswerData vector must be correspondent
	to the sequence of the Questions in the Questions Set vector in this class. This should not
	be a problem since the order of the Questions are also controlled by the user of this class.

*/

#ifndef CHECKER_H
#define CHECKER_H

#include <iostream>
#include <vector>

#include "patterns/singleton.h"
#include "data_types.h"

using namespace std;

class Checker : public Singleton<Checker>
{
	private:
		vector<Question> *m_qset;

	public:
		Checker();
		~Checker();
		
		bool addQuestion(Question &myQuestion);
		bool resetQuestionSet();
		
		double score(AnswerData& answer);
		
	
};

#endif
