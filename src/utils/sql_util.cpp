#include "sql_util.h"

bool SqlUtil::init(const QString& dbname){
	return true;
}

int SqlUtil::addTeam(const QString& team_name, const QString& school){
	return 0;
}

int SqlUtil::addUser(const UserData& ud){
	return 0;
}

int SqlUtil::getScore(const QString& user_name){
	return 0;
}

void SqlUtil::setScore(const QString& user_name, int score){
}

bool SqlUtil::authenticate(const QString& user_name, const QString& password){
	return true;
}
