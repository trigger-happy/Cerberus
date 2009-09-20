#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::equal;

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
	bool operator==(const ClientConfig& rhs){
		if(this->ip == rhs.ip && this->port == rhs.port){
			return true;
		}
		return false;
	}
	string ip;
	int port;
};

struct StageData{
	bool operator==(const StageData& rhs){
		if(question_file == rhs.question_file
			&& answer_file == rhs.answer_file){
			return true;
		}
		return false;
	}
	string question_file;
	string answer_file;
};

struct ServerConfig{
	bool operator==(const ServerConfig& rhs){
		if(port == rhs.port && db_path == rhs.db_path){
			if(stage_data.size() != rhs.stage_data.size()){
				return false;
			}
			if(equal(stage_data.begin() ,stage_data.end(), rhs.stage_data.begin())){
				return true;
			}
		}
		return false;
	}
	int port;
	string db_path;
	vector<StageData> stage_data;
};

struct UserData{
	string user_name;
	string password;
	string firstname;
	string lastname;
	string teamname;
};

struct TeamData{
	string teamname;
	string school;
};

struct ScoreData{
	string user_name;
	double score;
};

#endif //DATA_TYPES_H