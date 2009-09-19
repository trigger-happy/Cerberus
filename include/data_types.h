#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

struct R1Question{
	string question;
	map<int, string> choices;
};

struct R1QData{
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