#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::pair;

struct R1Question{
	bool operator==(const R1Question& rhs){
		if(this->question != rhs.question){
			return false;
		}
		if(this->choices.size() == rhs.choices.size()){
			map<int, string>::const_iterator lit, rit;
			lit = this->choices.begin();
			rit = rhs.choices.begin();
			while(lit != this->choices.end()){
				if(lit->first != rit->first || lit->second != rit->second){
					return false;
				}
				lit++;
				rit++;
			}
		}
		return true;
	}
	string question;
	map<int, string> choices;
};

struct R1QData{
	bool operator==(const R1QData& rhs){
		if(this->welcome_msg != rhs.welcome_msg){
			return false;
		}
		if(this->questions.size() == rhs.questions.size()){
			for(int i = 0; i < this->questions.size(); i++){
				if(!(this->questions[i] == rhs.questions[i])){
					return false;
				}
			}
		}else{
			return false;
		}
		return true;
	}
	string welcome_msg;
	vector<R1Question> questions;
};

typedef map<int,int> R1Answers;

struct ClientConfig{
	string ip;
	int port;
};

struct StageData{
	string question_file;
	string answer_file;
};

struct ServerConfig{
	int port;
	string db_path;
	vector<StageData> stage_data;
};

#endif //DATA_TYPES_H