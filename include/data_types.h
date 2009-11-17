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
#include <iostream>

using namespace std;

struct Question {

	struct AnswerKeyEntry {
		QString c;
		bool is_answer;
		AnswerKeyEntry() : is_answer( false ) {}

		AnswerKeyEntry( const QString &c, bool is_answer = false ) : c( c ), is_answer( is_answer ) {}

		bool operator==( const AnswerKeyEntry &rhs ) const {
			return c == rhs.c && is_answer == rhs.is_answer;
		}
	};

	typedef std::vector<AnswerKeyEntry> AnswerKey;

	enum Type {IDENTIFICATION, CHOOSE_ONE, CHOOSE_ANY};

	QString id;
	unsigned int score;
	int time_limit;
	QString question;
	QString answer;
	Type type;
	/*!
	  The choices/answers for the question. They're interpreted based on the type of the question:
		IDENTIFICATION: contains all valid answers (if entry's is_answer is true) for the question.
		CHOOSE_ONE: entries with is_answer set to 'true' can be selected (i.e. the contestant can choose any one of them)
		CHOOSE_ANY: all entries with is_answer set to 'true' should be selected.
	*/
	AnswerKey answer_key;

	Question() {
		score = 1;
		time_limit = -1;
	}

	bool hasTimeLimit() const {
		return time_limit > 0;
	}

	bool hasId() const {
		return !id.isEmpty();
	}

	bool operator==( const Question &rhs ) const {
		return id == rhs.id &&
		       score == rhs.score &&
		       time_limit == rhs.time_limit &&
		       question == rhs.question &&
		       type == rhs.type &&
		       answer_key.size() == rhs.answer_key.size() &&
		       std::equal( answer_key.begin(), answer_key.end(), rhs.answer_key.begin() );
	}

	/*!
	  Check the choices the contestant selected against the answer key.
	  \return boolean if the choice is correct.
	  \throws std::logic_error The Question's type is not CHOOSE_ONE.
	*/
	bool checkAnswer( const size_t choice ) const {
		if ( type != CHOOSE_ONE )
			throw std::logic_error( "Question::checkAnswer(const size_t) called on a non-CHOOSE_ONE type." );

		if ( choice > answer_key.size() )
			return false;

		return answer_key[choice-1].is_answer;
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
	double checkAnswer( const std::vector<size_t> &choices ) const {
		if ( type != CHOOSE_ANY )
			throw std::logic_error( "Question::checkAnswer(const std::vector<size_t>&) called on a non-CHOOSE_ANY type." );

		if ( answer_key.size() == 0 )
			throw std::length_error( "There are no entries in the answer key... (so how do you expect me to grade it, hmmm?!)" );

		double correct = 0;

		size_t choiceIndex = 0;

		/* Let me try another algorithm -- Nick
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
		}*/


		int key_correct_length = 0;

		// This is the one I'm using until I find a more efficient way of coding this.
		/*
			I've noticed that since the answer key in each question is invariant, I would
			rather that there is a more efficient way of finding the number of correct
			answers in the key rather than just by brute force, which I have done below
			-- Nick
		*/

		// count the number of correct answers
		for ( size_t i = 0; i < answer_key.size(); ++i ) {
			if ( answer_key[i].is_answer ) {
				key_correct_length++;
			}
		}

		QString result;

		for ( size_t i = 0; i < answer_key.size(); ++i ) {

			if ( choiceIndex < choices.size() && i == ( choices[choiceIndex] - 1 ) ) {
				// if it is one of the choices, and it is wrong,
				// no credit.
				if ( !answer_key[i].is_answer ) {
					correct = 0;
					break;
				} else {
					++correct;
				}

				++choiceIndex;
			}
		}

		return correct / key_correct_length;
	}

	/*!
	  Check the answer of the contestant in an identification question (case-insensitive).
	  \param ans The user input to be compared against.
	  \throws std::logic_error The Question's type is not IDENTIFICATION.
	  \throws std::length_error There are no entries in the answer_key.
	*/
	bool checkAnswer( const QString &ans ) const {
		if ( type != IDENTIFICATION )
			throw std::logic_error( "Question::checkAnswer(const QString &) called on a non-IDENTIFICATION type." );

		if ( answer_key.size() < 1 )
			throw std::length_error( "Answer key has no data to check against." );

		for ( size_t i = 0; i < answer_key.size(); ++i ) {
			if ( !answer_key[i].is_answer ) continue;

			if ( ans.compare( answer_key[i].c, Qt::CaseInsensitive ) == 0 )
				return true;
		}

		return false;
	}
};

/*!
This struct represents an answer to a single question.
ans_type determines which of the fields should be set.
IDENTIFICATION: id_answer must have the answer data.
CHOOSE_ONE or CHOOSE_ANY: multi_choice must contain the answer(s)
*/

struct Answer {
	Question::Type ans_type;
	// answer for identification
	QString id_answer;
	// answer for multiple choice
	std::vector<size_t> multi_choice;
};

typedef std::vector<Answer> AnswerData;

struct StageData {
	StageData() {
		contest_time = -1;
	}

	QString welcome_msg;
	std::vector<Question> questions;
	int contest_time;

	bool operator==( const StageData &rhs ) const {
		return welcome_msg == rhs.welcome_msg &&
		       questions.size() == rhs.questions.size() &&
		       std::equal( questions.begin(), questions.end(), rhs.questions.begin() ) &&
		       contest_time == rhs.contest_time;
	}

	bool hasContestTime() const {
		return contest_time > 0;
	}
};

struct NetworkConfig {
	QString ip;
	int port;

	bool operator==( const NetworkConfig &rhs ) const {
		return ip == rhs.ip && port == rhs.port;
	}

	virtual ~NetworkConfig() {}
};

struct ClientConfig : public NetworkConfig {
};

struct ProjectorConfig : public NetworkConfig {

	struct AuthorMode {

		struct ScoreEntry {
			QString name;
			QString group;
			std::vector<unsigned int> scores;
		};

		QString question;
		QString answer;
		unsigned int rounds;
		std::vector<ScoreEntry> scores;
	};

	QString theme_path;
	QString contest_name;
	/*! time update interval in milliseconds */
	unsigned int time_precision;
	AuthorMode *author_mode;

	bool hasAuthorMode() const {
		return author_mode != 0;
	}

	enum { DEFAULT_TIME_PRECISION = 100 };
	ProjectorConfig() :
			time_precision( DEFAULT_TIME_PRECISION ), author_mode( 0 ) {}

	~ProjectorConfig() {
		delete author_mode;
	}
};

struct AdminConfig : public NetworkConfig {
};

typedef QString StageFile;

struct ServerConfig {
	int port;
	QString db_path;
	std::vector<StageFile> stage_files;

	bool operator==( const ServerConfig &rhs ) const {
		return port == rhs.port &&
		       db_path == rhs.db_path &&
		       stage_files.size() == rhs.stage_files.size() &&
		       std::equal( stage_files.begin(), stage_files.end(), rhs.stage_files.begin() );
	}
};

struct RankData {
	bool operator<( const RankData& rhs ) const {
		if ( score > rhs.score ) {
			return true;
		} else if ( score == rhs.score ) {
			if ( time < rhs.time ) {
				return true;
			}
		}

		return false;
	}

	ushort rank;
	double score;
	uint time;
	QString fullname;
	QString teamname;
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
