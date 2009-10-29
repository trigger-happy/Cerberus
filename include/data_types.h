/*
Copyright (C) 2009 James Choa

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
#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <QString>

struct Question {
	struct AnswerKeyEntry {
		QString c;
		bool is_answer;
		AnswerKeyEntry() : is_answer(false) {}
		AnswerKeyEntry(const QString &c, bool is_answer = false) : c(c), is_answer(is_answer) {}

		bool operator==(const AnswerKeyEntry &rhs) const {
			return c == rhs.c && is_answer == rhs.is_answer;
		}
	};
	typedef std::vector<AnswerKeyEntry> AnswerKey;

	enum Type {IDENTIFICATION, CHOOSE_ONE, CHOOSE_ANY};

	QString id;
	unsigned int score;
	int time_limit;
	QString question;

	Type type;
	/*!
	  The choices/answers for the question. They're interpreted based on the type of the question:
		IDENTIFICATION: contains all valid answers (if entry's is_answer is true) for the question.
		CHOOSE_ONE: entries with is_answer set to 'true' can be selected (i.e. the contestant can choose any one of them)
		CHOOSE_ANY: all entries with is_answer set to 'true' should be selected.
	*/
	AnswerKey answer_key;

	Question(){
		score = 1;
		time_limit = -1;
	}

	bool hasTimeLimit() const { return time_limit > 0; }
	bool hasId() const { return !id.isEmpty(); }

	bool operator==(const Question &rhs) const {
		return id == rhs.id &&
			   score == rhs.score &&
			   time_limit == rhs.time_limit &&
			   question == rhs.question &&
			   type == rhs.type &&
			   answer_key.size() == rhs.answer_key.size() &&
			   std::equal(answer_key.begin(), answer_key.end(), rhs.answer_key.begin());
	}

	/*!
	  Check the choices the contestant selected against the answer key.
	  \return boolean if the choice is correct.
	  \throws std::logic_error The Question's type is not CHOOSE_ONE.
	*/
	bool checkAnswer(const size_t choice) const {
		if ( type != CHOOSE_ONE )
			throw std::logic_error("Question::checkAnswer(const size_t) called on a non-CHOOSE_ONE type.");
		if ( choice >= answer_key.size() )
			return false;
		return answer_key[choice].is_answer;
	}

	/*!
	  Check the choices the contestant selected against the answer key.
	  \param choices The choices of the user in ASCENDING ORDER (read: SORTED).
	  \return A double value [0,1] stating the number of correct answers (check what's supposed to be
			  checked and didn't check what's not supposed to be checked) over total choices. A perfect is exactly 1.0.
			  If the multiple choice has only one answer, it might be a good idea to floor the result
			  such that it can only be 0 or 1. Why? Because if the question has 4 answers and 1
			  out of the 4 is the correct one. If he just left it blank (i.e. 'none of the above'),
			  then he'll be getting .75 for that question which can be unfair.
	  \throws std::logic_error The Question's type is not CHOOSE_ANY.
	  \throws std::length_error There are no entries in the answer_key.
	*/
	double checkAnswer(const std::vector<size_t> &choices) const {
		if ( type != CHOOSE_ANY )
			throw std::logic_error("Question::checkAnswer(const std::vector<size_t>&) called on a non-CHOOSE_ANY type.");

		if ( answer_key.size() == 0 )
			throw std::length_error("There are no entries in the answer key... (so how do you expect me to grade it, hmmm?!)");

		double correct = 0;
		size_t choiceIndex = 0;

		for ( size_t i = 0; i < answer_key.size(); ++i ) {
			if ( choiceIndex < choices.size() && i == choices[choiceIndex] ) {
				//your answer matches the key.
				if ( answer_key[i].is_answer ) {
					correct += 1;
				}
				++choiceIndex;
			} else {
				//you didn't over-choose.
				if ( !answer_key[i].is_answer ) {
					correct += 1;
				}
			}
		}

		return correct/answer_key.size();
	}

	/*!
	  Check the answer of the contestant in an identification question (case-insensitive).
	  \param ans The user input to be compared against.
	  \throws std::logic_error The Question's type is not IDENTIFICATION.
	  \throws std::length_error There are no entries in the answer_key.
	*/
	bool checkAnswer(const QString &ans) const {
		if ( type != IDENTIFICATION )
			throw std::logic_error("Question::checkAnswer(const QString &) called on a non-IDENTIFICATION type.");
		if ( answer_key.size() < 1 )
			throw std::length_error("Answer key has no data to check against.");
		for ( size_t i = 0; i < answer_key.size(); ++i ) {
			if ( !answer_key[i].is_answer ) continue;
			if ( ans.compare(answer_key[i].c, Qt::CaseInsensitive) == 0 )
				return true;
		}
		return false;
	}
};

struct StageData {
	StageData(){
		contest_time = -1;
	}

	QString welcome_msg;
	std::vector<Question> questions;
	int contest_time;

	bool operator==(const StageData &rhs) const {
		return welcome_msg == rhs.welcome_msg &&
			   questions.size() == rhs.questions.size() &&
			   std::equal(questions.begin(), questions.end(), rhs.questions.begin()) &&
			   contest_time == rhs.contest_time;
	}

	bool hasContestTime() const { return contest_time > 0; }
};

struct NetworkConfig {
	QString ip;
	int port;

	bool operator==(const NetworkConfig &rhs) const {
		return ip == rhs.ip && port == rhs.port;
	}
	virtual ~NetworkConfig() {}
};

struct ClientConfig : public NetworkConfig {
};

struct PresenterConfig : public NetworkConfig {
};

struct AdminConfig : public NetworkConfig {
};

typedef QString StageFile;

struct ServerConfig {
	int port;
	QString db_path;
	std::vector<StageFile> stage_files;

	bool operator==(const ServerConfig &rhs) const {
		return port == rhs.port &&
			   db_path == rhs.db_path &&
			   stage_files.size() == rhs.stage_files.size() &&
			   std::equal(stage_files.begin(), stage_files.end(), rhs.stage_files.begin());
	}
};

struct UserData {
	QString user_name;
	QString password;
	QString firstname;
	QString lastname;
	QString teamname;
};

struct TeamData {
	QString teamname;
	QString school;
};

struct ScoreData {
	QString user_name;
	double score;
};

struct AdminData {
	QString user_name;
	QString password;
};

#endif //DATA_TYPES_H
